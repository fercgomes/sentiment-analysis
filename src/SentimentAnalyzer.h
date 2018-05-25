#pragma once
#include "HashTable.h"

class SentimentAnalyzer
{
public:
    HashTable *wordEntries;

    SentimentAnalyzer();    
    SentimentAnalyzer(std::size_t initSize);
    ~SentimentAnalyzer();

    void ImportFile(const char* filename);

    WordEntry* GetWordEntry(std::string word);
    float GetWordScore(std::string word);
    unsigned int GetWordCount(std::string word);
    float GetCommentScore(std::string comment);    

    void PrintInvertedFile(std::string word);
    void PrintWords();
};