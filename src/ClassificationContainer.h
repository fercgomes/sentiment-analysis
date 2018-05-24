/*
    This container holds pointers to the:
    * Resulting classification of a word
    * Its place on an inverted file
*/

#include "WordClassification.h"

struct WordContainer {
    WordClassification* classification;
    // InvertedFile* invFile;
}

typedef struct WordContainer WContainer;