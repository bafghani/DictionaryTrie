/**
 * TODO: File Header
 *
 * Author: Joseph Mattingly
 * Author:
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
 */
DictionaryTrie::DictionaryTrieNode::DictionaryTrieNode(char thisLabel)
    : nodeLabel(thisLabel),
      left(0),
      right(0),
      parent(0),
      child(0),
      isWordNode(false) {}

DictionaryTrie::DictionaryTrie() : root(0) {}

/**
 * insert
 */
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

    // if word is in trie, return false
    if (find(word)) {
        return false;
    }

    // insert helper function that builds the subtree
    root = insertNode(word, 0, freq, root);

    return true;
}

/**
 * find
 */
bool DictionaryTrie::find(string word) const {
    // creates curr node and sets it to root
    DictionaryTrieNode* curr = root;

    // iterate through the string
    string::iterator strItr = word.begin();

    if (curr == nullptr) {
        return false;
    }

    while (true) {
        if (curr == nullptr) {
            return 0;
        }

        // Left
        if (*strItr < curr->nodeLabel) {
            if (curr->left != nullptr) {
                curr = curr->left;
            } else {
                return false;
            }
        }

        // Right
        else if (*strItr > curr->nodeLabel) {
            if (curr->right != nullptr) {
                curr = curr->right;
            } else {
                return false;
            }
        }

        // if the value of the curr character is equal to that of the node's
        else {
            // if we are at the end of the string and the node is a word node
            if (strItr == word.end() - 1 && *strItr == curr->nodeLabel &&
                curr->isWordNode) {
                return true;
            }
            // if we are not at the end of the word
            else {
                // set the curr to it's child and iterate to the next
                // character in the string
                if (curr->nodeLabel == *strItr) {
                    curr = curr->child;
                    strItr++;
                }
                // else statement for safety's sake
                else {
                    return false;
                }
            }
        }
    }
}

/**
 * predictCompletions
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    // string to store the final completion set
    vector<string> finalCompletions;
    // Returns if numCompletions is less than/equal to 0
    if (numCompletions <= 0) {
        return finalCompletions;
    }
    // find the node that matches last letter of the prefix
    DictionaryTrieNode* lastPrefixNode = findNode(prefix);

    // Returns if node is null
    if (lastPrefixNode == nullptr) {
        return finalCompletions;
    }

    // if node is a word, push to PQ
    if (lastPrefixNode->isWordNode) {
        thisPQ.push(pair<int, string>(lastPrefixNode->wordFrequency, prefix));
    }

    // Calls DPS
    depthFirst(prefix, lastPrefixNode->child, numCompletions);

    // pop into finalCompletions
    while (thisPQ.size() != 0) {
        finalCompletions.push_back(thisPQ.top().second);
        thisPQ.pop();
    }
    return finalCompletions;
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/**
 * Destructor
 */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

/**
 * Insert Node Helper
 */
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::insertNode(
    string word, int index, int wordFreq, DictionaryTrieNode* curr) {
    // if node does not exists
    if (curr == nullptr) {
        if (index == word.length() - 1) {
            curr = new DictionaryTrieNode(word.at(index));
            curr->wordFrequency = wordFreq;
            curr->isWordNode = true;
            return curr;
        } else {
            curr = new DictionaryTrieNode(word.at(index));
        }
    }
    // if node exists
    else {
        if (index == word.length() - 1 && curr->nodeLabel == word.at(index) &&
            curr->isWordNode == false) {
            curr->isWordNode = true;
            curr->wordFrequency = wordFreq;
            return curr;
        }
    }

    // recurse left
    if (word.at(index) < curr->nodeLabel) {
        curr->left = insertNode(word, index, wordFreq, curr->left);
    }

    // recurse right
    else if (word.at(index) > curr->nodeLabel) {
        curr->right = insertNode(word, index, wordFreq, curr->right);
    }
    // recurse child
    else {
        curr->child = insertNode(word, index + 1, wordFreq, curr->child);
    }
    // return the curr
    return curr;
}

/**
 * findNode
 */
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::findNode(string prefix) {
    DictionaryTrieNode* curr = root;

    // iterate through the string
    string::iterator strItr = prefix.begin();

    // root is null
    if (root == nullptr) {
        return nullptr;
    }

    while (true) {
        if (curr == nullptr) {
            break;
        }

        // Left
        if (*strItr < curr->nodeLabel) {
            if (curr->left) {
                curr = curr->left;
            } else {
                return nullptr;
            }
        }

        // Right
        else if (*strItr > curr->nodeLabel) {
            if (curr->right) {
                curr = curr->right;
            } else {
                return nullptr;
            }
        }

        // Down
        else {
            // End of prefix
            if (strItr == prefix.end() - 1 && *strItr == curr->nodeLabel) {
                return curr;
            }
            // iterate to the child
            else {
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
 * DPS
 */
void DictionaryTrie::depthFirst(string prefix, DictionaryTrieNode* curr,
                                int numCompletions) {
    // if curr is not null
    if (curr) {
        // recurse left
        depthFirst(prefix, curr->left, numCompletions);
        // recurse right
        depthFirst(prefix, curr->right, numCompletions);

        prefix.push_back(curr->nodeLabel);

        // if word node add to PQ
        if (curr->isWordNode) {
            if (thisPQ.size() < numCompletions) {
                thisPQ.push(pair<int, string>(curr->wordFrequency, prefix));
            } else {
                if (thisPQ.top().first < curr->wordFrequency) {
                    thisPQ.pop();
                    thisPQ.push(pair<int, string>(curr->wordFrequency, prefix));
                }
            }
        }

        // recurse on child
        depthFirst(prefix, curr->child, numCompletions);
    }
}

/**
 * Destructor Helper
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
