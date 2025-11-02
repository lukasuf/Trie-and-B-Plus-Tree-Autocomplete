//
// Created by mbrit on 10/26/2025.
//
#include "Trie.h"

// constructor: initializes the root node of the trie
Trie::Trie()
{
    this->root = new Node();
}

// destructor: clears all nodes from memory
Trie::~Trie()
{
    clear(root);
}

// recursively deletes all nodes in the trie
void Trie::clear(Node* currentNode)
{
    if (currentNode == nullptr)
    {
        return;
    }

    // iterate through each child node and clear it
    for (auto& pair : currentNode->children)
    {
        clear(pair.second);
    }

    // delete the current node
    delete currentNode;
}

// inserts a single word into the trie
void Trie::insert(const std::string& word)
{
    Node* currentNode = this->root;

    // traverse through each character in the word
    for (char c : word)
    {
        // if the character doesn't exist yet, create a new node
        if (currentNode->children.find(c) == currentNode->children.end())
        {
            currentNode->children[c] = new Node();
        }
        // move to the next node
        currentNode = currentNode->children[c];
    }

    // mark the end of the word
    currentNode->isEndOfWord = true;
}

// returns true if the word exists in the trie
bool Trie::contains(const std::string& word) const
{
    const Node* currentNode = this->root;

    // check each character sequentially
    for (char c : word)
    {
        auto it = currentNode->children.find(c);
        if (it == currentNode->children.end())
        {
            return false;
        }
        currentNode = it->second;
    }

    // the word exists only if the last node marks the end of a word
    return currentNode->isEndOfWord;
}

// helper function that recursively collects all words starting from a given node
// since the children are stored in a map, this traversal automatically visits
// characters in alphabetical order, so the results are naturally sorted
void Trie::collectWords(Node* currentNode, const std::string& prefix, std::vector<std::string>& results)
{
    if (currentNode == nullptr)
    {
        return;
    }

    // if this node marks the end of a valid word, add the prefix to results
    if (currentNode->isEndOfWord)
    {
        results.push_back(prefix);
    }

    // recursively explore all child nodes (in lexicographic order)
    for (auto& pair : currentNode->children)
    {
        collectWords(pair.second, prefix + pair.first, results);
    }
}

// finds and returns a queue of autocomplete options sorted alphabetically
std::queue<std::string> Trie::findAutoCompleteOptions(const std::string& prefix) {
    const Node* currentNode = this->root;

    // traverse the trie to reach the node corresponding to the prefix
    for (char c : prefix)
    {
        auto it = currentNode->children.find(c);
        if (it == currentNode->children.end())
        {
            std::cout << "Not a real word with no ending." << std::endl;
            return {};
        }
        currentNode = it->second;
    }

    // collect all words that start with the prefix
    std::vector<std::string> words;
    collectWords(const_cast<Node*>(currentNode), prefix, words);

    // remove the prefix itself from results
    if (!words.empty() && words.front() == prefix)
    {
        words.erase(words.begin());
    }

    // now decide which message to show if nothing remains
    if (words.empty())
    {
        if (currentNode->isEndOfWord)
        {
            // the prefix is a real word but has no longer completions
            std::cout << "No possible endings." << std::endl;
        }
        else
        {
            // the prefix path exists but isn’t a full word and has no children
            std::cout << "Word not found, no possible endings" << std::endl;
        }
        return {};
    }

    // move results into a standard queue for easier cycling in main
    std::queue<std::string> autoCompleteOptions;
    for (const std::string& word : words)
    {
        autoCompleteOptions.push(word);
    }
    return autoCompleteOptions;
}
