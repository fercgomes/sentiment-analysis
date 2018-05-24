#pragma once

#include <list>
#include <string>
#include "WordClassification.h"

class CommentEntry
{
public:
    std::size_t commentID;
    float commentScore;
    std::list<std::size_t> ocurrences;

    CommentEntry(std::size_t id, float score)
    {
        commentScore = score;
        commentID = id;
    }    
};

class InvertedFileEntry
{
public:
    std::list<CommentEntry> invFile;
    std::string word;

    InvertedFileEntry(std::string w);
    ~InvertedFileEntry();

    void AddWordOcurrence(std::size_t commentID, std::size_t wordPos, float commentScore);
    void PrintOcurrences();
};