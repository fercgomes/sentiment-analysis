#pragma once
#include <list>
#include <string>
#include <iostream>

class CommentEntry
{
public:
    int commentID;
    float commentScore;
    std::list<std::size_t> ocurrences;

    CommentEntry(int id, int offset, float commentScore);

    std::size_t count();
};

class WordEntry
{
public:
    std::string word;
    float averageScore;
    double standardDeviation;
    unsigned int count;
    std::list<CommentEntry> invertedFile;

    WordEntry(std::string word);

    void AddOcurrence(int commentID, int commentOffset, float commentScore);
    void RoundAll();
    void UpdateSandardDeviation();
    void PrintOcurrences();
    std::list<CommentEntry>& GetInvertFile();

    /* stdout */
    friend std::ostream& operator<<(std::ostream& os, const WordEntry* wD);
};