#pragma once

#include "WordEntry.h"
#include "HashTable.h"
#include <list>
#include <vector>

class Ranking
{
    public:
        std::vector<WordEntry*> bestScores;
        std::vector<WordEntry*> worstScores;
        std::vector<WordEntry*> mostFrequent;

        int minOcurrences;
        int maxSize;

        Ranking();
        void SetOptions(int minOcurrences, int maxSize);
        void LoadRank(HashTable& hTable);
        void ResetRank();

};
