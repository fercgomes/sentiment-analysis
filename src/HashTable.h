#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <memory>
#include <fstream>
#include "ClassificationContainer.h"

#define HASHTABLE_DEFAULT_SIZE  10
#define MAX_OCCUPANCY_RATIO     0.8
#define MAX_CHAINED_OCCUPANCY   25      /* maximum number of elements in a chained list */
#define REHASH_FACTOR           1.3     /* the factor by which the hash size will be increased in rehashing */
#define TABLESIZE_FACTOR        1.2     /* the factor by which the initial hash table size will be calculated */

class HashTable {
    private:
        unsigned int collisions;
        bool listLimitFlag;
        int rehashCount;

        std::size_t tableSize;

        unsigned long long stringToInteger(std::string);
        unsigned long hash(unsigned long long);
        unsigned long hash(unsigned long long, int);
        unsigned long nextPrime(unsigned long);
        float occupationRatio();
        bool shouldExpand();
        bool isPrime(unsigned long);
        void rehash();

        WordContainer* GetContainer(std::list<WordContainer*>& contList, std::string word);

    public:
        std::list<WordContainer*>* hashTable;

        HashTable();
        HashTable(std::size_t);

        ~HashTable();

        void push(std::string, float commentScore, std::size_t commentID, std::size_t wordPos);
        WordContainer* search(std::string);
        void pop(std::string);

        WordContainer* operator[](std::string);        

        void LoadFromFile(const char* filename);

        void printReport();
        void printHashTable();
};