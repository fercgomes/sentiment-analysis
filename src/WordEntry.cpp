#include <iostream>
#include <cmath>
#include "WordEntry.h"

/*
    CommentEntry class
*/

CommentEntry::CommentEntry(int id, int offset, float score)
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
}

void WordEntry::AddOcurrence(int id, int offset, float score)
{
    /* sets new average */
    averageScore = (count * averageScore + score) / (count + 1);
    count++;
    /* TODO: update std deviation dinamically */

    /* inserts ocurrence into inverted file */
    bool found = false;

    /* look for comment entry */
    std::list<CommentEntry>::iterator it;
    for(it = invertedFile.begin(); it != invertedFile.end(); it++)
    {
        if((*it).commentID == id)
        {
            /* insert word ocurrence */
            ((*it).ocurrences).push_back(offset);
            UpdateSandardDeviation();
            found = true;
        }
    }

    if(!found || invertedFile.empty())
    {
        /* create new comment entry */
        CommentEntry newEntry(id, offset, score);
        invertedFile.push_back(newEntry);
        UpdateSandardDeviation();
    }
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

std::ostream& operator<<(std::ostream& os, const WordEntry* dt)
{
    os << "Entry for " << dt->word << ":" << std::endl;
    os << "Average word score:\t" << dt->averageScore << std::endl;
    os << "Standard deviation:\t" << dt->standardDeviation << std::endl;
    os << "Ocurrences:\t\t" << dt->count << std::endl;
    return os;
}