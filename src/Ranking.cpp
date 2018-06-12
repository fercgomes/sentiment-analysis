#include "Ranking.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include "HashTable.h"

constexpr int tam_rank = 10;

void Ranking::make_rank(HashTable& hTable, unsigned int min_ocurr)
{
    std::size_t i = 0;
    int inseridos = 0;
    float current_lbest,
          current_lworst;
    int current_locur;
    std::list<WordEntry*>::iterator it;

    while( inseridos < tam_rank )
    {
        if( !hTable.hashTable[i].empty() )
        {
            it = hTable.hashTable[i].begin();
            while((it != hTable.hashTable[i].end()) && (inseridos<tam_rank))
            {
                if( (*it)->count > min_ocurr){
                best_score.push_back(*it);
                worst_score.push_back(*it);
                ocurrences.push_back(*it);
                inseridos++;
                }
                it++;
            }
        }
        i++;
    }
    i--;

    std::sort(best_score.begin(), best_score.end(), [](WordEntry* a, WordEntry* b)
    {
        return a->averageScore > b->averageScore;
    });

    std::sort(worst_score.begin(),worst_score.end(), [](WordEntry* a, WordEntry* b)
    {
        return a->averageScore < b->averageScore;
    });

    std::sort(ocurrences.begin(),ocurrences.end(), [](WordEntry* a, WordEntry* b)
    {
        return a->count > b->count;
    });

    current_lbest = best_score.back()->averageScore;
    current_lworst = worst_score.back()->averageScore;
    current_locur = ocurrences.back()->count;

    while(i < hTable.size())
    {

        while(it != hTable.hashTable[i].end())
        {
            if((*it)->averageScore > current_lbest && ( (*it)->count > min_ocurr))
            {

                best_score.pop_back();
                best_score.push_back(*it);

                std::sort(best_score.begin(), best_score.end(), [](WordEntry* a, WordEntry* b)
                {
                    return a->averageScore > b->averageScore;
                });

                current_lbest = best_score.back()->averageScore;

            }
            if((*it)->averageScore < current_lworst && ( (*it)->count > min_ocurr))
            {
                worst_score.pop_back();
                worst_score.push_back(*it);

                std::sort(worst_score.begin(),worst_score.end(), [](WordEntry* a, WordEntry* b)
                {
                    return a->averageScore < b->averageScore;
                });

                current_lworst = worst_score.back()->averageScore;

            }
            if((*it)->count > current_locur )
            {
                ocurrences.pop_back();
                ocurrences.push_back(*it);

                std::sort(ocurrences.begin(),ocurrences.end(), [](WordEntry* a, WordEntry* b)
                {
                    return a->count > b->count;
                });

                current_locur = ocurrences.back()->count;

            }
            it++;
        }
        i++;
        it = hTable.hashTable[i].begin();


    }
}

void Ranking::print_rank(std::vector<WordEntry*> vetor,int mode,int k)
{
    int i;

    if(mode == 0)
    {
        std::ofstream saida("saida0.txt");

        saida << "Melhores escores: \n";
        for(i=0; i<k; i++)
        {
            saida << vetor[i]->averageScore<<" "<<vetor[i]->word<<"\n";
        }
        saida.close();
    }
    else if(mode == 1)
    {
        std::ofstream saida("saida1.txt");
        saida << "Piores escores: \n";
        for(i=0; i<k; i++)
        {
            saida << vetor[i]->averageScore<<" "<<vetor[i]->word<<"\n";
        }
        saida.close();
    }
    else if(mode == 2)
    {
        std::ofstream saida("saida2.txt");
        saida <<"frequencias: \n";
        for(i=0; i<k; i++)
        {
            saida << vetor[i]->count <<" "<<vetor[i]->word<<"\n";
        }
        saida.close();

    }
}
