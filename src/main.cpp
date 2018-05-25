/*
    Sentiment Analyser
    Authors:    Fernando Corrêa Gomes
                Carlos Morvan Filho de Paula e Santiago 
    
    UFRGS 2018
*/

#include <iostream>
#include "SentimentAnalyzer.h"

int main()
{
    SentimentAnalyzer teste;
    teste.ImportFile("grande.txt");
    std::cout << "Done importing." << std::endl;

    //teste.PrintWords();

    std::string comment;
    std::getline(std::cin, comment);
    while(!comment.empty())
    {
        auto res = teste.GetCommentScore(comment);
        std::cout << "Resulting score: " << res << std::endl;
        std::getline(std::cin, comment);
    }
    
    return 0;
}