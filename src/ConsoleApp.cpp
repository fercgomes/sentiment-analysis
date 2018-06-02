#include "ConsoleApp.h"
#include <curses.h>

#define MAIN_MENU_ANCHOR_POINT_X 4
#define MAIN_MENU_ANCHOR_POINT_Y 2

ConsoleApp::ConsoleApp()
{
    running = true;
    goodToGo = false;
}

int ConsoleApp::init()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_RED);

    /* Main Loop */
    while(running)
    {
        MainMenu();

        refresh();
    }

    endwin();
    return 0;
}

void ConsoleApp::MainMenu()
{
    int opt;
    clear();
    PrintString("Sentiment Analyzer | STATUS: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 1);
    PrintString(" Load/Reload Data", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Get Word Classification", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Get Word Ocurrences", MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Classify Comment", MAIN_MENU_ANCHOR_POINT_Y + 5, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Preffix Search", MAIN_MENU_ANCHOR_POINT_Y + 6, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Ranking", MAIN_MENU_ANCHOR_POINT_Y + 7, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Exit", MAIN_MENU_ANCHOR_POINT_Y + 8, MAIN_MENU_ANCHOR_POINT_X, 2);

    if(goodToGo)
    {
        PrintString("Data loaded", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 29, 3);
    }
    else
    {
        PrintString("No data loaded", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 29, 4);
    }

    opt = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 7, 0);
    switch(opt)
    {
        case 0:
            LoadDataMenu();
            break;

        /* word classification */
        case 1:
            if(goodToGo)
            {
                WordClassificationMenu();
            }
            else
            {
                Error("No data loaded.");
            }
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        case 6:
            running = false;
            break;
    }
}

void ConsoleApp::LoadDataMenu()
{
    bool menuActive = true;
    int opt = 0;

    do
    {
        clear();
        PrintString("Import Data", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 1);
        PrintString(" Convert words to lowercase: ", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Filter non-alphabetical words: ", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Path to stopwords: ", MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Path to file: ", MAIN_MENU_ANCHOR_POINT_Y + 5, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Load", MAIN_MENU_ANCHOR_POINT_Y + 6, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Back", MAIN_MENU_ANCHOR_POINT_Y + 7, MAIN_MENU_ANCHOR_POINT_X, 2);

        /* print flags */
        if(controller.convertLowerCase)
            PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 4);
        else
            PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 3);

        if(controller.filterNonAlpha)
            PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 32, 4);
        else
            PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 32, 3);


        PrintString(pathSW.c_str(), MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X + 20, 3);
        PrintString(pathData.c_str(), MAIN_MENU_ANCHOR_POINT_Y + 5, MAIN_MENU_ANCHOR_POINT_X + 15, 3);
        opt = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 6, opt);
        switch(opt)
        {
            /* convert lowercase */
            case 0:
                if(controller.convertLowerCase)
                {
                    PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 4);
                    controller.convertLowerCase = false;
                }
                else
                {
                    PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 3);
                    controller.convertLowerCase = true;
                }
                break;

            /* filter non alpha */
            case 1:
                if(controller.filterNonAlpha)
                {
                    PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 32, 4);
                    controller.filterNonAlpha = false;
                }
                else
                {
                    PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 32, 3);
                    controller.filterNonAlpha = true;
                }
                break;

            /* stopwords */
            case 2:
                pathSW = UserInput(MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X + 20);

                /* check if file exists */
                if(!FileExists(pathSW.c_str()))
                {
                   Error("File does not exist!"); 
                   pathSW.clear();
                }

                if(pathSW.empty())
                {
                    controller.removeStopWords = false;
                }
                else
                {
                    controller.removeStopWords = true;
                }
                break;

            /* comments file path */
            case 3:
                pathData = UserInput(MAIN_MENU_ANCHOR_POINT_Y + 5, MAIN_MENU_ANCHOR_POINT_X + 15);

                /* check if file exists */
                if(!FileExists(pathData.c_str()))
                {
                   Error("File does not exist!"); 
                   pathData.clear();
                }

                break;

            /* load */
            case 4:
                /* there's data to be loaded */
                if(!pathData.empty() && !goodToGo)
                {
                    clear();
                    PrintString("Loading data...", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3);

                    /* load stopwords */
                    if(controller.removeStopWords)
                    {
                        /* TODO: get status from function */
                        controller.LoadStopWords(pathSW.c_str());
                    }

                    /* TODO: get status from import function */
                    controller.ImportFile(pathData.c_str());
                    goodToGo = true;

                    StatusMessage("Done loading.");
                }
                else
                {
                    Error("Error loading data.");
                }

                break;

            /* back */
            case 5:
                menuActive = false;
                break;
        }
    }
    while(menuActive);
}

void ConsoleApp::WordClassificationMenu()
{
    std::string word;
    WordEntry *wEntry;

    clear();
    PrintString("Word: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    word = UserInput(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 6);

    wEntry = controller.GetWordEntry(word);
    if(wEntry != nullptr)
    {
        auto score = std::to_string(wEntry->averageScore);
        auto ocurrences = std::to_string(wEntry->count);
        auto stdDev = std::to_string(wEntry->standardDeviation);

        PrintString("Average sentiment score: ", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString("Ocurrences: ", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString("Standard Deviation: ", MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X, 2);

        PrintString(score.c_str(), MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 25, 2);
        PrintString(ocurrences.c_str(), MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 12, 2);
        PrintString(stdDev.c_str(), MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X + 20, 2);

        PressAnyKey(MAIN_MENU_ANCHOR_POINT_Y + 5, MAIN_MENU_ANCHOR_POINT_X);
    }
    else
    {
        Error("Word doesn't exist.");
    }

}

void ConsoleApp::Error(const char* errorMessage)
{
    clear();
    PrintString(errorMessage, MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 4);
    PressAnyKey(MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X);
}

void ConsoleApp::PressAnyKey(int y, int x)
{
    move(y, x);
    PrintString("Press any key to continue...", y, x, 2);
    getch();
}

void ConsoleApp::StatusMessage(const char* message)
{
    clear();
    PrintString(message, MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3);
    PressAnyKey(MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X);
}

std::string ConsoleApp::UserInput(int y, int x)
{
    move(y, x);
    clrtoeol();
    char inputStr[80];

    echo();
    getstr(inputStr);
    noecho();
    std::string str(inputStr);

    return str;
}

void ConsoleApp::PrintString(const char* str, int y, int x, int pair)
{
    move(y, x);
    attron(COLOR_PAIR(pair));
    addstr(str);
    attroff(COLOR_PAIR(pair));
}

int ConsoleApp::MoveCursor(int y, int x, int options, int yInit)
{
    constexpr int cPair = 2;
    int cursor = yInit;
    wchar_t c;
    bool menuActive = true;

    PrintString(">", y + cursor, x, cPair);

    do
    {
        c = wgetch(stdscr);

        if(c == KEY_UP)
        {
            if(cursor > 0)
            {
                PrintString(" ", y + cursor, x, cPair);
                cursor--;
                PrintString(">", y + cursor, x, cPair);
            }
        }
        else if(c == KEY_DOWN)
        {
            if(cursor < options - 1)
            {
                PrintString(" ", y + cursor, x, cPair);
                cursor++;
                PrintString(">", y + cursor, x, cPair);
            }
        }
        /* pressed enter */
        else if(c == 10)
        {
            menuActive = false;
        }
    }
    while(menuActive);

    return cursor;
}

bool ConsoleApp::FileExists(const char* path)
{
    std::ifstream infile(path);
    return infile.good();
}