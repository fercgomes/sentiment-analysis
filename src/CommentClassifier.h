#pragma once
#include "HashTable.h"
#include <unordered_map>
#include <list>

class CommentClassifier
{

public:
    CommentClassifier(HashTable* wordEntries);
    bool LoadWeights(const char* modifiersPath, const char* invertersFile);
    bool IsModifier(std::string word);
    bool IsInverter(std::string word);

    double GetScore(std::string comment);

private:
    HashTable* wordEntries;
    std::unordered_map<std::string, double> modifiers;
    std::list<std::string> inverters;

};