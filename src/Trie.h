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
    /* default constructors and destructors */
    TrieTree();
    ~TrieTree();

    /* default operations */
    /* PUSH: key is a word to be pushed in to the Trie Tree */
    bool push(std::string key);
    /* SEARCH: returns wheter the key is present */
    bool search(std::string key);
    /* GET PREFFIXES: returns a vector containing the words beginning with 'key' */
    std::vector<std::string> getPreffixes(std::string key);

private:
    TrieNode* root;

    bool findPreffix(std::string key, std::vector<std::string>& target);
    void getPreffixesCrawler(TrieNode* subRoot, std::string key, std::vector<std::string>& target);

    static int getCharPosition(char c);
    static char getCharFromPosition(std::size_t pos);
    bool validWord(std::string word);
};