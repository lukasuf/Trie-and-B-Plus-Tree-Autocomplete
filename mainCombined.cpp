//
// Created by mbrit on 11/2/2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <chrono>
#include "Trie.h"
#include "BPT.h"

int main()
{
    using namespace std;
    using namespace std::chrono;

    // variables for performance tracking
    double totalSearchTime = 0.0;
    int totalQueries = 0;

    // ask user which data structure they want to use
    cout << "Select data structure:" << endl;
    cout << "1. Trie" << endl;
    cout << "2. B+ Tree" << endl;
    cout << "Enter choice (1 or 2): ";

    int choice;
    cin >> choice;
    cout << endl;

    // exit if the user enters an invalid option
    if (choice != 1 && choice != 2)
    {
        cout << "Invalid selection. Exiting." << endl;
        return 0;
    }

    // load all words from the data folder
    // only words with at least 3 lowercase letters are added
    vector<string> words;
    regex filter("(^[a-z]{3,}$)");

    for (char l = 'A'; l <= 'Z'; l++)
    {
        string filename = "data/" + string(1, l) + " Words.txt";
        ifstream file(filename);

        if (!file.is_open())
        {
            cerr << "Unable to open file " << filename << endl;
            continue;
        }

        string line;
        while (getline(file, line))
        {
            istringstream stream(line);
            string word;
            while (stream >> word)
            {
                if (regex_match(word, filter))
                {
                    words.push_back(word);
                }
            }
        }

        file.close();
    }

    cout << "Words Size: " << words.size() << endl << endl;

    // build the chosen data structure and measure the build time
    auto buildStart = high_resolution_clock::now();

    Trie* trie = nullptr;
    BPT* bpt = nullptr;
    size_t order = 220;

    if (choice == 1)
    {
        trie = new Trie();
        for (const string& w : words)
        {
            trie->insert(w);
        }
    }
    else
    {
        bpt = new BPT(order);
        for (const string& w : words)
        {
            bpt->insert(w);
        }
    }

    auto buildEnd = high_resolution_clock::now();
    duration<double> buildTime = buildEnd - buildStart;
    cout << "Build Time: " << buildTime.count() << " seconds" << endl << endl;

    // open a file to save all words the user locks in
    // trunc ensures file refreshes
    ofstream outFile("SavedWords.txt", ios::trunc);
    if (!outFile.is_open())
    {
        cerr << "Error: Could not open SavedWords.txt for writing." << endl;
        return 0;
    }

    cout << "Words will be saved to SavedWords.txt" << endl << endl;

    // the main autocomplete loop runs until the user types 0
    while (true)
    {
        cout << "Enter a (partial) lower-case word, 's' to save manually, or '0' to exit: ";
        string prefix;
        cin >> prefix;

        // allow user to manually save a word without searching
        if (prefix == "s")
        {
            cout << "Enter the full word you would like to save: ";
            string customWord;
            cin >> customWord;
            outFile << customWord << " ";
            cout << customWord << " saved manually." << endl << endl;
            continue;
        }

        // exit condition
        if (prefix == "0")
        {
            cout << endl << "Exiting program..." << endl;
            break;
        }

        // check if the entered word exists in the dataset
        bool prefixExists = false;
        for (const string& w : words)
        {
            if (w == prefix)
            {
                prefixExists = true;
                break;
            }
        }

        // measure the time it takes to find autocomplete options
        auto searchStart = high_resolution_clock::now();

        queue<string> acOptions;
        if (choice == 1)
        {
            acOptions = trie->findAutoCompleteOptions(prefix);
        }
        else
        {
            acOptions = bpt->findAutoCompleteOptions(prefix);
        }

        auto searchEnd = high_resolution_clock::now();
        duration<double, milli> searchTime = searchEnd - searchStart;
        cout << "Search Time: " << searchTime.count() << " ms" << endl;
        // track total query time and count
        totalSearchTime += searchTime.count();
        totalQueries++;

        // handle case when no completions are found
        if (acOptions.empty())
        {
            if (!prefixExists)
            {
                cout << "Error: not a real word with no endings." << endl << endl;
                continue; // skip back to next prefix
            }
            else
            {
                cout << "No possible endings." << endl;
                cout << "Would you like to save this word? (y/n): ";
                char saveChoice;
                cin >> saveChoice;

                if (tolower(saveChoice) == 'y')
                {
                    outFile << prefix << " ";
                    cout << prefix << " saved." << endl << endl;
                }
                else
                {
                    cout << "Word not saved." << endl << endl;
                }

                continue; // then continue to next prefix
            }
        }

        // cycle through autocomplete suggestions
        string option = "";
        string entry = "";

        while (!acOptions.empty() && option != "e")
        {
            entry = acOptions.front();
            cout << "Current Autocomplete Option: " << entry << endl;
            cout << "Enter (e) to lock in the word or (t) to cycle to next word: ";
            cin >> option;

            if (option == "e")
            {
                cout << entry << " entered." << endl << endl;
                outFile << entry << " ";
            }
            else if (option == "t" && acOptions.size() == 1)
            {
                cout << "Out of words." << endl << endl;
            }
            else if (option == "t")
            {
                // do nothing, next iteration will show next word
            }
            else
            {
                cout << "Invalid entry. Try again." << endl;
                continue;
            }

            acOptions.pop();
        }
    }

    // cleanup and exit
    outFile.close();
    delete trie;
    delete bpt;

    // print session summary
    cout << "\n--- Efficiency Summary ---" << endl;
    cout << "Build Time: " << buildTime.count() << " seconds" << endl;
    if (totalQueries > 0)
    {
        cout << "Total Queries: " << totalQueries << endl;
        cout << "Average Search Time: " << (totalSearchTime / totalQueries) << " ms" << endl;
    }
    else
    {
        cout << "No valid queries were performed." << endl;
    }
    cout << "Saved words written to: SavedWords.txt" << endl;
    cout << "---------------------------" << endl;

    cout << endl << "Writing Completed. Goodbye!" << endl;
    return 0;
}
