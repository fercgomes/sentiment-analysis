#pragma once

#include <list>
#include <vector>
#include "WordClassification.h"

struct InvertedFileEntry
{
    std::size_t wordPos;
    float* wordScore;
};

typedef struct InvertedFileEntry InvFileEntry;

class InvertedFile
{
public:
    std::vector< std::list<InvFileEntry> > invFile;

    InvertedFile();
    ~InvertedFile();

private:

};