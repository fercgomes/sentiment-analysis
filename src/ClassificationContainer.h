/*
    This container holds pointers to the:
    * Resulting classification of a word
    * Its place on an inverted file
*/

#pragma once

#include "WordClassification.h"
#include "InvertedFile.h"
#include <string>

class WordContainer
{

public:
    std::string word;
    WordClassification* classification;
    InvertedFileEntry* invFile;

    WordContainer(std::string word);
    ~WordContainer();

};