/*****************
    Trie Tree
*****************/
#include "Trie.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <list>

/* Trie Node */

TrieNode::TrieNode()
{
    for(int i = 0; i < alphabetSize; i++)
    {
        children[i] = nullptr;
    }

    isEndOfWord = false;
}

TrieNode::~TrieNode()
{
    TrieNode* aux;
    /* propagate deallocation */
    for(int i = 0; i < alphabetSize; i++)
    {
        aux = children[i];
        if(aux != nullptr)
        {
            delete aux;
        }
    }
}

/* Trie Tree */

TrieTree::TrieTree()
{
    root = new TrieNode;
}

TrieTree::~TrieTree()
{
    delete root;
}

bool TrieTree::push(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if(!key.empty() && validWord(key))
    {
        TrieNode* pCrawl = root;
        for(std::size_t i = 0; i < key.length(); i++)
        {
            std::size_t index = getCharPosition(tolower(key[i]));

            if(pCrawl->children[index] == nullptr)
            {
                pCrawl->children[index] = new TrieNode;
            }

            pCrawl = pCrawl->children[index];
        }

        pCrawl->isEndOfWord = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool TrieTree::search(std::string key)
{
    TrieNode* pCrawl = root;

    for(std::size_t i = 0; i < key.size(); i++)
    {
        std::size_t index = getCharPosition(key[i]);

        if(pCrawl->children[index] == nullptr)
        {
            return false;
        }
        else
        {
            pCrawl = pCrawl->children[index];
        }
    }

    return (pCrawl != nullptr && pCrawl->isEndOfWord);
}

std::list<std::string> TrieTree::getPreffixes(std::string key)
{
    std::list<std::string> preffixes;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    findPreffix(key, preffixes);

    return preffixes;
}

bool TrieTree::findPreffix(std::string key, std::list<std::string>& target)
{
    TrieNode* pCrawl = root;
    std::size_t i = 0;
    bool found = true;

    while(i < key.size() && found)
    {
        int index = getCharPosition(key[i]);

        if(pCrawl->children[index] == nullptr)
        {
            found = false;
        }
        else
        {
            pCrawl = pCrawl->children[index];
            i++;
        }
    }

    if(found)
    {
        getPreffixesCrawler(pCrawl, key, target);
    }

    return found;
}

void TrieTree::getPreffixesCrawler(TrieNode* subRoot, std::string key, std::list<std::string>& target)
{
    TrieNode* pCrawl = subRoot;

    if(pCrawl->isEndOfWord)
    {
        target.push_back(key);
    }

    for(int i = 0; i < alphabetSize; i++)
    {
        if(pCrawl->children[i] != nullptr)
        {
            auto character = getCharFromPosition(i);
            key.push_back(character);
            getPreffixesCrawler(pCrawl->children[i], key, target);
            key.pop_back();
        }
    }
}

int TrieTree::getCharPosition(char c)
{
    c = tolower(c);
    return c - 'a'; 
}

char TrieTree::getCharFromPosition(std::size_t pos)
{
    return static_cast<char>('a' + pos);
}

bool TrieTree::validWord(std::string word)
{
    bool valid = true;
    std::size_t i = 0;
    while(i < word.size() && valid)
    {
        if(word[i] < 'a' || word[i] > 'z')
        {
            valid = false;
        }
        else
        {
            i++;
        }
    }
    return valid;
}