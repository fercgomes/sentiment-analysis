/*
    Hash Table implementation
*/

#include <fstream>
#include <chrono>
#include "HashTable.h"
#include "WordEntry.h"

extern std::ofstream logger;

/*
    Initialization procedure
    - Set hash table size
    - Allocate empty lists
*/
HashTable::HashTable()
{
    logger << "HASHTABLE: intializing with default size of " << HASHTABLE_DEFAULT_SIZE << "." << std::endl;
    
    /* allocate with default size */
    hashTable = new std::list<WordEntry*> [HASHTABLE_DEFAULT_SIZE];
    tableSize = HASHTABLE_DEFAULT_SIZE;

    collisions = 0;
    listLimitFlag = false;
    expansionCount = 0;
}

HashTable::HashTable(std::size_t elements)
{
    std::size_t calculatedSize = std::ceil(TABLESIZE_FACTOR * elements);
    logger << "HASHTABLE: intializing with given size of " << calculatedSize << "." << std::endl;

    tableSize = calculatedSize;
    hashTable = new std::list<WordEntry*> [calculatedSize];

    collisions = 0;
    listLimitFlag = false;
    expansionCount = 0;
}

HashTable::~HashTable()
{
    logger << "HASHTABLE: destroying." << std::endl;
    
    /* deallocate the list */
    delete[] hashTable;
}

std::size_t HashTable::size()
{
    return tableSize;
}

std::size_t HashTable::storedElements()
{
    std::size_t count = 0;
    for(int i = 0; i < tableSize; i++)
    {
        auto current = hashTable[i];
        if(!current.empty())
        {
            count += current.size();                
        }
    }
    return count;
}

/************
Math related
************/

bool HashTable::isPrime(unsigned long number)
{
    if (number == 2 || number == 3)
        return true;
    if (number % 2 == 0 || number % 3 == 0)
        return false;

    unsigned long divisor = 6;
    while (divisor * divisor - 2 * divisor + 1 <= number)
    {
        if (number % (divisor - 1) == 0)
            return false;
        if (number % (divisor + 1) == 0)
            return false;
        divisor += 6;
    }

    return true;
}

unsigned long long HashTable::nextPrime(unsigned long long a)
{
    while (!isPrime(++a)){}
    return a;
}

/* expands the hash table by a constant factor */
void HashTable::expand(std::size_t baseSize)
{
    logger << "HASHTABLE: Resizing to base size of " << baseSize << std::endl;
    auto start = std::chrono::steady_clock::now();

    std::size_t newSize = nextPrime(baseSize);
    std::list<WordEntry*> *newHashTable = new std::list<WordEntry*> [newSize];
    std::list<WordEntry*>::iterator it;

    collisions = 0;

    /* iterates over the elements in the old hash table */
    for(std::size_t i = 0; i < tableSize; i++)
    {
        if(!hashTable[i].empty())
        {
            for(it = hashTable[i].begin(); it != hashTable[i].end(); it++)
            {
                /* pushed old elements into the new hash table */
                auto key = hash(stringToInteger((*it)->word), newSize);
                if(!newHashTable[key].empty()) collisions++;
                newHashTable[key].push_front(*it);
            }
        }
    }

    /* returns the new hash table */
    tableSize = newSize;
    delete[] hashTable;
    hashTable = newHashTable;
    listLimitFlag = false;
    
    expansionCount++;

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    logger << "Expansion took " << elapsed.count() << " seconds." << std::endl;
}

void HashTable::rehash()
{
    std::size_t newSize = std::ceil(tableSize * REHASH_FACTOR);
    expand(newSize);
}

/* returns the ratio (occupied slots) / (total slots) */
double HashTable::occupationRatio()
{
    double occupiedSlots = 0;
    for(std::size_t i = 0; i < tableSize; i++)
    {
        if(!hashTable[i].empty()) occupiedSlots++;
    }
    double result = occupiedSlots / tableSize;
    return result;
}

/* tests if the hash table is too full and has to be expanded */
/* criteria: 80% of the list if full */
bool HashTable::shouldExpand()
{
    if(occupationRatio() >= MAX_OCCUPANCY_RATIO) return true;
    else if(listLimitFlag) return true;
    else return false;
}

/* converts a string into an integer */
unsigned long long HashTable::stringToInteger(std::string name)
{
    unsigned long long stringKey = 0,
                  characterInt;

    std::string::iterator character;   
    int i = 0;
    for(character = name.end(); character != name.begin(); character--){
        characterInt = (unsigned long long) (*character);
        stringKey = stringKey + (characterInt * std::pow(8, i));
        i++;
    }

    return stringKey;
}

/* hashes a key using the division method */
/* hash(key) = key % (tableSize/modulo) */
unsigned long long HashTable::hash(unsigned long long key)
{
    return key % tableSize; 
}

unsigned long long HashTable::hash(unsigned long long key, int modulo)
{
    return key % modulo;
}

/* Hash Table Operations */
/* inserts a string into the hash table */
void HashTable::push(std::string word, double score, int id, int offset)
{
    if(!word.size())
    {
        std::cerr << "ERROR: Can't insert an empty string into the Hash Table." << std::endl;
    }
    else
    {
        unsigned long key = hash(stringToInteger(word));
    

        /* Check if word exists already */
        WordEntry* lookup = search(word);
        if(lookup == nullptr)
        {
            /* check if there is a collision */
            if(!hashTable[key].empty())
                collisions++;

            /* no entry so far, make a new one */
            lookup = new WordEntry(word);

            /* inserts into hash table */
            hashTable[key].push_front(lookup);
        }
        
        /* push new word ocurrence */
        lookup->AddOcurrence(id, offset, score);

        /* checks expansion conditions */
        if(hashTable[key].size() > MAX_CHAINED_OCCUPANCY) listLimitFlag = true;

        if(shouldExpand())
        {
            rehash(); 
            logger << "Hash Table will be expanded!" << std::endl;
        }
    }
}

/* returns a pointer to a word's entry */
WordEntry* HashTable::search(std::string word)
{
    if(word.empty())
    {
        return nullptr;
    }
    else
    {
        unsigned long key = hash(stringToInteger(word));

        if(hashTable[key].empty()){
            return nullptr;
        }
        else
        {
            std::list<WordEntry*>::iterator it;
            for(it = hashTable[key].begin(); it != hashTable[key].end(); it++)
            {
                if((*it)->word == word)
                {
                    return (*it);
                }
            }
            return nullptr;
        }
    }
}

WordEntry* HashTable::operator[](std::string word)
{
    return search(word);
}

void HashTable::printReport()
{
    logger << "====================================" << std::endl;
    logger << "Details about the Hash Table: " << std::endl;
    logger << "Table size:\t" << tableSize << std::endl;
    logger << "Occupancy rate:\t" << occupationRatio() << std::endl;
    logger << "Collisions:\t" << collisions << std::endl;
    logger << "Expansions:\t" << expansionCount << std::endl;
    logger << "====================================" << std::endl;
    logger << "PARAMETERS:" << std::endl;
    logger << "HASHTABLE_DEFAULT_SIZE:\t\t" << HASHTABLE_DEFAULT_SIZE << std::endl;
    logger << "MAX_OCCUPANCY_RATIO:\t\t" << MAX_OCCUPANCY_RATIO << std::endl;
    logger << "MAX_CHAINED_OCCUPANCY:\t\t" << MAX_CHAINED_OCCUPANCY << std::endl; 
    logger << "REHASH_FACTOR:\t\t\t" << REHASH_FACTOR << std::endl;         
    logger << "TABLESIZE_FACTOR:\t\t" << TABLESIZE_FACTOR << std::endl; 
}

void HashTable::printHashTable()
{
    logger << "Hash Table (showing linked lists): " << std::endl;
    for(std::size_t i = 0; i < tableSize; i++)
    {
        logger << i << ":\t";
        if(hashTable[i].empty()) logger << "NULL";
        else
        {
            std::list<WordEntry*>::iterator it;
            for(it = hashTable[i].begin(); it != hashTable[i].end(); it++)
            {
                logger << (*it)->word << " -> "; 
            }
        }
        logger << std::endl;
    }

    printReport();
}

void HashTable::printWords()
{
    for(std::size_t i = 0; i < size(); i++)
    {
        std::list<WordEntry*>::iterator it;
        auto hTable = hashTable[i];
        for(it = hTable.begin(); it != hTable.end(); it++)
        {
            auto word = (*it)->word;
            logger << word << std::endl; 
        }
    }
}