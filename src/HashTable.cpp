/*
    Hash Table implementation
*/

#include <fstream>
#include "HashTable.h"
#include "WordEntry.h"

/*
    Initialization procedure
    - Set hash table size
    - Allocate empty lists
*/
HashTable::HashTable()
{
    // std::cout << "HASHTABLE: intializing with default size of " << HASHTABLE_DEFAULT_SIZE << "." << std::endl;
    
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
    // std::cout << "HASHTABLE: intializing with given size of " << calculatedSize << "." << std::endl;

    tableSize = calculatedSize;
    hashTable = new std::list<WordEntry*> [calculatedSize];

    collisions = 0;
    listLimitFlag = false;
    expansionCount = 0;
}

HashTable::~HashTable()
{
    // std::cout << "HASHTABLE: destroying." << std::endl;
    
    /* deallocate the list */
    delete[] hashTable;
}

std::size_t HashTable::size()
{
    return tableSize;
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

unsigned long HashTable::nextPrime(unsigned long a)
{
    while (!isPrime(++a)){}
    return a;
}

/* expands the hash table by a constant factor */
void HashTable::expand(std::size_t baseSize)
{
    // std::cout << "HASHTABLE: Resizing to base size of " << baseSize << std::endl;

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
}

void HashTable::rehash()
{
    std::size_t newSize = std::ceil(tableSize * REHASH_FACTOR);
    expand(newSize);
}

/* returns the ratio (occupied slots) / (total slots) */
float HashTable::occupationRatio()
{
    float occupiedSlots = 0;
    for(std::size_t i = 0; i < tableSize; i++)
    {
        if(!hashTable[i].empty()) occupiedSlots++;
    }
    float result = occupiedSlots / tableSize;
    return result;
}

/* tests if the hash table is too full and has to be expanded */
/* criteria: 80% of the list if full */
bool HashTable::shouldExpand()
{
    if(occupationRatio() >= MAX_OCCUPANCY_RATIO) return true;
    // else if(listLimitFlag) return true;
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
        stringKey = stringKey + (characterInt * std::pow(32, i));
        i++;
    }

    return stringKey;
}

/* hashes a key using the division method */
/* hash(key) = key % (tableSize/modulo) */
unsigned long HashTable::hash(unsigned long long key)
{
    return key % tableSize; 
}

unsigned long HashTable::hash(unsigned long long key, int modulo)
{
    return key % modulo;
}

/* Hash Table Operations */
/* inserts a string into the hash table */
void HashTable::push(std::string word, float score, int id, int offset)
{
    if(!word.size())
    {
        std::cerr << "ERROR: Can't insert an empty string into the Hash Table." << std::endl;
    }
    else
    {
        unsigned long key = hash(stringToInteger(word));
    
        /* check if there is a collision */
        if(!hashTable[key].empty())
            collisions++;

        /* Check if word exists already */
        WordEntry* lookup = search(word);
        if(lookup == nullptr)
        {
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
            // std::cout << "Hash Table will be expanded!" << std::endl;
        }
    }
}

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
    std::cout << "====================================" << std::endl;
    std::cout << "Details about the Hash Table: " << std::endl;
    std::cout << "Table size:\t" << tableSize << std::endl;
    std::cout << "Occupancy rate:\t" << occupationRatio() << std::endl;
    std::cout << "Collisions:\t" << collisions << std::endl;
    std::cout << "Expansions:\t" << expansionCount << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "PARAMETERS:" << std::endl;
    std::cout << "HASHTABLE_DEFAULT_SIZE:\t\t" << HASHTABLE_DEFAULT_SIZE << std::endl;
    std::cout << "MAX_OCCUPANCY_RATIO:\t\t" << MAX_OCCUPANCY_RATIO << std::endl;
    std::cout << "MAX_CHAINED_OCCUPANCY:\t\t" << MAX_CHAINED_OCCUPANCY << std::endl; 
    std::cout << "REHASH_FACTOR:\t\t\t" << REHASH_FACTOR << std::endl;         
    std::cout << "TABLESIZE_FACTOR:\t\t" << TABLESIZE_FACTOR << std::endl; 
}

void HashTable::printHashTable()
{
    std::cout << "Hash Table (showing linked lists): " << std::endl;
    for(std::size_t i = 0; i < tableSize; i++)
    {
        std::cout << i << ":\t";
        if(hashTable[i].empty()) std::cout << "NULL";
        else
        {
            std::list<WordEntry*>::iterator it;
            for(it = hashTable[i].begin(); it != hashTable[i].end(); it++)
            {
                std::cout << (*it)->word << " -> "; 
            }
        }
        std::cout << std::endl;
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
            std::cout << word << std::endl; 
        }
    }
}