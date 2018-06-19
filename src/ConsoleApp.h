/*
    ConsoleApp
    A graphical interface implemented with the ncurses library.
*/

#pragma once
#include "SentimentAnalyzer.h"

class SentimentAnalyzer;
class ConsoleApp
{

public:
    int init();
    ConsoleApp();
    ~ConsoleApp();

    /* checks whether a file exists */
    static bool FileExists(const char* path);

    /* print to screen */
    void PrintString(const char* str, int y, int x, int pair);
    void UpdateProgress(int y, int x, int progress);

private:
    SentimentAnalyzer *controller;
    bool running;
    bool goodToGo;

    /* path to dataset containing classified comments */
    std::string pathData;
    /* path to stopwords */
    std::string pathSW;

    /* Menu screens */
    void MainMenu();
    void LoadDataMenu();
    void WordClassificationMenu();
    void WordOcurrencesMenu();
    void ClassifyCommentMenu();
    void PreffixSearchMenu();
    void RankingMenu();
    void KaggleMenu();
    void Error(const char* errorMessage);
    void PressAnyKey(int y, int x);
    void StatusMessage(const char* message);
    void Reader(std::list<std::string> text);

    /* user input with cursor at (x,y) */
    std::string UserInput(int y, int x);

    /* user integer selection at (x, y) */
    /* lower and upper bounds are inclusive */
    int UserIntegerSelect(int y, int x, int lowerBound, int upperBound);

    int MoveCursor(int y, int x, int options, int yInit);
    void LoadFile(const char* path);

    /* prints the inverted file of a word into a file */
    void DumpInvertedFile(std::ofstream& of, const WordEntry* entry);

    /* an interface for the kaggle challenge
       input: kaggle input file */
    void KaggleChallenge(std::string path, int method);

};