#include "ClassificationContainer.h"
#include <string>

WordContainer::WordContainer(std::string wd)
{
    word = wd;
    classification = new WordClassification();
    invFile = new InvertedFileEntry(wd);
}

WordContainer::~WordContainer()
{
    delete classification;
    delete invFile;
}