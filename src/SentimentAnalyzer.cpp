#include "SentimentAnalyzer.h"
#include <sstream>
#include <algorithm>

#define INIT_SIZE_FACTOR 1.3

SentimentAnalyzer::SentimentAnalyzer()
{
    // std::cout << "SA: initializing." << std::endl;

    wordEntries = new HashTable();
    preffixes = new TrieTree;

    /* set flags */
    convertLowerCase = false;
    filterNonAlpha = false;
    removeStopWords = false;

    /* initializes the classifier */
    classifier = new CommentClassifier(wordEntries);
}

SentimentAnalyzer::SentimentAnalyzer(std::size_t size)
{
    // std::cout << "SA: initializing." << std::endl;

    wordEntries = new HashTable(size);
    preffixes = new TrieTree;

    /* set flags */
    convertLowerCase = false;
    filterNonAlpha = false;
    removeStopWords = false;

    /* initializes the classifier */
    classifier = new CommentClassifier(wordEntries);
}

SentimentAnalyzer::~SentimentAnalyzer()
{
    // std::cout << "SA: destroying." << std::endl;

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
    // std::cout << "SA: importing from file " << filename << std::endl;
    // if(convertLowerCase) std::cout << "SA: converting words to lowercase." << std::endl;
    // if(filterNonAlpha) std::cout << "SA: filtering out non alphabetic characters." << std::endl;
    // if(removeStopWords) std::cout << "SA: removing stop words." << std::endl;

    std::ifstream fp;
    fp.open(filename);
    bool running = true;

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
                    std::remove_if(word.begin(), word.end(), [](char c)
                    {
                        return (c < 'a' || c > 'z');
                    });
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
        }
        fp.close();
    }


    /* done loading */

    /* initializes ranking */
    ranking.make_rank(*wordEntries, 0);
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

/************************************************************
 * GetCommentScore
 * 
 * Given a string containing a sentence, it calculates the
 * average score with each words individual score.
 ***********************************************************/
float SentimentAnalyzer::GetCommentScore(std::string comment)
{
    float total = 0, average = 0, wordScore, wordCount = 0;
    WordEntry* wEntry;
    std::string word;

    std::stringstream ss(comment);
    while(ss.tellg() != -1)
    {
        word.clear();
        ss >> word;
        wEntry = GetWordEntry(word);
        if(wEntry != nullptr)
        {
            /* word has a score */
            wordScore = wEntry->averageScore;

            total += wordScore;
            wordCount += 1;
        }
    }

    if(total)
    {
        average = total / wordCount;
    }

    return average;
}

double SentimentAnalyzer::GetCommentWeightedScore(std::string comment)
{
    return classifier->GetScore(comment);
}

void SentimentAnalyzer::GetCommentFileScore(const char* inPath, const char* outPath)
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
        double commentScore = GetCommentScore(comment);
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
    return ranking.best_score;
}

std::vector<WordEntry*>& SentimentAnalyzer::GetWorstRank()
{
    return ranking.worst_score;
}

std::vector<WordEntry*>& SentimentAnalyzer::GetOcurrencesRank()
{
    return ranking.ocurrences;
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
        std::cout << word << " not found." << std::endl;
    }
}

void SentimentAnalyzer::PrintWords()
{
    wordEntries->printWords();
}

bool SentimentAnalyzer::isStopWord(std::string word)
{
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