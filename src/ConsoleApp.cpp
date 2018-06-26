#include "ConsoleApp.h"
#include "Ranking.h"
#include <curses.h>
#include <ostream>
#include <list>
#include <string>
#include <sstream>
#include <iomanip>

#define MAIN_MENU_ANCHOR_POINT_X 4
#define MAIN_MENU_ANCHOR_POINT_Y 2

ConsoleApp::ConsoleApp()
{
    running = true;
    goodToGo = false;
    controller = new SentimentAnalyzer(this);
}

ConsoleApp::~ConsoleApp()
{
    delete controller;
}

/* the interface's main loop */
int ConsoleApp::init()
{
    /* ncurses initialization */
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

/*
    Most of the options in the main menu can only be accessed
    if the data has been loaded.
*/
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
    PrintString(" Kaggle Challenge", MAIN_MENU_ANCHOR_POINT_Y + 8, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Exit", MAIN_MENU_ANCHOR_POINT_Y + 9, MAIN_MENU_ANCHOR_POINT_X, 2);

    if(goodToGo)
    {
        PrintString("Data loaded", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 29, 3);
    }
    else
    {
        PrintString("No data loaded", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 29, 4);
    }

    opt = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 8, 0);
    switch(opt)
    {
        /* load data */
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

        /* word ocurrences */
        case 2:
            if(goodToGo)
            {
                WordOcurrencesMenu();
            }
            else
            {
                Error("No data loaded.");
            }
            break;

        /* classify comment */
        case 3:
            if(goodToGo)
            {
                ClassifyCommentMenu();
            }
            else
            {
                Error("No data loaded.");
            }
            break;

        /* preffix search */
        case 4:
            if(goodToGo)
            {
                PreffixSearchMenu();
            }
            else
            {
                Error("No data loaded.");
            }
            break;

        /* ranking menu */
        case 5:
            if(goodToGo)
            {
                RankingMenu();
            }
            else
            {
                Error("No data loaded.");
            }
            break;

        /* kaggle challenged */
        case 6:
            if(goodToGo)
            {
                KaggleMenu();
            }
            else
            {
                Error("No data loaded.");
            }
            break;

        case 7:
            running = false;
            break;
    }
}

/* loads the files and sets the flags */
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
        if(!controller->convertLowerCase)
            PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 4);
        else
            PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 3);

        if(!controller->filterNonAlpha)
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
                if(controller->convertLowerCase)
                {
                    PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 4);
                    controller->convertLowerCase = false;
                }
                else
                {
                    PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X + 30, 3);
                    controller->convertLowerCase = true;
                }
                break;

            /* filter non alpha */
            case 1:
                if(controller->filterNonAlpha)
                {
                    PrintString("false", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 32, 4);
                    controller->filterNonAlpha = false;
                }
                else
                {
                    PrintString("true ", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 32, 3);
                    controller->filterNonAlpha = true;
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
                    controller->removeStopWords = false;
                }
                else
                {
                    controller->removeStopWords = true;
                }
                break;

            /* comments file path */
            case 3:
                pathData = UserInput(MAIN_MENU_ANCHOR_POINT_Y + 5, MAIN_MENU_ANCHOR_POINT_X + 15);
                // pathData = "/home/grad/fcgomes/Desktop/sentiment-analyzer/bin/grande.txt";
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
                    /* load stopwords */
                    if(controller->removeStopWords)
                    {
                        controller->LoadStopWords(pathSW.c_str());
                    }

                    LoadFile(pathData.c_str());
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

/* asks user for a word and returns the current classification */
void ConsoleApp::WordClassificationMenu()
{
    std::string word;
    WordEntry *wEntry;

    clear();
    PrintString("Word: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    word = UserInput(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 6);

    wEntry = controller->GetWordEntry(word);
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

/* asks user for a word and prints the inverted file in a file */
void ConsoleApp::WordOcurrencesMenu()
{
    bool menuActive = true;
    std::string word;
    std::string fileName;
    std::ofstream fp;

    do 
    {
        clear();
        PrintString(" Word: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
        word = UserInput(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 7);
        
        /* check word exists */
        WordEntry *wEntry = controller->GetWordEntry(word);
        if(wEntry != nullptr)
        {
            /* asks for filer method */
            PrintString(" All ocurrences", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);
            PrintString(" Positive polarity", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X, 2);
            PrintString(" Negative polarity", MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X, 2);
            int polarityFilter = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 3, 0); 

            /* converts to the readable format */
            auto invertedFile = wEntry->GetInvertFile(polarityFilter);
            if(invertedFile.empty())
            {
                Error("No ocurrences found");
            }
            else
            {
                std::list<std::string> text;

                std::list<CommentEntry>::iterator it;
                for(it = invertedFile.begin(); it != invertedFile.end(); it++)
                {
                    std::string header = "Comment ID: " + std::to_string(it->commentID) + ", Sentiment: " + std::to_string(it->commentScore) + "\n";
                    text.emplace_back(header);

                    std::list<std::size_t>::iterator offset;
                    for(auto& offset : it->ocurrences)
                    {
                        text.emplace_back("Offset #" + std::to_string(offset) + "\n");
                    }

                    text.emplace_back("---------------------------------");
                }

                Reader(text);
                menuActive = false;
            }
        }
        else
        {
            Error("Word doesn't exist.");
            menuActive = false;
        }
    }
    while(menuActive);
}

/* asks user for a string, and classifies it using the data loaded */
void ConsoleApp::ClassifyCommentMenu()
{
    int opt = 0;
    int method = 0;
    std::string comment;
    std::string classification;
    std::string path;
    std::ofstream fp;

    /* select classification method */
    clear();
    PrintString(" Regular mean", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Quantity saturated mean", MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Filtered mean", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);
    method = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3, opt);

    clear();
    PrintString(" Load comment from standard input", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Load comments from file", MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" Back", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);

    opt = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3, opt);
    switch(opt)
    {
        /* load from stdin */
        case 0:
            PrintString("Comment:", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X, 2);

            comment = UserInput(MAIN_MENU_ANCHOR_POINT_Y + 4, MAIN_MENU_ANCHOR_POINT_X);
            classification = std::to_string(controller->GetCommentScore(comment, method));

            PrintString("Sentiment: ", MAIN_MENU_ANCHOR_POINT_Y + 6, MAIN_MENU_ANCHOR_POINT_X, 2);
            PrintString(classification.c_str(), MAIN_MENU_ANCHOR_POINT_Y + 6, MAIN_MENU_ANCHOR_POINT_X + 11, 1);
            PressAnyKey(MAIN_MENU_ANCHOR_POINT_Y + 7, MAIN_MENU_ANCHOR_POINT_X);
            break;

        /* load from file */
        case 1:
            PrintString("Path: ", MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X, 2);
            path = UserInput(MAIN_MENU_ANCHOR_POINT_Y + 3, MAIN_MENU_ANCHOR_POINT_X + 6);
            
            if(path.empty())
            {
                Error("No path given.");
            }
            else if(!FileExists(path.c_str()))
            {
                return Error("File doesn't exist.");
            }
            else
            {
                controller->GetCommentFileScore(path.c_str(), "comment-output.txt", method);
                StatusMessage("Done.");
            } 
            break;

        /* back */
        case 2:
            break;
    }
}

/* asks user for a preffix and shows the corresponding words */
void ConsoleApp::PreffixSearchMenu()
{
    std::string pref;
    std::list<std::string> prefList;

    clear();
    PrintString("Preffix: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    pref = UserInput(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 9);
    prefList = controller->GetPreffixes(pref);

    if(prefList.empty())
    {
        Error("No words found.");
    }
    else
    {
        Reader(prefList);
    }
}

void ConsoleApp::RankingMenu()
{
    constexpr int maxSizeLowerBound = 1;
    constexpr int maxSizeUpperBound = 1000;
    constexpr int minOcurrencesLowerBound = 1;
    constexpr int minOcurrencesUpperBound = 1000;

    int opt = 0;
    int rankPositions;
    int rankFilter;

    std::vector<WordEntry*> rankedList;

    clear();
    PrintString("Ranking size: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    rankPositions = UserIntegerSelect(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 15, maxSizeLowerBound, maxSizeUpperBound);

    clear();
    PrintString("Ranking ocurrence filter: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    rankFilter = UserIntegerSelect(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 26, minOcurrencesLowerBound, minOcurrencesUpperBound);

    /* builds ranking */
    controller->ranking.SetOptions(rankFilter, rankPositions);
    controller->ranking.LoadRank(*(controller->wordEntries));

    clear();
    PrintString(" By positive polarity", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" By negative polarity", MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X, 2);
    PrintString(" By ocurrences", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);

    opt = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3, opt);
    switch(opt)
    {
        case 0:
            rankedList = controller->GetBestRank();
            break;

        case 1:
            rankedList = controller->GetWorstRank();
            break;

        case 2:
            rankedList = controller->GetOcurrencesRank();
            break;
    }

    std::vector<WordEntry*>::iterator it;
    std::list<std::string> text;

    int position = 0;
    for(it = rankedList.begin(); it != rankedList.end() && position < rankPositions; it++)
    {
        std::stringstream ss;

        ss << std::setw(3) << position + 1
           << std::setw(25) << (*it)->word
           << std::setw(10) << " Score: "
           << std::setw(6) << (*it)->averageScore
           << std::setw(10) << " Count: "
           << std::setw(5) << (*it)->count;

        text.push_back(ss.str());
        position++;
    }

    Reader(text);
}

/* asks the user for the kaggle input data, and creates a submission file */
void ConsoleApp::KaggleMenu()
{
    clear();
    PrintString("Path to dataset: ", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
    std::string path = UserInput(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X + 17); 
    if(path.empty())
    {
        Error("No file given.");
    }
    else if(!FileExists(path.c_str()))
    {
        Error("File doesn't exist.");
    }
    else
    {

        /* select classification method */
        clear();
        PrintString(" Regular mean", MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Quantity saturated mean", MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X, 2);
        PrintString(" Filtered mean", MAIN_MENU_ANCHOR_POINT_Y + 2, MAIN_MENU_ANCHOR_POINT_X, 2);
        int method = MoveCursor(MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3, 0);

        KaggleChallenge(path, method);
        StatusMessage("Done.");
    }
}

/* shows a error message and waits for input */
void ConsoleApp::Error(const char* errorMessage)
{
    clear();
    PrintString(errorMessage, MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 4);
    PressAnyKey(MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X);
}

/* waits for user interaction */
void ConsoleApp::PressAnyKey(int y, int x)
{
    move(y, x);
    PrintString("Press any key to continue...", y, x, 2);
    getch();
}

/* shows a status message and waits user */
void ConsoleApp::StatusMessage(const char* message)
{
    clear();
    PrintString(message, MAIN_MENU_ANCHOR_POINT_Y, MAIN_MENU_ANCHOR_POINT_X, 3);
    PressAnyKey(MAIN_MENU_ANCHOR_POINT_Y + 1, MAIN_MENU_ANCHOR_POINT_X);
}

/* shows a on-screen reader */
void ConsoleApp::Reader(std::list<std::string> text)
{
    constexpr int onScreen = 20;
    
    bool topEnd, bottomEnd, active;
    std::list<std::string>::iterator top, bottom, it;

    top = bottom = text.begin();
    topEnd = true;
    bottomEnd = false;

    int i = 0;
    while(bottom != text.end() && i < onScreen)
    {
        bottom++;
        i++;
    }

    active = true;
    do
    {
        clear();
        /* print current lines */
        i = 0;
        for(it = top; it != bottom; it++)
        {
            PrintString((*it).c_str(), i + 1, MAIN_MENU_ANCHOR_POINT_X, 2);
            i++;
        } 

        /* wait input */
        wchar_t op = wgetch(stdscr);
        switch(op)
        {
            case KEY_UP:
                if(!topEnd)
                {
                    bottom = top;
                    i = 0;
                    while(top != text.begin() && i < onScreen)
                    {
                        top--;
                        i++;
                    }
                    if(top == text.begin()) topEnd = true;
                    bottomEnd = false;
                }
                break;

            case KEY_DOWN:
                if(!bottomEnd)
                {
                    top = bottom;
                    i = 0;
                    while(bottom != text.end() && i < onScreen)
                    {
                        bottom++;
                        i++;
                    }
                    if(bottom == text.end()) bottomEnd = true; 
                    topEnd = false;

                }
                break;

            case 10:
                active = false;
                break;
        }

    }
    while(active);
}

/* gets a input string from user */
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

/* gets an integer from user */
int ConsoleApp::UserIntegerSelect(int y, int x, int lowerBound, int upperBound)
{
    bool selectionActive = true;
    int currentValue = lowerBound;
    wchar_t option;

    while(selectionActive)
    {
        move(y, x);
        clrtoeol();
        PrintString(std::to_string(currentValue).c_str(), y, x, 3);

        option = wgetch(stdscr);
        switch(option)
        {
            case KEY_UP:
                if(currentValue < upperBound)
                {
                    currentValue++;
                }
                break;

            case KEY_DOWN:
                if(currentValue > lowerBound)
                {
                    currentValue--;
                }
                break;
            
            case 10:
                selectionActive = false;
                break;
        }
    }

    return currentValue;
}

/* prints a string to the console at (x, y) */
void ConsoleApp::PrintString(const char* str, int y, int x, int pair)
{
    move(y, x);
    attron(COLOR_PAIR(pair));
    addstr(str);
    attroff(COLOR_PAIR(pair));
}

/* interaction loop for the menu cursor */
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

/* updates the progress */
void ConsoleApp::UpdateProgress(int y, int x, int done)
{
    auto progress = std::to_string(done);
    auto str = (progress + "%").c_str();

    PrintString("Progress: ", 0, 0, 1);
    PrintString(str, 0, 10, 3);

    refresh();
}

void ConsoleApp::LoadFile(const char* path)
{
    controller->ImportFile(path);
}

/* check wheter a file exists */
bool ConsoleApp::FileExists(const char* path)
{
    std::ifstream infile(path);
    return infile.good();
}

/* parses a kaggle input file, and classifies the comments */
void ConsoleApp::KaggleChallenge(std::string path, int method)
{
    std::ifstream inFile;
    std::ofstream outFile;

    std::string phraseIDstr;
    std::string sentenceIDstr;
    std::string comment;
    unsigned int phraseID, sentenceID;

    std::string outPath = "submission.csv";

    inFile.open(path);
    outFile.open(outPath);

    /* ignores header line */
    std::getline(inFile, comment);
    /* sets header line for output */
    outFile << "PhraseId,Sentiment" << std::endl;

    bool active = true;
    while(active)
    {
        std::getline(inFile, phraseIDstr, '\t');
        if(inFile.eof())
        {
            active = false;
        }
        else
        {
            phraseID = std::stoi(phraseIDstr);
            std::getline(inFile, sentenceIDstr, '\t');
            sentenceID = std::stoi(sentenceIDstr);
            std::getline(inFile, comment);

            auto score = std::round(controller->GetCommentScore(comment, method));
            outFile << phraseID << "," << score << std::endl;
        }
    }

    inFile.close();
    outFile.close();
}