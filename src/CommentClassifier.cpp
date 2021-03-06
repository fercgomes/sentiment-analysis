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
            while(fp >> word)
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

double CommentClassifier::GetScore(std::string comment, int method)
{
    switch(method)
    {
        case 0:
            return RegularMean(comment);

        case 1:
            return QuantityBasedMean(comment);
        
        case 2:
            return FilteredMean(comment);
        
        default:
            throw "No function implemented.";

    }
}

double CommentClassifier::RegularMean(std::string comment)
{
    double total = 0, average = 0, wordScore, wordCount = 0;
    WordEntry* wEntry;
    std::string word;

    std::stringstream ss(comment);
    while(ss.tellg() != -1)
    {
        word.clear();
        ss >> word;
        wEntry = wordEntries->search(word);
        if(wEntry != nullptr)
        {
            /* word has a score */
            wordScore = wEntry->averageScore;

            total += wordScore;
            wordCount += 1;
        }
    }

    if(total)
    {
        average = total / wordCount;
    }

    return average;
}

double CommentClassifier::QuantityBasedMean(std::string comment)
{
    constexpr double baseWeight = 1;
    double accumulatedWeight = 0;
    int inversionsCount = 0; 
    double totalScore = 0;
    double totalWords = 0;
    double resultingScore = 0;
    std::string word;

    std::ofstream lg;

    std::stringstream ss(comment);
    while(ss.tellg() != -1)
    {
        word.clear();
        ss >> word;

        /* check if it's a modifier */
        if(IsModifier(word))
        {
            accumulatedWeight += modifiers[word];
        }
        /* check if it's a inverter */
        else if(IsInverter(word))
        {
            inversionsCount++;
        }
        /* tries to get a valid score for word */
        else
        {
            WordEntry* wEntry = wordEntries->search(word);
            /* there's a valid entry */
            if(wEntry != nullptr)
            {
                double totalWeight;
                double newScore;

                /* odd number of inversions, and no accumulated weight = invert the polarity around the center */
                if(inversionsCount % 2 != 0 && !accumulatedWeight)
                {
                    newScore = 2.0f - std::abs(2.0f - wEntry->averageScore);
                }
                else
                {
                    totalWeight = baseWeight + std::pow(-1, inversionsCount) * accumulatedWeight * 1.3f;
                    newScore = totalWeight * wEntry->averageScore;
                }

                totalScore += newScore;
                totalWords += 1; 
            }

            /* resets the variables */
            accumulatedWeight = 0;
            inversionsCount = 0;
        }
    }

    if(totalScore && totalWords)
    {
        resultingScore = totalScore / totalWords;
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
        return 0;
    }
}

double CommentClassifier::FilteredMean(std::string comment)
{
    constexpr double maxStdDev = 0.9f;
    constexpr int minOcur = 5;

    double total = 0, average = 0, wordScore, wordCount = 0;
    WordEntry* wEntry;
    std::string word;

    std::stringstream ss(comment);
    while(ss.tellg() != -1)
    {
        word.clear();
        ss >> word;

        wEntry = wordEntries->search(word);
        if(wEntry != nullptr && wEntry->count > minOcur && wEntry->standardDeviation < maxStdDev)
        {
            /* word has a consistent rating, make it worth more */
            wordScore = wEntry->averageScore * 7.0f;

            total += wordScore;
            wordCount += 7;
        }
        else if(wEntry != nullptr)
        {
            /* use regular score */
            wordScore = wEntry->averageScore;

            total += wordScore;
            wordCount += 1;
        }
        else
        {
            /* no entry, make it neutral */
            wordScore = 2.0f;

            total += wordScore;
            wordCount += 1;
        }
    }

    if(total)
    {
        average = total / wordCount;
    }

    return average;
}