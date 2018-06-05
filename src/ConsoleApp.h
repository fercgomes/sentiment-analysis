#pragma once
#include "SentimentAnalyzer.h"

class ConsoleApp
{

public:
    int init();
    ConsoleApp();

private:
    SentimentAnalyzer controller;
    bool running;
    bool goodToGo;

    std::string pathData;
    std::string pathSW;

    /* Menu */
    void MainMenu();
    void LoadDataMenu();
    void WordClassificationMenu();
    void WordOcurrencesMenu();
    void ClassifyCommentMenu();
    void PreffixSearchMenu();
    void RankingMenu();
    void Error(const char* errorMessage);
    void PressAnyKey(int y, int x);
    void StatusMessage(const char* message);
    void Reader(std::list<std::string> text);

    /* user input with cursor at (x,y) */
    std::string UserInput(int y, int x);

    /* print to screen */
    /* TODO: add colors */
    void PrintString(const char* str, int y, int x, int pair);
    int MoveCursor(int y, int x, int options, int yInit);

    bool FileExists(const char* path);

    void DumpInvertedFile(std::ofstream& of, const WordEntry* entry);

};