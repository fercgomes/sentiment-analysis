#pragma once
#include <list>
#include <string>
#include <iostream>

/* holds a document's word entries */
class CommentEntry
{
public:
    int commentID;
    double commentScore;
    std::list<std::size_t> ocurrences;

    CommentEntry(int id, int offset, double commentScore);

    std::size_t count();
};

/* the word entry, holds its information and inverted file */
class WordEntry
{
public:
    std::string word;
    double averageScore;
    double standardDeviation;
    unsigned int count;
    std::list<CommentEntry> invertedFile;

    WordEntry(std::string word);

    void AddOcurrence(int commentID, int commentOffset, double commentScore);
    void UpdateSandardDeviation();
    void PrintOcurrences();
    std::list<CommentEntry>& GetInvertFile();

    /* pretty printing on stdout */
    friend std::ostream& operator<<(std::ostream& os, const WordEntry* wD);
};