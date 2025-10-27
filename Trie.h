//
// Created by mbrit on 10/26/2025.
//
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>

/*
Trie class

This class implements an autocomplete system using a trie (prefix tree)
data structure. Each node represents a single character, and paths from
the root to terminal nodes form complete words. The trie supports fast
insertion, lookup, and prefix-based search operations.

In this project, the trie is used to generate autocomplete suggestions
based on a user-provided prefix. When a partial word is entered, the
trie traverses character-by-character to the nearest matching node and
collects all valid completions that extend from that prefix.
*/

class Trie
{
public:
    struct Node
    {
        // marks the end of a valid word
        bool isEndOfWord;

        // stores children nodes for each character
        std::map<char, Node*> children;

        Node()
        {
            this->isEndOfWord = false;
        }
    };

private:
    // pointer to the root node of the trie
    Node* root;

    // recursively collects all words starting from a given node
    void collectWords(Node* currentNode, const std::string& prefix, std::vector<std::string>& results);


    // clears the trie from memory
    void clear(Node* currentNode);

public:
    Trie();
    ~Trie();

    // inserts a single word into the trie
    void insert(const std::string& word);

    // returns true if a word exists in the trie
    bool contains(const std::string& word) const;

    // returns a queue of autocomplete options sorted alphabetically
    std::queue<std::string> findAutoCompleteOptions(const std::string& prefix);
};
