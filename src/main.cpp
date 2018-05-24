/*
    Sentiment Analyser
    Authors:    Fernando Corrêa Gomes
                Carlos Morvan Filho de Paula e Santiago 
    
    UFRGS 2018
*/

#include <iostream>
#include "HashTable.h"
#include "ClassificationContainer.h"
#include "InvertedFile.h"
#include "WordClassification.h"

int main()
{
    HashTable test(1000);

    test.LoadFromFile("test.txt");
    test.printHashTable();
    

    return 0;
}