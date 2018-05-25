#include "SentimentAnalyzer.h"
#include <sstream>
#include <algorithm>

SentimentAnalyzer::SentimentAnalyzer()
{
    wordEntries = new HashTable();
}

SentimentAnalyzer::SentimentAnalyzer(std::size_t size)
{
    wordEntries = new HashTable(size);
}

SentimentAnalyzer::~SentimentAnalyzer()
{
    delete wordEntries;
}

void SentimentAnalyzer::ImportFile(const char* filename)
{
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
            int i = 0;

            /* extract words and indices */
            /* format: int sentence */
            ss >> commentScore;

            while(i < line.size() && ss.tellg() != -1)
            {
                wordPos = ss.tellg() + 1;
                ss >> word;
                /*  TODO: filter non-alphabetic and trailing whitespaces
                    TODO: set verbose flag */
                
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                std::remove_if(word.begin(), word.end(), [](char c)
                {
                    return (c < 'a' || c > 'z');
                });

                if(!word.empty())
                {
                    /* push into hash table */
                    wordEntries->push(word, commentScore, commentID, wordPos);
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

WordEntry* SentimentAnalyzer::GetWordEntry(std::string word)
{
    return (*wordEntries)[word];
}

float SentimentAnalyzer::GetWordScore(std::string word)
{
    WordEntry* wEntry = GetWordEntry(word);
    if(wEntry != nullptr) return wEntry->averageScore;
    else return -1;
}

unsigned int SentimentAnalyzer::GetWordCount(std::string word)
{
    WordEntry* wEntry = GetWordEntry(word);
    if(wEntry != nullptr) return wEntry->count;
    else return 0;
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
            wordCount++;

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
    for(int i = 0; i < wordEntries->size(); i++)
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