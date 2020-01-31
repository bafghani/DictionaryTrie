/**
 * This file implements functions defined
 * in DictionaryTrie.hpp in order to build
 * a fully functional Dictionary Trie
 * based on a MWT
 *
 * Authors: Joseph Mattingly
 *          Bijan Afghani
 */
#include "DictionaryTrie.hpp"
#include <string.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>

/**
 * Node constructor
 * @param thisLabel the letter we want
 * our node to store
 */
DictionaryTrie::DictionaryTrieNode::DictionaryTrieNode(char thisLabel)
    : nodeLabel(thisLabel),
      left(0),
      right(0),
      parent(0),
      child(0),
      isWordNode(false) {}

DictionaryTrie::DictionaryTrie() : root(0) {}

/* inserts a new word into the dictionary
 * @param word, the word we want to insert
 * @param freq, the number of times that word occurs
 * @return true if inserted false if duplicate
 **/
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (word.length() == 0 || freq <= 0) {
        return false;
    }

    DictionaryTrieNode* curr = root;

    // if the tree is empty
    if (root == nullptr) {
        root = new DictionaryTrieNode(word.at(0));
        curr = root;

        int index = 1;

        while (index < word.length()) {
            curr->child = new DictionaryTrieNode(word.at(index));
            curr = curr->child;
            index++;
        }

        curr->isWordNode = true;
        curr->wordFrequency = freq;

        return true;
    }

    // if word exists, return false
    if (find(word)) {
        return false;
    }

    // insert helper function that builds the subtree
    root = insertNode(word, 0, freq, root);

    return true;
}

/* finds a word in the dictionary
 * @param word we want to find
 * @return true if found false otherwise
 **/
bool DictionaryTrie::find(string word) const {
    // creates curr node and sets it to root
    DictionaryTrieNode* curr = root;

    // iterate through the characters of the word
    string::iterator strItr = word.begin();

    if (curr == nullptr) {  // edge case null node
        return false;
    }

    while (true) {
        if (curr == nullptr) {
            return 0;
        }

        if (*strItr < curr->nodeLabel) {  // Traverse left
            if (curr->left != nullptr) {
                curr = curr->left;
            } else {
                return false;
            }
        }

        else if (*strItr > curr->nodeLabel) {  // Traverse right
            if (curr->right != nullptr) {
                curr = curr->right;
            } else {
                return false;
            }
        }

        // if the character of the iterator is the same as that of the node
        else {
            // if we are at the end of the word and the node is the end of a
            // word
            if (strItr == word.end() - 1 && *strItr == curr->nodeLabel &&
                curr->isWordNode) {
                return true;
            }

            else {  // if word is not complete continue traversing downward
                // Traverse down to curr->child
                if (curr->nodeLabel ==
                    *strItr) {  // if char matches the char in the string
                    curr = curr->child;
                    strItr++;
                }

                else {  // otherwise word does not exist
                    return false;
                }
            }
        }
    }
}

/* predicts words given a prefix based on words with
 * the highest frequencies
 * @param prefix, the prefix we want to complete
 * @param numCompletions, the number of suggestions we want
 * @return a vector of suggested completions
 **/
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    vector<string> finalCompletions;  // vector to store all predictions

    if (numCompletions <= 0) {  // Edge case if numCompletions is <= 0
        return finalCompletions;
    }
    // find node that contains prefix
    DictionaryTrieNode* lastPrefixNode = findNode(prefix);

    if (lastPrefixNode == nullptr) {  // Returns if node is null
        return finalCompletions;
    }

    // if node is a word, push to Priority Queue
    if (lastPrefixNode->isWordNode) {
        thisPQ.push(pair<int, string>(lastPrefixNode->wordFrequency, prefix));
    }

    // Depth First Search
    depthFirst(prefix, lastPrefixNode->child, numCompletions);

    while (thisPQ.size() != 0) {  // push into vector
        finalCompletions.push_back(thisPQ.top().second);
        thisPQ.pop();  // pop from Priority Queue
    }
    return finalCompletions;
}

/* predicts words given a pattern with underscores
 * @param pattern, the pattern we want to complete
 * @param numCompletions, the number of suggestions we want
 * @return a vector of suggested completions
 **/
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/**
 * Destructor
 * deletes all nodes of Dictionary Trie
 * Avoids Memory Leaks
 */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

/**
 * Insert Node Helper Method
 */
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::insertNode(
    string word, int index, int wordFreq, DictionaryTrieNode* curr) {
    if (curr == nullptr) {  // if the node does not exists
        if (index == word.length() - 1) {
            curr = new DictionaryTrieNode(word.at(index));
            curr->wordFrequency = wordFreq;
            curr->isWordNode = true;
            return curr;
        } else {
            curr = new DictionaryTrieNode(word.at(index));
        }
    }

    else {  // if the node exists
        if (index == word.length() - 1 && curr->nodeLabel == word.at(index) &&
            curr->isWordNode == false) {
            curr->isWordNode = true;
            curr->wordFrequency = wordFreq;
            return curr;
        }
    }

    if (word.at(index) < curr->nodeLabel) {  // recurse left
        curr->left = insertNode(word, index, wordFreq, curr->left);
    }

    else if (word.at(index) > curr->nodeLabel) {  // recurse right
        curr->right = insertNode(word, index, wordFreq, curr->right);
    }

    else {
        / / recurse child curr->child =
            insertNode(word, index + 1, wordFreq, curr->child);
    }

    return curr;  // return the current node
}

/**
 * findNode
 */
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::findNode(string prefix) {
    DictionaryTrieNode* curr = root;

    string::iterator strItr = prefix.begin();  // iterate through the string

    if (root == nullptr) {  // edge case null root
        return nullptr;
    }

    while (true) {
        if (curr == nullptr) {
            break;
        }

        if (*strItr < curr->nodeLabel) {  // Iterate left
            if (curr->left) {
                curr = curr->left;
            } else {
                return nullptr;
            }
        }

        else if (*strItr > curr->nodeLabel) {  // Iterate right
            if (curr->right) {
                curr = curr->right;
            } else {
                return nullptr;
            }
        }

        else {  // Iterate down
            // we are at prefix
            if (strItr == prefix.end() - 1 && *strItr == curr->nodeLabel) {
                return curr;
            }

            else {  // iterate to the child
                if (curr->nodeLabel == *strItr) {
                    curr = curr->child;
                    strItr++;
                } else {
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}

/**
 * DFS
 */
void DictionaryTrie::depthFirst(string prefix, DictionaryTrieNode* curr,
                                int numCompletions) {
    if (curr) {  // if curr exists

        depthFirst(prefix, curr->left, numCompletions);  // recurse left

        depthFirst(prefix, curr->right, numCompletions);  // recurse right

        prefix.push_back(curr->nodeLabel);

        if (curr->isWordNode) {  // if the word was added to Priority Queue
            if (thisPQ.size() < numCompletions) {
                thisPQ.push(pair<int, string>(curr->wordFrequency, prefix));
            } else {
                if (thisPQ.top().first < curr->wordFrequency) {
                    thisPQ.pop();
                    thisPQ.push(pair<int, string>(curr->wordFrequency, prefix));
                }
            }
        }

        depthFirst(prefix, curr->child, numCompletions);  // recurse child
    }
}

/**
 * Destructor Helper
 * Recursively deletes all nodes
 */
void DictionaryTrie::deleteAll(DictionaryTrieNode* root) {
    if (root == nullptr) {
        return;
    }

    deleteAll(root->left);
    deleteAll(root->right);
    deleteAll(root->child);
    delete root;
}
