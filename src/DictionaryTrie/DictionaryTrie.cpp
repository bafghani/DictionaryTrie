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
        root = new DictionaryTrieNode(word[0]);
        curr = root;

        unsigned int index = 1;

        while (index < word.length()) {
            curr->child = new DictionaryTrieNode(word[index]);
            curr = curr->child;
            index++;
        }

        curr->isWordNode = true;
        curr->Frequency = freq;

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
    string::iterator Itr = word.begin();

    if (curr == nullptr) {  // edge case null node
        return false;
    }

    while (true) {
        if (curr == nullptr) {  // node does not exist
            return 0;
        }

        if (*Itr < curr->nodeLabel) {  // Traverse left
            if (curr->left != nullptr) {
                curr = curr->left;
            } else {
                return false;
            }
        }

        else if (*Itr > curr->nodeLabel) {  // Traverse right
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
            if (Itr == word.end() - 1 && *Itr == curr->nodeLabel &&
                curr->isWordNode) {
                return true;  // find successful
            }

            else {  // if word is not complete continue traversing downward
                // Traverse down to curr->child
                if (curr->nodeLabel ==
                    *Itr) {  // if char matches the char in the string
                    curr = curr->child;
                    Itr++;
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
    vector<string> completionSet;  // vector to store all predictions

    if (numCompletions <= 0) {  // Edge case if numCompletions is <= 0
        return CompletionSet;
    }
    // find node that contains prefix
    DictionaryTrieNode* endOfPrefix = findNode(prefix);

    if (endOfPrefix == nullptr) {  // Returns if node is null
        return CompletionSet;
    }

    // if node is a word, push to Priority Queue
    if (endOfPrefix->isWordNode) {
        thisPQ.push(pair<int, string>(endOfPrefix->Frequency, prefix));
    }

    // Depth First Search
    depthFirst(prefix, endOfPrefix->child, numCompletions);

    while (thisPQ.size() != 0) {  // push into vector
        CompletionSet.push_back(thisPQ.top().second);
        thisPQ.pop();  // pop from Priority Queue
    }
    return CompletionSet;
}

/* predicts words given a pattern with underscores
 * @param pattern, the pattern we want to complete
 * @param numCompletions, the number of suggestions we want
 * @return a vector of suggested completions
 **/
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    vector<string> completionSet;  // vector to store string predictions
    // Edge case (numCompletions <= 0 or pattern is empty string)
    if (numCompletions <= 0 || pattern == "") {
        return completionSet;  // return empty list
    }
    // helper method to recursively find the underscore patterns
    predictUnderscoresHelper(pattern, "", 0, root, numCompletions);
    // add priority queue's elements into vector in required order
    while (!underscorePQ.empty()) {
        completionSet.push_back(underscorePQ.top().second);
        underscorePQ.pop();
    }
    // return the vector of suggested completions
    return completionSet;
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
    string word, int index, int Freq, DictionaryTrieNode* curr) {
    if (curr == nullptr) {  // if the node does not exists
        if (index == word.length() - 1) {
            curr = new DictionaryTrieNode(word[index]);
            curr->Frequency = Freq;
            curr->isWordNode = true;
            /* MAX FREQ UPDATE */
            curr->maxFrequency = Freq;
            if (curr->left->wordFrequency > curr->maxFrequency) {
                curr->maxFrequency = curr->left->wordFrequency;
            }
            if (curr->right->wordFrequency > curr->maxFrequency) {
                curr->maxFrequency = curr->right->wordFrequency;
            }
            return curr;
        } else {
            curr = new DictionaryTrieNode(word[index]);
        }
    }

    else {  // if the node exists
        if (index == word.length() - 1 && curr->nodeLabel == word[index] &&
            curr->isWordNode == false) {
            curr->isWordNode = true;
            curr->Frequency = Freq;
            /* MAX FREQ UPDATE */
            curr->maxFrequency = Freq;
            if (curr->left->wordFrequency > curr->maxFrequency) {
                curr->maxFrequency = curr->left->wordFrequency;
            }
            if (curr->right->wordFrequency > curr->maxFrequency) {
                curr->maxFrequency = curr->right->wordFrequency;
            }
            return curr;
        }
    }

    if (word[index] < curr->nodeLabel) {  // recurse left
        curr->left = insertNode(word, index, Freq, curr->left);
    }

    else if (word[index] > curr->nodeLabel) {  // recurse right
        curr->right = insertNode(word, index, Freq, curr->right);
    }

    else {  // recurse child
        curr->child = insertNode(word, index + 1, Freq, curr->child);
    }

    return curr;  // return the current node
}

/**
 * findNode: Helper Method for Find
 */
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::findNode(string prefix) {
    DictionaryTrieNode* curr = root;

    string::iterator Itr = prefix.begin();  // iterate through the string

    if (root == nullptr) {  // edge case null root
        return nullptr;
    }

    while (true) {
        if (curr == nullptr) {
            break;
        }

        if (*Itr < curr->nodeLabel) {  // Iterate left
            if (curr->left) {
                curr = curr->left;
            } else {
                return nullptr;
            }
        }

        else if (*Itr > curr->nodeLabel) {  // Iterate right
            if (curr->right) {
                curr = curr->right;
            } else {
                return nullptr;
            }
        }

        else {  // Iterate down
            // we are at the end of prefix
            if (Itr == prefix.end() - 1 && *Itr == curr->nodeLabel) {
                return curr;
            }

            else {  // iterate to the child
                if (curr->nodeLabel == *Itr) {
                    curr = curr->child;
                    Itr++;
                } else {
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}

/**
 * DFS : Helper method to optimize predictCompletions
 */
void DictionaryTrie::depthFirst(string prefix, DictionaryTrieNode* curr,
                                int numCompletions) {
    if (curr) {  // if curr exists

        depthFirst(prefix, curr->left, numCompletions);  // recurse left

        depthFirst(prefix, curr->right, numCompletions);  // recurse right

        prefix.push_back(curr->nodeLabel);

        if (curr->isWordNode) {  // added to PQ
            if (thisPQ.size() < numCompletions) {
                thisPQ.push(pair<int, string>(curr->Frequency, prefix));
            } else {
                if (thisPQ.top().first < curr->Frequency) {
                    thisPQ.pop();
                    thisPQ.push(pair<int, string>(curr->Frequency, prefix));
                }
            }
        }

        depthFirst(prefix, curr->child, numCompletions);  // recurse child
    }
}

/**
 * Underscore Helper method
 */

void DictionaryTrie::predictUnderscoresHelper(string pattern,
                                              string currentProgress, int index,
                                              DictionaryTrieNode* curr,
                                              int numCompletions) {
    // Edge Case (Index Out of Bounds)
    if (index >= pattern.length()) {
        return;
    }

    // Edge Case (Current Node is null)
    if (curr == NULL) {
        return;
    }

    // If the current index of the string is an underscore
    if (pattern.[index] == '_') {
        // Recursively retrieves all the underscores on the left subtrie
        if (curr->left) {
            predictUnderscoresHelper(pattern, currentProgress, index,
                                     curr->left, numCompletions);
        }

        // recursively retrieves all the underscores on the right subtrie
        if (curr->right) {
            predictUnderscoresHelper(pattern, currentProgress, index,
                                     curr->right, numCompletions);
        }

        // push back to update current pattern in progress by adding curr's char
        currentProgress.push_back(curr->nodeLabel);
        // if curr is a word node and we have reached the last index
        if (curr->isWordNode && index == pattern.length() - 1) {
            if (underscorePQ.size() < numCompletions) {
                underscorePQ.push(pair<int, string>(
                    curr->Frequency, currentProgress));  // add to PQ
            } else {
                if (curr->Frequency >
                    underscorePQ.top()
                        .first) {  // if curr has greater frequency
                                   // than top of PQ, replace with curr
                    underscorePQ.pop();
                    underscorePQ.push(
                        pair<int, string>(curr->Frequency, currentProgress));
                }
            }
            return;
        }
        // if it is not a word node recurse on child
        if (curr->child) {
            predictUnderscoresHelper(pattern, currentProgress, index + 1,
                                     curr->child, numCompletions);
        }
    }
    // if not an underscore

    else {
        // if the char of current is equal to the string's char
        if (pattern[index] == curr->nodeLabel) {
            currentProgress.push_back(
                curr->nodeLabel);  // add char to pattern in progress

            // if we are at the end of the prefix
            if (curr->isWordNode && index == pattern.length() - 1) {
                if (underscorePQ.size() < numCompletions) {
                    underscorePQ.push(pair<int, string>(
                        curr->Frequency,
                        currentProgress));  // push new pair to PQ
                } else {
                    if (curr->Frequency > underscorePQ.top().first) {
                        underscorePQ.pop();
                        underscorePQ.push(pair<int, string>(
                            curr->Frequency,
                            currentProgress));  // update order of PQ
                    }
                }
                return;
            }
            // if we are not at the end of the prefix
            else {  // recurse child
                predictUnderscoresHelper(pattern, currentProgress, index + 1,
                                         curr->child, numCompletions);
            }
        }
        // if the chars are not equal
        else {
            // if less than current node's char then recurse on left
            if (pattern[index] < curr->nodeLabel) {
                predictUnderscoresHelper(pattern, currentProgress, index,
                                         curr->left, numCompletions);
            }
            // if greater than current node's char recurse on right
            if (pattern[index] > curr->nodeLabel) {
                predictUnderscoresHelper(pattern, currentProgress, index,
                                         curr->right, numCompletions);
            }
        }
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
