#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <memory>
#include <fstream>
#include "WordEntry.h"

#define HASHTABLE_DEFAULT_SIZE  10      /* default size for initialization */
#define MAX_OCCUPANCY_RATIO     0.8     /* maximum ratio of occupancy */
#define MAX_CHAINED_OCCUPANCY   100     /* maximum number of elements in a chained list */
#define REHASH_FACTOR           1.3     /* the factor by which the hash size will be increased in rehashing */
#define TABLESIZE_FACTOR        1.2     /* the factor by which the initial hash table size will be calculated */

class HashTable
{
    using bigInt = unsigned long long;
    private:
        std::size_t     tableSize;
        unsigned int    collisions;
        bool            listLimitFlag;
        int             expansionCount;

        /* converts a string to integer */
        bigInt stringToInteger(std::string);
        /* hashes an integer using the hash table's size */
        bigInt hash(bigInt);
        /* hashes an integer using another integer */
        bigInt hash(bigInt, int);
        /* finds the nearest prime */
        bigInt nextPrime(bigInt);

        double  occupationRatio();
        bool    shouldExpand();
        bool    isPrime(unsigned long);
        void    rehash();

    public:
        /* the actual hash table */
        std::list<WordEntry*> *hashTable;

        HashTable();
        HashTable(std::size_t);
        ~HashTable();

        std::size_t size() const { return tableSize; }
        std::size_t storedElements();

        void        push(std::string word, double commentScore, int commentID, int wordPos);
        void        pop(std::string);
        WordEntry*  search(std::string);
        WordEntry*  operator[](std::string);

        void expand(std::size_t baseSize);
        void printReport();
        void printHashTable();
        void printWords();
};