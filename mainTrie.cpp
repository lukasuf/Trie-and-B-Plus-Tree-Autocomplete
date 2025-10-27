//
// Created by mbrit on 10/26/2025.
//
#include "Trie.h"
#include <iostream>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include "Trie.h"

int main()
{
    std::vector<std::string> words;

    // define the regex pattern for valid autocomplete options
    // [a-z] - alphabetical; {3,} - at least 3 letters; ^/$ - ensures the entire word matches
    std::regex filter("(^[a-z]{3,}$)");

    // for each letter in the alphabet,
    for (char l = 'A'; l <= 'Z'; l++) {
        // construct a filename for that letter, yielding "data/A Words.txt", etc.
        std::string filename = "data/" + std::string(1, l) + " Words.txt";

        // open the file
        std::ifstream file(filename);

        // if the file fails to open, print an error and continue to the next letter
        if (!file.is_open())
        {
            std::cerr << "Unable to open file " << filename << std::endl;
            continue;
        }

        // read each line, filter valid words, and add them to the vector
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream stream(line);
            std::string word;
            while (stream >> word)
            {
                if (std::regex_match(word, filter))
                {
                    words.push_back(word);
                }
            }
        }

        // close the file before moving to the next letter
        file.close();
    }

    // DEBUG: to test smaller input manually
    // words = {"app", "apple", "applet", "banana", "band", "bat", "bake", "base", "bass"};

    // print total number of valid words loaded
    std::cout << "Words Size: " << words.size() << std::endl << std::endl;

    // initialize the trie
    Trie myTrie;

    // insert each word into the trie
    for (const std::string& word : words)
    {
        myTrie.insert(word);
    }

    // prompt user for prefix input
    std::cout << "Enter a (partial) lower-case word: ";
    std::string prefix;
    std::cin >> prefix;
    std::cout << std::endl;

    // find autocomplete options for the prefix
    std::queue<std::string> acOptions = myTrie.findAutoCompleteOptions(prefix);

    // variables for cycling options
    std::string option = "";
    std::string entry = "";

    // autocomplete cycling interface
    while ((acOptions.empty() == false) && (option != "e"))
    {
        // display the current suggestion
        entry = acOptions.front();
        std::cout << "Current Autocomplete Option: " << entry << std::endl << std::endl;

        // prompt user for input
        std::cout << "Enter (e) to lock in the word or (t) to cycle to next word: ";
        std::cin >> option;

        // handle user input
        if (option == "e")
        {
            std::cout << entry << " entered." << std::endl;
        }
        else if (option == "t" && acOptions.size() == 1)
        {
            std::cout << "Out of words." << std::endl;
        }
        else if (option == "t")
        {
            // move to next option automatically in next loop iteration
        }
        else
        {
            std::cout << "Invalid entry. Try again." << std::endl;
            continue;
        }

        // remove the displayed option from the queue
        acOptions.pop();
    }

    // if no suggestions were found
    if (acOptions.empty() && option != "e")
    {
        std::cout << "No matching strings found." << std::endl;
    }

    return 0;
}
