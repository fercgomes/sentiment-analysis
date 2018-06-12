#include "CommentClassifier.h"
#include "ConsoleApp.h"
#include "WordEntry.h"
#include <fstream>
#include <algorithm>
#include <sstream>

/* public functions */

CommentClassifier::CommentClassifier(HashTable* we) :
    wordEntries(we)
{
    if(!LoadWeights("modifiers.txt", "inverters.txt"))
    {
        throw "Couldn't find the classifiers files modifiers.txt and inverters.txt";
    }
}

bool CommentClassifier::LoadWeights(const char* modPath, const char* invPath)
{
    std::ifstream fp;
    
    /* loads modifiers */
    fp.open(modPath);
    if(ConsoleApp::FileExists(modPath))
    {
        std::string word;
        double weight;
        while(fp >> word >> weight)
        {
            modifiers[word] = weight;
        }
        /* done loading modifiers */
        fp.close();

        fp.open(invPath);
        if(ConsoleApp::FileExists(invPath))
        {
            while(fp >> word >> weight)
            {
                inverters.emplace_back(word);
            }

            /* all good */
            return true;
        }
        else
        {
            /* no file */
            return false;
        }
    }
    else
    {
        /* no file */
        return false;
    }
}

bool CommentClassifier::IsModifier(std::string word)
{
    if(modifiers.find(word) != modifiers.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CommentClassifier::IsInverter(std::string word)
{
    if(find(inverters.begin(), inverters.end(), word) != inverters.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

double CommentClassifier::GetScore(std::string comment)
{
    constexpr double baseWeight = 1;
    double accumulatedWeight = 0;
    double inversionsCount = 0; 
    double totalScore = 0;
    double totalWords = 0;
    double resultingScore = 0;
    std::string word;

    std::ofstream lg;
    lg.open("log.txt");

    std::stringstream ss(comment);
    while(ss.tellg() != -1)
    {
        word.clear();
        ss >> word;
        lg << "word " << word << std::endl;

        /* check if it's a modifier */
        if(IsModifier(word))
        {
            accumulatedWeight += modifiers[word];
            lg << "weight + " << modifiers[word] << std::endl;
        }
        /* check if it's a inverter */
        else if(IsInverter(word))
        {
            inversionsCount++;
            lg << "inv + " << std::endl;
        }
        /* tries to get a valid score for word */
        else
        {
            WordEntry* wEntry = wordEntries->search(word);
            /* there's a valid entry */
            if(wEntry != nullptr)
            {
                double totalWeight = baseWeight + std::pow(-1, inversionsCount) * accumulatedWeight;
                totalScore += totalWeight * wEntry->averageScore;
                totalWords += 1; 
                lg << "ac weight = " << accumulatedWeight << std::endl;
                lg << "inv = " << inversionsCount << std::endl;
                lg << std::endl;
            }

            /* resets the variables */
            accumulatedWeight = 0;
            inversionsCount = 0;
        }
    }
    lg << "total score = " << totalScore << std::endl;
    lg << "total words = " << totalWords << std::endl;

    if(totalScore && totalWords)
    {
        resultingScore = totalScore / totalWords;
        lg << "result = " << resultingScore << std::endl;
        lg.close();
        if(resultingScore <= 4)
        {
            return resultingScore;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        lg.close();
        return 0;
    }
}