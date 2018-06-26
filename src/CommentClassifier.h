#pragma once
#include "HashTable.h"
#include <unordered_map>
#include <list>

class CommentClassifier
{

public:
    /* default constructor */
    CommentClassifier(HashTable* wordEntries);

    /* loads files with modifiers and inverters */
    bool LoadWeights(const char* modifiersPath, const char* invertersFile);

    bool IsModifier(std::string word);
    bool IsInverter(std::string word);

    /* method selector */
    double GetScore(std::string comment, int method);

    /* methods */
    double RegularMean(std::string comment);
    double QuantityBasedMean(std::string comment);
    double FilteredMean(std::string comment);

private:
    HashTable* wordEntries;

    std::unordered_map<std::string, double> modifiers;
    std::list<std::string> inverters;

};