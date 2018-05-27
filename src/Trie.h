/*****************
    Trie Tree
*****************/
#include <iostream>
#include <string>
#include <vector>

constexpr int alphabetSize = 26;

class TrieNode
{
public:
    TrieNode *children[alphabetSize];
    bool isEndOfWord;

    TrieNode();
    ~TrieNode();
};

class TrieTree
{
public:
    TrieTree();
    ~TrieTree();

    bool push(std::string key);
    bool search(std::string key);
    std::vector<std::string> getSuffixes(std::string key);

private:
    TrieNode* root;

    bool findSuffix(std::string key, std::vector<std::string>& target);
    void getSuffixesCrawler(TrieNode* subRoot, std::string key, std::vector<std::string>& target);

    static std::size_t getCharPosition(char c);
    static char getCharFromPosition(std::size_t pos);
    bool validWord(std::string word);
};