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
    teste.LoadStopWords("/home/fernando/Desktop/trabalho-cpd/bin/stopwords.txt");
    teste.ImportFile("/home/fernando/Desktop/trabalho-cpd/bin/grande.txt");
    std::cout << "Done importing." << std::endl;
    // teste.PrintWords();

    std::string comment;
    std::cin >> comment;
    // std::getline(std::cin, comment);
    while(!comment.empty())
    {
        // std::vector<std::string> pref = teste.preffixes->getSuffixes(comment);
        // for(std::size_t i = 0; i < pref.size(); i++)
        // {
        //     std::cout << pref[i] << std::endl;
        // }

        auto res = teste.GetWordEntry(comment);
        if(res != nullptr)
            std::cout << res << std::endl;

        std::cin >> comment;
        // std::getline(std::cin, comment);
    }
    
    return 0;
}