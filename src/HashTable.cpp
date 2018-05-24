/*
    Hash Table implementation
*/

#include <fstream>
#include "HashTable.h"
#include "ClassificationContainer.h"

/*
    Initialization procedure
    - Set hash table size
    - Allocate empty lists
*/
HashTable::HashTable()
{
    /* allocate with default size */
    hashTable = new std::list<WordContainer*> [HASHTABLE_DEFAULT_SIZE];
    tableSize = HASHTABLE_DEFAULT_SIZE;
    collisions = 0;
    listLimitFlag = false;
    rehashCount = 0;

    std::cout << "HASHTABLE: intializing with default size of " << HASHTABLE_DEFAULT_SIZE << "." << std::endl;
}

HashTable::HashTable(std::size_t elements)
{
    std::size_t calculatedSize = std::ceil(TABLESIZE_FACTOR * elements);
    tableSize = calculatedSize;
    hashTable = new std::list<WordContainer*> [calculatedSize];
    collisions = 0;
    listLimitFlag = false;
    rehashCount = 0;

    std::cout << "HASHTABLE: intializing with given size of " << calculatedSize << "." << std::endl;
}

HashTable::~HashTable()
{
    /* deallocate the list */
    delete[] hashTable;

    std::cout << "HASHTABLE: destroying." << std::endl;
}

/*
    Math related
*/

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
    std::list<WordContainer*> *newHashTable = new std::list<WordContainer*> [newSize];
    std::list<WordContainer*>::iterator it;

    collisions = 0;

    /* iterates over the elements in the old hash table */
    for(int i = 0; i < tableSize; i++)
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
    
    rehashCount++;
}

WordContainer* HashTable::GetContainer(std::list<WordContainer*>& contList, std::string word)
{
    std::list<WordContainer*>::iterator it;
    for(it = contList.begin(); it != contList.end(); it++)
    {
        if((*it)->word == word)
        {
            /* word already exists in the hash table */
            return *it;
        }
    }

    /* word does not exist */
    return nullptr;
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
    else if(listLimitFlag) return false;
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
void HashTable::push(std::string word, float commentScore, std::size_t commentID, std::size_t wordPos)
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
        WordContainer* lookup = GetContainer(hashTable[key], word);
        if(lookup == nullptr)
        {
            /* no entry so far, make a new one */
            lookup = new WordContainer(word);

            /* inserts into hash table */
            hashTable[key].emplace_front(lookup);
        }
        
        /* push new word ocurrence */
        (lookup->classification)->AddOcurrence(commentScore);
        (lookup->invFile)->AddWordOcurrence(commentID, wordPos, commentScore);


        /* checks expansion conditions */
        if(hashTable[key].size() > MAX_CHAINED_OCCUPANCY) listLimitFlag = true;

        if(shouldExpand())
        {
            rehash(); 
            std::cout << "Hash Table will be expanded!" << std::endl;
        }
    }
}

WordContainer* HashTable::search(std::string word)
{
    if(word.empty())
    {
        std::cerr << "HASHTABLE ERROR: Can't search for an empty string" << std::endl;
        return nullptr;
    }
    else
    {
        unsigned long key = hash(stringToInteger(word));

        if(hashTable[key].empty()){
            std::cerr << "HASHTABLE ERROR: " << word << " was not found in the Hash Table." << std::endl;
            return nullptr;
        }
        else
        {
            std::list<WordContainer*>::iterator it;
            for(it = hashTable[key].begin(); it != hashTable[key].end(); it++)
            {
                if((*it)->word == word)
                {
                    return (*it);
                }
            }

            std::cerr << "ERROR: " << word << " was not found in the Hash table." << std::endl;
            return nullptr;
        }
    }
}

/* returns a name from the hash table with the operator [] */
WordContainer* HashTable::operator[](std::string name)
{
}

void HashTable::LoadFromFile(const char* filename)
{
    std::ifstream fp;
    fp.open(filename);
    bool running = true;

    if(fp.is_open())
    {
        std::string line;
        std::string word;
        int commentID = 0;
        std::getline(fp, line);
        while(running)
        {
            int commentScore;
            int wordPos;

            std::size_t begin = 0;
            std::size_t i = 0;

            /* get comment score (first 'word') */
            while(line[i] != ' ') i++;

            std::string aux = line.substr(begin, i);
            if(!aux.empty())
                commentScore = std::stoi(aux);


            i++; /* beginning of next word */
            begin = i;
            while(i < line.size())
            {
                while(line[i] != ' ') i++;

                word = line.substr(begin, i - begin);
                wordPos = begin;

                /* push into hash table */
                push(word, commentScore, commentID, wordPos);
                std::cout << "Inserting " << word << std::endl;

                i++; /* beginning of next word */
                begin = i;
            }

            if(fp.eof()) running = false;
            else
            {
                std::getline(fp, line);
                commentID++;
            }
        }

        fp.close();
    }
}

void HashTable::printReport()
{
    std::cout << "====================================" << std::endl;
    std::cout << "Details about the Hash Table: " << std::endl; std::cout << "Table size:\t" << tableSize << std::endl;
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
            std::list<WordContainer*>::iterator it;
            for(it = hashTable[i].begin(); it != hashTable[i].end(); it++)
            {
                std::cout << (*it)->word << " -> "; 
            }
        }
        std::cout << std::endl;
    }

    printReport();
}
