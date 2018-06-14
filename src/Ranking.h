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

        /* minimum number of ocurrences for a word to appear on the ranking */
        int minOcurrences;
        /* limits the number of words on the ranking */
        int maxSize;

        /* default constructor */
        Ranking();

        /* sets minOcurrences and maxSize */
        void SetOptions(int minOcurrences, int maxSize);

        /* constructs a ranking */
        void LoadRank(HashTable& hTable);

        /* resets the ranking */
        void ResetRank();

};
