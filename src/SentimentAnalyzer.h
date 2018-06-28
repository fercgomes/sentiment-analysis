#pragma once
#include "HashTable.h"
#include "Trie.h"
#include "CommentClassifier.h"
#include "Ranking.h"
#include "ConsoleApp.h"
#include <unordered_set>

class ConsoleApp;
class SentimentAnalyzer
{
public:
    HashTable                       *wordEntries;
    TrieTree                        *preffixes;
    std::unordered_set<std::string> stopWords;
    CommentClassifier*              classifier;
    Ranking                         ranking;

    /* points back to the application that owns this object */
    ConsoleApp *backPointer;

    /* flags */
    bool convertLowerCase;
    bool filterNonAlpha;
    bool removeStopWords;

    SentimentAnalyzer(ConsoleApp *app);    
    SentimentAnalyzer(std::size_t initSize, ConsoleApp* app);
    ~SentimentAnalyzer();

    /* loads a .txt file with the comments and scores */
    void ImportFile(const char* filename);

    /* loads a .txt file containing stopwords from the english language */
    void LoadStopWords(const char* filename);

    /* returns a pointer to a word's entry */
    WordEntry* GetWordEntry(std::string word);

    /* returns the sentiment score for a word */
    double GetCommentScore(std::string comment, int method);    

    /* classifies a file containing movie reviews */
    void GetCommentFileScore(const char* inPath, const char* outPath, int method);

    /* returns a vector containing all words starting with 'pref' */
    std::list<std::string> GetPreffixes(std::string pref);

    /* returns a list with the ranked best words */
    std::vector<WordEntry*>& GetBestRank();
    /* returns a list with the ranked worst words */
    std::vector<WordEntry*>& GetWorstRank();
    /* returns a list with the ranked words by ocurrence  */
    std::vector<WordEntry*>& GetOcurrencesRank();

    /* prints all ocurrences of a word */
    void PrintInvertedFile(std::string word);
    /* prints all words within the structure */
    void PrintWords();

    bool isStopWord(std::string word);
};