#pragma once
#include "HashTable.h"
#include "Trie.h"
#include <unordered_set>

class SentimentAnalyzer
{
public:
    /* flags */
    bool convertLowerCase;
    bool filterNonAlpha;
    bool removeStopWords;

    SentimentAnalyzer();    
    SentimentAnalyzer(std::size_t initSize);
    ~SentimentAnalyzer();

    void ImportFile(const char* filename);
    void LoadStopWords(const char* filename);

    WordEntry* GetWordEntry(std::string word);
    float GetCommentScore(std::string comment);    

    void PrintInvertedFile(std::string word);
    void PrintWords();

private:
    HashTable *wordEntries;
    TrieTree *preffixes;
    std::unordered_set<std::string> stopWords;

    bool isStopWord(std::string word);
};