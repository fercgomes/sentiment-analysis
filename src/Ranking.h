#pragma once

#include "WordEntry.h"
#include "HashTable.h"
#include <list>
#include <vector>

class Ranking
{

    public:
        void make_rank(HashTable& hTable, unsigned int min_ocurr);

        std::vector<WordEntry*> best_score;
        std::vector<WordEntry*> worst_score;
        std::vector<WordEntry*> ocurrences;

        void print_rank(std::vector<WordEntry*> vetor,int mode,int k);
};
