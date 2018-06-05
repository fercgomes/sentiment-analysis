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

    /* loads a .txt file with the comments and scores */
    void ImportFile(const char* filename);
    /* loads a .txt file containing stopwords from the english language */
    void LoadStopWords(const char* filename);

    /* returns a pointer to a word's entry */
    WordEntry* GetWordEntry(std::string word);
    /* returns the sentiment score for a word */
    float GetCommentScore(std::string comment);    

    /* returns a vector containing all words starting with 'pref' */
    std::list<std::string> GetPreffixes(std::string pref);

    /* prints all ocurrences of a word */
    void PrintInvertedFile(std::string word);
    /* prints all words within the structure */
    void PrintWords();

private:
    HashTable *wordEntries;
    TrieTree *preffixes;
    std::unordered_set<std::string> stopWords;

    bool isStopWord(std::string word);
};