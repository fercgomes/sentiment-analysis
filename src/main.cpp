/*
    Sentiment Analyser
    Authors:    Fernando Corrêa Gomes
                Carlos Morvan Filho de Paula e Santiago 
    
    UFRGS 2018
*/

#include <iostream>
#include "ConsoleApp.h"

int main(int argc, char **argv)
{
    /* initializes the application */
    // ConsoleApp app;
    // return app.init();

    SentimentAnalyzer a;
    a.ImportFile("/home/grad/fcgomes/Desktop/sentiment-analysis/bin/grande.txt");
    return 0;
}