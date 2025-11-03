#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include "BPT.h"

int main()
{
    std::vector<std::string> words;

    // Define the regex pattern for valid autocomplete options
    // [a-z] - alphabetical; {3,} - atleast 3 letters; ^/$ - together upgrades atleast to all letters
    std::regex filter("(^[a-z]{3,}$)");

    // For each letter in the alphabet,
    for (char l = 'A'; l <= 'Z'; l++) {
        // construct a filename for that letter _, yielding "_ Words.txt"
        std::string filename = "data/" + std::string(1, l) + " Words.txt";

        // Open the file at that filename
        std::ifstream file(filename);

        // If it did not open,
        if (!file.is_open())
        {
            // print error and continue to next letter
            std::cerr << "Unable to open file " << filename << std::endl;
            continue;
        }

        // If it did, put each line through the filter, adding valid words to words vector
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

        // Close file before exiting iteration
        file.close();
    }


    //DEBUG: Easier Structure Checking
    // words = {"app", "appl", "ban", "baa", "bana", "basi", "best", "brea", "bat", "boss", "band", "cat", "catr", "dog", "dove"};
    //ACCORDING TO VISUALIZER, SHOULD MATCH   <basi>
    //                                        <baa bana> <best brea catr>
    //                                        <app appl> <baa ban> <bana band> <basi bat> <best boss> <brea cat> <catr dog dove>


    //DEBUG: Check words size
    std::cout << "Words Size: " << words.size() << std::endl << std::endl;

    //Initialize n-Order B+ Tree
    size_t n = 220;
    BPT myBPT(n);

    //Insert each word into the B+ Tree
    for (const std::string& word : words)
    {
        myBPT.insert(word);
    }

    //DEBUG: Check B+ tree structure
    // std::cout << "B+ Tree structure:" << std::endl;
    // myBPT.printBPT();

    //"Prefix" Entry
    std::cout << "Enter a lower-case prefix query: ";
    std::string prefix;
    std::cin >> prefix;

    // check if prefix is a full word
    bool prefixExists = false;
    for (const std::string& w : words)
    {
        if (w == prefix)
        {
            prefixExists = true;
            break;
        }
    }

    //Find Auto Complete Options
    std::queue<std::string> acOptions = myBPT.findAutoCompleteOptions(prefix);

    // handle cases where the prefix produced no matches
    if (acOptions.empty()) {
        if (prefixExists) {
            std::cout << "\nNo possible endings.\n";
        } else {
            std::cout << "\nNot a real word with no endings.\n";
        }
        return 0;
    }

    // //Print All Options
    // std::cout << std::endl << "Auto-Complete Options Matching: '" << prefix << "':" << std::endl;
    // while (acOptions.empty() == false)
    // {
    //     std::cout << acOptions.front() << std::endl;
    //     acOptions.pop();
    // }

    std::string option = "";
    std::string entry = "";

    //AutoComplete Option Cycling / Interface
    while ((acOptions.empty() == false) && (option != "e"))
    {
        entry = acOptions.front();

        std::cout << "Current Autocomplete Option: " << entry << std::endl << std::endl;

        std::cout << "Enter (e) to lock in the word or (t) to cycle to next word: ";
        std::cin >> option;

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

        }
        else
        {
            std::cout << "Invalid entry. Try again." << std::endl;
            continue;
        }

        acOptions.pop();
    }

    return 0;
}
