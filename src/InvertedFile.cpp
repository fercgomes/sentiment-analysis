#include "InvertedFile.h"
#include <string>
#include <iostream>

InvertedFileEntry::InvertedFileEntry(std::string w)
{
   word = w;
}

InvertedFileEntry::~InvertedFileEntry()
{}

void InvertedFileEntry::AddWordOcurrence(std::size_t commentID, std::size_t wordPos, float commentScore)
{
    bool found = false;

    /* look for comment entry */
    std::list<CommentEntry>::iterator it;
    for(it = invFile.begin(); it != invFile.end(); it++)
    {
        if((*it).commentID == commentID)
        {
            /* insert word ocurrence */
            ((*it).ocurrences).push_back(wordPos);
            found = true;
        }
    }

    if(!found || invFile.empty())
    {
        /* create new comment entry */
        CommentEntry newEntry(commentID, commentScore);
        (newEntry.ocurrences).push_back(wordPos);
        invFile.push_back(newEntry);
    }
}

void InvertedFileEntry::PrintOcurrences()
{
    std::list<CommentEntry>::iterator comment;
    std::cout << "Printing ocurrences of word " << word << "." << std::endl;
    for(comment = invFile.begin(); comment != invFile.end(); comment++)
    {
        std::cout << "Comment ID: " << (*comment).commentID << std::endl;

        std::list<std::size_t>::iterator word;
        for(word = ((*comment).ocurrences).begin(); word != ((*comment).ocurrences).end(); word++)
        {
            std::cout << (*word) << ", ";
        } 

        std::cout << std::endl;
    }
}