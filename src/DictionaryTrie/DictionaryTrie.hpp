/**
 * TODO: File Header
 *
 * Author:
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct wordComparator {
    // operator takes in two pairs of <int, string>
    bool operator()(const pair<int, string>& p1, const pair<int, string>& p2) {
        // if the frequencies are equal sort by alphabetical order
        if (p1.first == p2.first) {
            return p1.second < p2.second;
        }
        // else sort from least to greatest frequency
        else {
            return p1.first > p2.first;
        }
    }
};

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    class DictionaryTrieNode {
      public:
        DictionaryTrieNode* parent;
        DictionaryTrieNode* left;
        DictionaryTrieNode* right;
        DictionaryTrieNode* child;
        char const nodeLabel;
        bool isWordNode;
        unsigned int wordFrequency;

        // Default constructor for the DictionaryTrieNode Class
        DictionaryTrieNode(char thisLabel);
    };

    DictionaryTrieNode* insertNode(string word, int currentIndex, int wordFreq,
                                   DictionaryTrieNode* currentNode);

    DictionaryTrieNode* findNode(string word);

    void depthFirst(string prefix, DictionaryTrieNode* currentNode,
                    int numCompletions);

    void deleteAll(DictionaryTrieNode* trieRoot);

  public:
    // root node of the trie, first letter of first inserted word
    DictionaryTrieNode* root;

    // priority queue to store the words that predictUnderscores finds
    priority_queue<pair<int, string>, vector<pair<int, string>>, wordComparator>
        underscorePQ;

    // priority queue to store the words that predictCompletions finds
    priority_queue<pair<int, string>, vector<pair<int, string>>, wordComparator>
        thisPQ;

    // TODO: add private members and helper methods here
  public:
    /* TODO: add function header */
    DictionaryTrie();

    /* TODO: add function header */
    bool insert(string word, unsigned int freq);

    /* TODO: add function header */
    bool find(string word) const;

    /* TODO: add function header */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
