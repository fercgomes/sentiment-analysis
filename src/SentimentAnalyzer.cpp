#include "SentimentAnalyzer.h"
#include <sstream>
#include <algorithm>

SentimentAnalyzer::SentimentAnalyzer()
{
    wordEntries = new HashTable();
    preffixes = new TrieTree;

    /* set flags */
    convertLowerCase = false;
    filterNonAlpha = false;
    removeStopWords = false;
}

SentimentAnalyzer::SentimentAnalyzer(std::size_t size)
{
    wordEntries = new HashTable(size);
    preffixes = new TrieTree;

    /* set flags */
    convertLowerCase = false;
    filterNonAlpha = false;
    removeStopWords = false;
}

SentimentAnalyzer::~SentimentAnalyzer()
{
    delete wordEntries;
    delete preffixes;
}

void SentimentAnalyzer::ImportFile(const char* filename)
{
    std::cout << "Importing from file " << filename << std::endl;
    if(convertLowerCase) std::cout << "Converting words to lowercase." << std::endl;
    if(filterNonAlpha) std::cout << "Filtering out non alphabetic characters." << std::endl;
    if(removeStopWords) std::cout << "Removing stop words." << std::endl;

    std::ifstream fp;
    fp.open(filename);
    bool running = true;

    if(fp.is_open())
    {
        /* get line count */
        std::size_t lineCount = std::count(std::istreambuf_iterator<char>(fp),
                                            std::istreambuf_iterator<char>(), '\n');

        /* set new size for hash table */
        wordEntries->expand(lineCount);

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
                    /* push into hash table */
                    wordEntries->push(word, commentScore, commentID, wordPos);
                    preffixes->push(word);
                    //std::cout << "inserting " << word << " from pos " << wordPos << ". id: " << commentID << ", score: " << commentScore << std::endl;
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
}

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

float SentimentAnalyzer::GetCommentScore(std::string comment)
{
    float total = 0, average = -1, wordScore, wordCount = 0;
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

            // std::cout << word << " has a score of " << wordScore << std::endl;
        }
    }

    average = total / wordCount;
    return average;
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
    for(std::size_t i = 0; i < wordEntries->size(); i++)
    {
        std::list<WordEntry*>::iterator it;
        auto hTable = wordEntries->hashTable[i];
        for(it = hTable.begin(); it != hTable.end(); it++)
        {
            auto word = (*it)->word;
            std::cout << word << std::endl; 
        }
    }
}

bool SentimentAnalyzer::isStopWord(std::string word)
{
    if(removeStopWords)
    {
        if( std::find(stopWords.begin(), stopWords.end(), word) != stopWords.end() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}