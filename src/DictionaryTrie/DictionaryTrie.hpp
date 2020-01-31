/**
 * This hpp file defines the functions and structure
 * of the DictionaryTrie, the DictionaryTrieNode,
 * and the wordComparator for comparing <string, int> pairs
 *
 * Authors: Joseph Mattingly
 *          Bijan Afghani
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/* comparator structure for comparing pairs
 * This compares <string, int> pairs
 * first the strings are compared in alphabetically
 * If the strings are equal we compare the ints
 * the smaller int is returned.
 */
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
 * a multi-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    /**
     * The class for a dictionary multi-way trie node
     * Each node holds a char, and has spatial pointers
     * used to determine the next node/letter in sequence
     */
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
    /* method to insert a new word into the dictionary */
    DictionaryTrieNode* insertNode(string word, int currentIndex, int wordFreq,
                                   DictionaryTrieNode* currentNode);
    /* method to find a given word in the dictionary */
    DictionaryTrieNode* findNode(string word);
    /* depth first search for prefix (helper for predict) */
    void depthFirst(string prefix, DictionaryTrieNode* currentNode,
                    int numCompletions);
    /* helper for destructor */
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
    /* Initializes an empty DictionaryTrie */
    DictionaryTrie();

    /* inserts a new word into the dictionary
     * @param word, the word we want to insert
     * @param freq, the number of times that word occurs
     * @return true if inserted false if duplicate
     **/
    bool insert(string word, unsigned int freq);

    /* finds a word in the dictionary
     * @param word we want to find
     * @return true if found false otherwise
     **/
    bool find(string word) const;

    /* predicts words given a prefix based on words with
     * the highest frequencies
     * @param prefix, the prefix we want to complete
     * @param numCompletions, the number of suggestions we want
     * @return a vector of suggested completions
     **/
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* predicts words given a pattern with underscores
     * @param pattern, the pattern we want to complete
     * @param numCompletions, the number of suggestions we want
     * @return a vector of suggested completions
     **/
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* Destructor for the DictionaryTrie object to avoid memory leaks */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
