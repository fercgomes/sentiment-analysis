#include "Ranking.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include "HashTable.h"

Ranking::Ranking()
{
    SetOptions(0, 100);
}

void Ranking::SetOptions(int minOcur, int mSize)
{
    minOcurrences = minOcur;
    maxSize = mSize;
}

void Ranking::LoadRank(HashTable& hTable)
{
    std::size_t i = 0;
    int insertedCount = 0;
    float current_lbest,
          current_lworst;
    int current_locur;
    std::list<WordEntry*>::iterator it;

    ResetRank();

    while(insertedCount < maxSize)
    {
        if(!hTable.hashTable[i].empty())
        {
            it = hTable.hashTable[i].begin();
            while((it != hTable.hashTable[i].end()) && (insertedCount < maxSize))
            {
                if((*it)->count > minOcurrences)
                {
                    bestScores.push_back(*it);
                    worstScores.push_back(*it);
                    mostFrequent.push_back(*it);
                    insertedCount++;
                }
                it++;
            }
        }
        i++;
    }
    i--;

    std::sort(bestScores.begin(), bestScores.end(), [](WordEntry* a, WordEntry* b)
    {
        return a->averageScore > b->averageScore;
    });

    std::sort(worstScores.begin(),worstScores.end(), [](WordEntry* a, WordEntry* b)
    {
        return a->averageScore < b->averageScore;
    });

    std::sort(mostFrequent.begin(),mostFrequent.end(), [](WordEntry* a, WordEntry* b)
    {
        return a->count > b->count;
    });

    current_lbest = bestScores.back()->averageScore;
    current_lworst = worstScores.back()->averageScore;
    current_locur = mostFrequent.back()->count;

    while(i < hTable.size())
    {
        while(it != hTable.hashTable[i].end())
        {
            if((*it)->averageScore > current_lbest && ( (*it)->count > minOcurrences))
            {
                bestScores.pop_back();
                bestScores.push_back(*it);

                std::sort(bestScores.begin(), bestScores.end(), [](WordEntry* a, WordEntry* b)
                {
                    return a->averageScore > b->averageScore;
                });

                current_lbest = bestScores.back()->averageScore;
            }
            if((*it)->averageScore < current_lworst && ( (*it)->count > minOcurrences))
            {
                worstScores.pop_back();
                worstScores.push_back(*it);

                std::sort(worstScores.begin(),worstScores.end(), [](WordEntry* a, WordEntry* b)
                {
                    return a->averageScore < b->averageScore;
                });

                current_lworst = worstScores.back()->averageScore;
            }
            if((*it)->count > current_locur )
            {
                mostFrequent.pop_back();
                mostFrequent.push_back(*it);

                std::sort(mostFrequent.begin(),mostFrequent.end(), [](WordEntry* a, WordEntry* b)
                {
                    return a->count > b->count;
                });

                current_locur = mostFrequent.back()->count;
            }
            it++;
        }
        i++;
        it = hTable.hashTable[i].begin();
    }
}

void Ranking::ResetRank()
{
    bestScores.clear();
    worstScores.clear();
    mostFrequent.clear();
}