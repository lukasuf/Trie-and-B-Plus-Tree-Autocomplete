#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include <stack>

class BPT
{
public:
    struct Node
    {
        //Leaf Node Status
        bool leafNode;
        //For Non-Leaf Nodes
        std::vector<Node*> children;
        //For Leaf-Node Linked List
        Node* next;
        Node* prev;
        //For Both Types of Nodes
        Node* parent;
        std::vector<std::string> keys;
        //Node Constructor
        Node(bool leaf)
        {
            this->leafNode = leaf;
            this->next = nullptr;
            this->prev = nullptr;
            //Vectors are initialized already
        }
    };

private:
    Node* root;
    int order;

    // Handles Insertions that Cause Splitting
    void splitNode(Node* originalNode);

    // Handles Promotions from Splitting that Insert into an Existing Parent Node
    void insertIntoExisting(Node* originalNode, const std::string& key, Node* newNode);

    // Helper for Finding Nearest Node to Gather AutoComplete Options
    Node* findNearestNode(const std::string& prefix);

public:
    // B+ Tree Constructor
    BPT(const size_t& order);

    // B+ Tree Destructor
    ~BPT();

    // main() Insertion: Hides Node Structure and Handles Basic Insertion
    void insert(const std::string& key);

    // Iterative Level Order Key Print
    void printBPT();

    // Returns Queue of Strings Starting with the Prefix
    std::queue<std::string> findAutoCompleteOptions(const std::string& prefix);
};
