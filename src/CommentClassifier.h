#pragma once
#include "HashTable.h"
#include <unordered_map>
#include <list>

class CommentClassifier
{

public:
    double saturationFactor;

    CommentClassifier(HashTable* wordEntries);
    bool LoadWeights(const char* modifiersPath, const char* invertersFile);
    bool IsModifier(std::string word);
    bool IsInverter(std::string word);

    /* methods */
    double GetScore(std::string comment, int method);

    double RegularMean(std::string comment);
    double QuantitySaturatedMean(std::string comment);

private:
    HashTable* wordEntries;
    std::unordered_map<std::string, double> modifiers;
    std::list<std::string> inverters;

};