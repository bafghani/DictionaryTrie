/*
 * This file does a bunch of voodoo and then calls
 * the predictCompletions function to give the
 * user of list of suggested words based on their
 * prefix and highest frequency.
 *
 * Authors: Bijan Afghani
 *          Joseph Mattingly
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;

/* Check if a given data file is valid */
bool fileValid(const char* fileName) {
    ifstream in;
    in.open(fileName, ios::binary);

    // Check if input file was actually opened
    if (!in.is_open()) {
        cout << "Invalid input file. No file was opened. Please try again.\n";
        return false;
    }

    // Check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if (len == 0) {
        cout << "The file is empty. \n";
        return false;
    }
    in.close();
    return true;
}

/* IMPORTANT! You should use the following lines of code to match the correct
 * output:
 *
 * cout << "This program needs exactly one argument!" << endl;
 * cout << "Reading file: " << file << endl;
 * cout << "Enter a prefix/pattern to search for:" << endl;
 * cout << "Enter a number of completions:" << endl;
 * cout << completion << endl;
 * cout << "Continue? (y/n)" << endl;
 *
 * arg 1 - Input file name (in format like freq_dict.txt)
 */
int main(int argc, char** argv) {
    const int NUM_ARG = 2;
    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments.\n"
             << "Usage: ./autocomplete <dictionary filename>" << endl;
        return -1;
    }
    if (!fileValid(argv[1])) return -1;

    DictionaryTrie* dt = new DictionaryTrie();

    // Read all the tokens of the file in order to get every word
    cout << "Reading file: " << argv[1] << endl;

    ifstream in;
    in.open(argv[1], ios::binary);
    string word;

    Utils::loadDict(*dt, in);
    in.close();

    char cont = 'y';
    unsigned int numberOfCompletions;
    while (cont == 'y') {
        cout << "Enter a prefix/pattern to search for:" << endl;
        getline(cin, word);
        cout << "Enter a number of completions:" << endl;
        cin >> numberOfCompletions;

        // Checks for valid prefix
        if (word == "") {
            while (word == "") {
                cout << "Enter valid prefix: " << endl;
                cin >> word;
            }
        }

        // check for underscore
        bool isUnderscore = false;
        for (int index = 0; index < word.length(); index++) {
            if (int(word.at(index)) == 95) {
                isUnderscore = true;
                break;
            }
        }

        vector<string> sortedCompletions;

        if (isUnderscore) {
            sortedCompletions =
                dt->predictUnderscores(word, numberOfCompletions);
        } else {
            sortedCompletions =
                dt->predictCompletions(word, numberOfCompletions);
        }

        while (!sortedCompletions.empty()) {
            cout << sortedCompletions.back() << endl;
            sortedCompletions.pop_back();
        }

        cout << "Continue? (y/n)" << endl;
        cin >> cont;
        cin.ignore();
    }
    delete dt;
    return 0;
}
