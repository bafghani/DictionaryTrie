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

/* TODO */
DictionaryTrie::DictionaryTrieNode::DictionaryTrieNode(char thisLabel)
    : nodeLabel(thisLabel),
      left(0),
      right(0),
      parent(0),
      child(0),
      isWordNode(false) {}

DictionaryTrie::DictionaryTrie() : root(0) {}

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    // checks if the word is empty or the freq. is less than 0
    if (word.length() == 0 || freq <= 0) {
        return false;
    }

    DictionaryTrieNode* curr = root;

    // if the tree is empty
    if (root == nullptr) {
        root = new DictionaryTrieNode(word.at(0));
        curr = root;

        int currIndex = 1;
        // iterates through the word and returns
        while (currIndex < word.length()) {
            curr->child = new DictionaryTrieNode(word.at(currIndex));
            curr = curr->child;
            currIndex++;
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

/* TODO */
bool DictionaryTrie::find(string word) const {
    // creates curr node and sets it to root
    DictionaryTrieNode* curr = root;

    // iterator to iterate through the string
    string::iterator wordIter = word.begin();

    if (curr == nullptr) {
        return false;
    }

    while (true) {
        if (curr == nullptr) {
            return 0;
        }

        // if the curr character is less than that of the node go left
        if (*wordIter < curr->nodeLabel) {
            if (curr->left != nullptr) {
                curr = curr->left;
            }
            // if no left exists return false
            else {
                return false;
            }
        }

        // if the curr character is greater than that of the node go right
        else if (*wordIter > curr->nodeLabel) {
            if (curr->right != nullptr) {
                curr = curr->right;
            }
            // if no right child exists return false
            else {
                return false;
            }
        }

        // if the value of the curr character is equal to that of the node's
        else {
            // if we are at the end of the string and the node is a word node
            if (wordIter == word.end() - 1 && *wordIter == curr->nodeLabel &&
                curr->isWordNode) {
                return true;
            }
            // if we are not at the end of the word
            else {
                // set the curr to it's child and iterate to the next
                // character in the string
                if (curr->nodeLabel == *wordIter) {
                    curr = curr->child;
                    wordIter++;
                }
                // else statement for safety's sake
                else {
                    return false;
                }
            }
        }
    }
}

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    // string to store the final completion set
    vector<string> finalCompletions;
    // if numcompletions is less than equal to 0 return
    if (numCompletions <= 0) {
        return finalCompletions;
    }
    // find the node that matches to the last letter of the prefix
    DictionaryTrieNode* lastPrefixNode = findNode(prefix);

    // if this node is null, return
    if (lastPrefixNode == nullptr) {
        return finalCompletions;
    }

    // if node is a word, push to prioirty queue
    if (lastPrefixNode->isWordNode) {
        thisPQ.push(pair<int, string>(lastPrefixNode->wordFrequency, prefix));
    }

    // call helper
    depthFirst(prefix, lastPrefixNode->child, numCompletions);

    // pop into the vector from pq
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

/* TODO */
DictionaryTrie::~DictionaryTrie() {
    // call a destructor helper method
    deleteAll(root);
}

// Insert Node Helper
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::insertNode(
    string word, int currentIndex, int wordFreq, DictionaryTrieNode* curr) {
    // if the node does not exists in the trie yet
    if (curr == nullptr) {
        if (currentIndex == word.length() - 1) {
            curr = new DictionaryTrieNode(word.at(currentIndex));
            curr->wordFrequency = wordFreq;
            curr->isWordNode = true;
            return curr;
        } else {
            curr = new DictionaryTrieNode(word.at(currentIndex));
        }
    }
    // if the node exists
    else {
        if (currentIndex == word.length() - 1 &&
            curr->nodeLabel == word.at(currentIndex) &&
            curr->isWordNode == false) {
            curr->isWordNode = true;
            curr->wordFrequency = wordFreq;
            return curr;
        }
    }

    // recurse left
    if (word.at(currentIndex) < curr->nodeLabel) {
        curr->left = insertNode(word, currentIndex, wordFreq, curr->left);
    }

    // recurse right
    else if (word.at(currentIndex) > curr->nodeLabel) {
        curr->right = insertNode(word, currentIndex, wordFreq, curr->right);
    }
    // recurse child
    else {
        curr->child = insertNode(word, currentIndex + 1, wordFreq, curr->child);
    }
    // return the curr
    return curr;
}

/* Method Description: Method is a helper method to find the node of the
 * last letter in a given prefix, which is passed as the function argument.
 */
DictionaryTrie::DictionaryTrieNode* DictionaryTrie::findNode(string prefix) {
    // Sets a curr node to traverse as initally root
    DictionaryTrieNode* curr = root;
    // iterator to traverse the string
    string::iterator stringIter = prefix.begin();

    if (root == nullptr) {
        return nullptr;
    }

    while (true) {
        // if the node is null, just return null
        if (curr == nullptr) {
            break;
        }

        // if the curr character is less than that of the node go left
        if (*stringIter < curr->nodeLabel) {
            if (curr->left) {
                curr = curr->left;
            } else {
                return nullptr;
            }
        }

        // if the curr character is greater than that of the node go right
        else if (*stringIter > curr->nodeLabel) {
            if (curr->right) {
                curr = curr->right;
            } else {
                return nullptr;
            }
        }

        // if the value of the curr character is equal to that of the node's
        else {
            // if we are at the end of the prefix
            if (stringIter == prefix.end() - 1 &&
                *stringIter == curr->nodeLabel) {
                return curr;
            }
            // iterate to the child
            else {
                if (curr->nodeLabel == *stringIter) {
                    curr = curr->child;
                    stringIter++;
                } else {
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}

/* Method Description: This method is a helper method for the
 * predictCompletions method. Takes in a prefix, and a curr and
 * then builds all of the words that come after the prefix and then adds
 * them to the priority queue thisPQ. Returns a string  vector of up to the
 * number of desired completions.
 */
void DictionaryTrie::depthFirst(string prefix, DictionaryTrieNode* curr,
                                int numCompletions) {
    // if curr is not null
    if (curr) {
        // recurse on the left and right subtrees
        depthFirst(prefix, curr->left, numCompletions);
        depthFirst(prefix, curr->right, numCompletions);

        // add node char to prefix
        prefix.push_back(curr->nodeLabel);

        // if it is word node add to priority queue
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

/* Method Description: Helper method for the destructor. Given the root
 * of a given subtrie, recursively deletes the right, left, and child
 * subtrees before deleting the node itself.
 */
void DictionaryTrie::deleteAll(DictionaryTrieNode* trieRoot) {
    // if the curr node is null return
    if (trieRoot == nullptr) {
        return;
    }
    // delete left subtree
    deleteAll(trieRoot->left);
    // delete right subtree
    deleteAll(trieRoot->right);
    // delete child subtree
    deleteAll(trieRoot->child);
    // delete curr node
    delete trieRoot;
}
