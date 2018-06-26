#include "SentimentAnalyzer.h"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "ConsoleApp.h"

#define INIT_SIZE_FACTOR 1.3

/* logging purposes */
std::ofstream logger("logger.txt");

SentimentAnalyzer::SentimentAnalyzer(ConsoleApp *app)
{
    logger << "SA: initializing." << std::endl;

    wordEntries = new HashTable();
    preffixes = new TrieTree;

    /* set flags */
    convertLowerCase = false;
    filterNonAlpha = false;
    removeStopWords = false;

    backPointer = app;

    /* initializes the classifier */
    classifier = new CommentClassifier(wordEntries);
}

SentimentAnalyzer::SentimentAnalyzer(std::size_t size, ConsoleApp *app)
{
    logger << "SA: initializing." << std::endl;

    wordEntries = new HashTable(size);
    preffixes = new TrieTree;

    /* set flags */
    convertLowerCase = false;
    filterNonAlpha = false;
    removeStopWords = false;

    backPointer = app;

    /* initializes the classifier */
    classifier = new CommentClassifier(wordEntries);
}

SentimentAnalyzer::~SentimentAnalyzer()
{
    logger << "SA: destroying." << std::endl;

    delete wordEntries;
    delete preffixes;
}

/*******************************************************
 * ImportFile
 * 
 * File format:
 * x foo bar
 * 
 * x: comment score
 * "foo bar": comment
 * 
 * ImportFile will use the line count times a constant
 * to initialize a Hash Table, and push each word in
 * each comment into the corresponding word entry. The
 * word is also pushed into the Trie Tree.
 ******************************************************/
void SentimentAnalyzer::ImportFile(const char* filename)
{
    logger << "SA: importing from file " << filename << std::endl;
    if(convertLowerCase) logger << "SA: converting words to lowercase." << std::endl;
    if(filterNonAlpha) logger << "SA: filtering out non alphabetic characters." << std::endl;
    if(removeStopWords) logger << "SA: removing stop words." << std::endl;

    std::ifstream fp;
    fp.open(filename);
    bool running = true;

    /* starts clock */
    auto start = std::chrono::steady_clock::now();
    if(fp.is_open())
    {
        /* get line count */
        std::size_t lineCount = std::count(std::istreambuf_iterator<char>(fp),
                                           std::istreambuf_iterator<char>(), '\n');

        /* set new size for hash table */
        wordEntries->expand(lineCount * INIT_SIZE_FACTOR);

        /* reset input buffer */
        fp.seekg(0);

        std::string line;
        std::string word;
        int commentID = 0;

        std::getline(fp, line);
        while(running)
        {
            std::stringstream ss(line);
            int commentScore;
            int wordPos;
            std::size_t i = 0;

            /* extract words and indices */
            /* format: int sentence */
            ss >> commentScore;

            while(i < line.size() && ss.tellg() != -1)
            {
                wordPos = static_cast<int>(ss.tellg()) + 1;
                ss >> word;
                
                if(convertLowerCase)
                {
                    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                }
                if(filterNonAlpha)
                {
                    word.erase(std::remove_if(word.begin(), word.end(), [](char c)
                    {
                        return !isalpha(c);
                    }), word.end());
                }

                if(word.size() > 1 && !isStopWord(word))
                {
                    /* pushes into hash table */
                    wordEntries->push(word, commentScore, commentID, wordPos);
                    /* pushes into Trie Tree */
                    preffixes->push(word);
                }

                i = wordPos + word.size();
            }

            if(fp.eof()) running = false;
            else
            {
                std::getline(fp, line);
                commentID++;
            }

            int loadProgress = ( (double)commentID / (double)lineCount * 100 );
            backPointer->UpdateProgress(0, 0, loadProgress);
        }
        fp.close();

        /* logging */
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        logger << "Loading " << filename << " took " << elapsed.count() << " seconds." << std::endl;
        logger << wordEntries->storedElements() << " words loaded." << std::endl;
    }

    /* done loading */
    /* logs the hashtable */
    wordEntries->printHashTable();
}

/* loads lines of text containing stopwords into a set */
void SentimentAnalyzer::LoadStopWords(const char* filename)
{
    std::ifstream fp(filename);
    if(fp.is_open())
    {
        std::string line;
        while(fp >> line)
        {
            stopWords.insert(line);
        }

        fp.close();
    } 
}

WordEntry* SentimentAnalyzer::GetWordEntry(std::string word)
{
    return (*wordEntries)[word];
}

/* selects the score method */
double SentimentAnalyzer::GetCommentScore(std::string comment, int method)
{
    return classifier->GetScore(comment, method);
}

/* classifies a whole file of comments */
void SentimentAnalyzer::GetCommentFileScore(const char* inPath, const char* outPath, int method)
{
    std::ifstream in_fp;
    std::ofstream out_fp;

    in_fp.open(inPath);
    out_fp.open(outPath);

    std::string comment;
    int i = 0;

    while(!in_fp.eof())
    {
        std::getline(in_fp, comment);
        /* TODO: method */
        double commentScore = GetCommentScore(comment, method);
        out_fp << "Comment #" << i << ": " << commentScore << std::endl;

        i++;
    }

}

std::list<std::string> SentimentAnalyzer::GetPreffixes(std::string pref)
{
    return preffixes->getPreffixes(pref);
}

std::vector<WordEntry*>& SentimentAnalyzer::GetBestRank()
{
    return ranking.bestScores;
}

std::vector<WordEntry*>& SentimentAnalyzer::GetWorstRank()
{
    return ranking.worstScores;
}

std::vector<WordEntry*>& SentimentAnalyzer::GetOcurrencesRank()
{
    return ranking.mostFrequent;
}

void SentimentAnalyzer::PrintInvertedFile(std::string word)
{
    WordEntry* wEntry = GetWordEntry(word);
    if(wEntry != nullptr)
    {
        wEntry->PrintOcurrences();
    }
    else
    {
        logger << word << " not found." << std::endl;
    }
}

void SentimentAnalyzer::PrintWords()
{
    wordEntries->printWords();
}

bool SentimentAnalyzer::isStopWord(std::string word)
{
    /* all stopwords are stored in lowercase */
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    if(removeStopWords)
    {
        /* if word is in the set */
        if( std::find(stopWords.begin(), stopWords.end(), word) != stopWords.end() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /* if stopwords are not taken in consideration, it always returns false */
    else
    {
        return false;
    }
}