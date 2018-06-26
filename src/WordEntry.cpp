#include <iostream>
#include <cmath>
#include <algorithm>
#include "WordEntry.h"

extern std::ofstream logger;

/*
    CommentEntry class
*/

CommentEntry::CommentEntry(int id, int offset, double score)
{
   commentID = id;
   commentScore = score;

   ocurrences.push_back(offset); 
}

std::size_t CommentEntry::count()
{
    return ocurrences.size();
}

/*
    WordEntry class
*/

WordEntry::WordEntry(std::string wd)
{
    word = wd;
    averageScore = 0.0f;
    count = 0;
    standardDeviation = 0.0f;
    currentDocument = invertedFile.begin();
    empty = true;
}

void WordEntry::AddOcurrence(int id, int offset, double score)
{
    /* sets new average */
    averageScore = (count * averageScore + score) / (count + 1);
    count++;

    /* check if strucutre hasn't been used */
    if(empty)
    {
        empty = false;

        /* create new comment entry */
        invertedFile.emplace_back(id, offset, score);
    }
    /* check if it is inserting in current document */
    else
    {
        /* done with last document, start next one */
        if(currentDocument->commentID != id)
        {
            invertedFile.emplace_back(id, offset, score);
            currentDocument++;
        }
        /* still in the same document, insert word's offset */
        else
        {
            currentDocument->ocurrences.push_back(offset);
        }
    }

    UpdateSandardDeviation();
}

void WordEntry::UpdateSandardDeviation()
{
    double sum = 0;

    if(invertedFile.size() > 1)
    {
        /* iterate over each individual score (comment) */
        std::list<CommentEntry>::iterator it;
        for(it = invertedFile.begin(); it != invertedFile.end(); it++)
        {
            std::size_t wordCount = (*it).count();
            double commentScore = (*it).commentScore;

            double difference = commentScore - averageScore;
            sum += wordCount * std::pow(difference, 2);
        }

        standardDeviation = std::sqrt( sum / (count - 1) );
    }
    else
    {
        standardDeviation = 0.0f;
    }
}

void WordEntry::PrintOcurrences()
{
    std::list<CommentEntry>::iterator comment;
    std::cout << "Printing ocurrences of word " << word << "." << std::endl;
    for(comment = invertedFile.begin(); comment != invertedFile.end(); comment++)
    {
        std::cout << "=====================================" << std::endl;
        std::cout << "Comment ID: " << (*comment).commentID << std::endl;
        std::cout << "Comment score: " << (*comment).commentScore << std::endl;

        std::list<std::size_t>::iterator word;
        for(word = ((*comment).ocurrences).begin(); word != ((*comment).ocurrences).end(); word++)
        {
            std::cout << (*word) << ", ";
        } 

        std::cout << std::endl;
    }
}

std::list<CommentEntry> WordEntry::GetInvertFile(int filter)
{
    std::list<CommentEntry> filteredInvertedFile;
    switch(filter)
    {
        /* no filter */
        case 0:
            return invertedFile;

        /* positive polarity */
        case 1:
            std::copy_if(invertedFile.begin(), invertedFile.end(), std::back_inserter(filteredInvertedFile), [&filter](CommentEntry ce){
                return ce.commentScore > 2;
            });

            break;

        /* negative polarity */   
        case 2:
            std::copy_if(invertedFile.begin(), invertedFile.end(), std::back_inserter(filteredInvertedFile), [&filter](CommentEntry ce){
                return ce.commentScore < 2;
            });
            break;
    }

    return filteredInvertedFile;
}

std::ostream& operator<<(std::ostream& os, const WordEntry* dt)
{
    os << "Entry for " << dt->word << ":" << std::endl;
    os << "Average word score:\t" << dt->averageScore << std::endl;
    os << "Standard deviation:\t" << dt->standardDeviation << std::endl;
    os << "Ocurrences:\t\t" << dt->count << std::endl;
    return os;
}