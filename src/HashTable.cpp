/*
    Hash Table implementation
*/

#include "HashTable.h"

HashTable::HashTable()
{
}

HashTable::HashTable(std::size_t elements)
{
}

HashTable::~HashTable(){
}

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
    while (!isPrime(++a)) 
    { }
    return a;
}

/* expands the hash table by a constant factor */
void HashTable::rehash()
{
    std::size_t newSize = nextPrime(std::ceil(tableSize * REHASH_FACTOR));
    std::list<std::string> *newHashTable = new std::list<std::string> [newSize];
    std::list<std::string>::iterator it;

    collisions = 0;

    /* iterates over the elements in the old hash table */
    for(int i = 0; i < tableSize; i++)
    {
        if(!hashTable[i].empty())
        {
            for(it = hashTable[i].begin(); it != hashTable[i].end(); it++)
            {
                /* pushed old elements into the new hash table */
                auto key = hash(stringToInteger(*it), newSize);
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
    
    rehashCount++;
}

/* returns the ratio (occupied slots) / (total slots) */
float HashTable::occupationRatio()
{
    float occupiedSlots = 0;
    for(int i = 0; i < tableSize; i++)
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
    else if(listLimitFlag) return true;
    else return false;
}

/* converts a string into a 32-bit integer */
unsigned long long HashTable::stringToInteger(std::string name)
{
    unsigned long long stringKey = 0,
                  characterInt;

    std::string::iterator character;   
    int i = 0;
    for(character = name.end(); character != name.begin(); character--){
        characterInt = (unsigned long long) (*character);
        stringKey = stringKey + (characterInt * (128 * i));
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
void HashTable::push(std::string name)
{
    if(name.empty()){
        std::cerr << "ERROR: Can't insert an empty string into the Hash Table." << std::endl;
    } else {
        unsigned long key = hash(stringToInteger(name));
    
        /* check if there is a collision */
        if(!hashTable[key].empty())
            collisions++;
            
        hashTable[key].emplace_front(name);
        if(hashTable[key].size() > MAX_CHAINED_OCCUPANCY) listLimitFlag = true;

        if(shouldExpand()){
            rehash(); 
            std::cout << "Hash Table will be expanded!" << std::endl;
        }
    }
}

/* returns a name from the hash table */
/* returns an empty string if it doesn't exist */
std::string HashTable::search(std::string name)
{
    if(name.empty())
    {
        std::cerr << "ERROR: Can't search for an empty string" << std::endl;
        return "";
    }
    else
    {
        unsigned long key = hash(stringToInteger(name));
        std::list<std::string>::iterator list_lookup;

        if(hashTable[key].empty()){
            std::cerr << "ERROR: " << name << " was not found in the Hash Table." << std::endl;
        }
        else
        {
            list_lookup = std::find(hashTable[key].begin(), hashTable[key].end(), name);

            /* check if element was found */
            if(list_lookup != hashTable[key].end())
            {
                return (*list_lookup);
            }
            else
            {
                std::cerr << "ERROR: " << name << " was not found in the Hash table." << std::endl;
            }
        }
    }
}

/* returns a name from the hash table with the operator [] */
std::string HashTable::operator[](std::string name)
{
    if(name.empty()
    {
        std::cerr << "ERROR: Can't search for an empty string" << std::endl;
        return "";
    }
    else
    {
        unsigned long key = hash(stringToInteger(name));
        std::list<std::string>::iterator list_lookup;

        if(hashTable[key].empty())
        {
            std::cerr << "ERROR: " << name << " was not found in the Hash Table." << std::endl;
        }
        else
        {
            list_lookup = std::find(hashTable[key].begin(), hashTable[key].end(), name);

            /* check if element was found */
            if(list_lookup != hashTable[key].end())
            {
                return (*list_lookup);
            }
            else
            {
                std::cerr << "ERROR: " << name << " was not found in the Hash table." << std::endl;
                return "";
            }
        }
    }
}

void HashTable::printReport()
{
    std::cout << "====================================" << std::endl;
    std::cout << "Details about the Hash Table: " << std::endl;
    std::cout << "Table size:\t" << tableSize << std::endl;
    std::cout << "Occupancy rate:\t" << occupationRatio() << std::endl;
    std::cout << "Collisions:\t" << collisions << std::endl;
    std::cout << "Rehash's:\t" << rehashCount << std::endl;
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
    for(int i = 0; i < tableSize; i++)
    {
        std::cout << i << ":\t";
        if(hashTable[i].empty()) std::cout << "NULL";
        else
        {
            std::list<std::string>::iterator it;
            for(it = hashTable[i].begin(); it != hashTable[i].end(); it++)
            {
                std::cout << (*it) << " -> "; 
            }
        }
        std::cout << std::endl;
    }

    printReport();
}
