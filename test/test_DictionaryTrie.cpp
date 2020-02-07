/**
 * This File contains several tests
 * that check the functionality of the
 * Dictionary Trie functions
 *
 * Author: Joseph Mattingly
 *         Bijan Afghani
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}

/* INSERT/FIND TESTS */
TEST(DictTrieTests, INSERT_ROOT) {
    DictionaryTrie dict;
    dict.insert("bijan", 1);
    ASSERT_EQ(dict.root->nodeLabel, 'b');
}
TEST(DictTrieTests, FIND_ROOT) {
    DictionaryTrie dict;
    dict.insert("joey", 1);
    ASSERT_EQ(dict.find("joey"), true);
}
TEST(DictTrieTests, INSERT_MULTIPLE) {
    DictionaryTrie dict;
    dict.insert("bijan", 1);
    dict.insert("apple", 1);
    dict.insert("joey", 1);
    ASSERT_EQ(dict.find("apple"), true);
}

/*  PREDICT COMPLETIONS TESTS   */
TEST(DictTrieTests, PREDICT_EMPTY) {
    DictionaryTrie dict;
    vector<string> vec = dict.predictCompletions("bij", 2);
    vector<string> emp;
    ASSERT_EQ(vec, emp);
}
TEST(DictTrieTests, PREDICT_SMALL) {
    DictionaryTrie dict;
    vector<string> compare{"apparent", "ape", "appendage", "apple"};
    dict.insert("apple", 5);
    dict.insert("appendage", 4);
    dict.insert("ape", 3);
    dict.insert("apparent", 2);
    vector<string> vec = dict.predictCompletions("ap", 4);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, PREDICT_LARGE_NUMCOMPLETIONS) {
    DictionaryTrie dict;
    vector<string> compare{"apparent", "ape", "appendage", "apple"};
    dict.insert("apple", 5);
    dict.insert("appendage", 4);
    dict.insert("ape", 3);
    dict.insert("apparent", 2);
    dict.insert("tired", 69);
    dict.insert("sad", 420);
    dict.insert("deez", 9000);
    dict.insert("nuts", 5000);
    vector<string> vec = dict.predictCompletions("ap", 7);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, PREDICT_1) {
    DictionaryTrie dict;
    vector<string> compare{"apple"};
    dict.insert("tired", 69);
    dict.insert("sad", 420);
    dict.insert("deez", 9000);
    dict.insert("nuts", 5000);
    dict.insert("apple", 5);
    dict.insert("banana", 4);
    dict.insert("orange", 3);
    dict.insert("grape", 2);
    vector<string> vec = dict.predictCompletions("ap", 3);
    ASSERT_EQ(compare, vec);
}

/*  PREDICT UNDERSCORES TESTS   */

TEST(DictTrieTests, COMPLETE_EMPTY) {
    DictionaryTrie dict;
    vector<string> compare;
    vector<string> vec = dict.predictUnderscores("bi_", 2);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_END_OF_WORD) {
    DictionaryTrie dict;
    vector<string> compare({"bij"});
    dict.insert("bij", 5);
    vector<string> vec = dict.predictUnderscores("bi_", 2);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_MIDDLE_OF_WORD) {
    DictionaryTrie dict;
    vector<string> compare({"bij"});
    dict.insert("bij", 5);
    vector<string> vec = dict.predictUnderscores("b_j", 2);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_BEGINNING_OF_WORD) {
    DictionaryTrie dict;
    vector<string> compare({"bij"});
    dict.insert("bij", 5);
    vector<string> vec = dict.predictUnderscores("_ij", 2);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_MULTIPLE_WORDS) {
    DictionaryTrie dict;
    vector<string> compare({"baj", "boj", "bij"});
    dict.insert("bij", 5);
    dict.insert("boj", 4);
    dict.insert("baj", 3);
    vector<string> vec = dict.predictUnderscores("b_j", 3);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_MULTIPLE_UNDERSCORES) {
    DictionaryTrie dict;
    vector<string> compare({"bij"});
    dict.insert("bij", 5);
    vector<string> vec = dict.predictUnderscores("b__", 2);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_MULTIPLE_UNDERSCORES_2) {
    DictionaryTrie dict;
    vector<string> compare({"bij"});
    dict.insert("bij", 5);
    vector<string> vec = dict.predictUnderscores("_i_", 2);
    ASSERT_EQ(compare, vec);
}
TEST(DictTrieTests, COMPLETE_ALL_UNDERSCORES) {
    DictionaryTrie dict;
    vector<string> compare({"bij"});
    dict.insert("bij", 5);
    vector<string> vec = dict.predictUnderscores("___", 1);
    ASSERT_EQ(compare, vec);
}
