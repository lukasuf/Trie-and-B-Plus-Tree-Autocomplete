#include "BPT.h"

// B+ Tree Constructor
BPT::BPT(const size_t& order)
{
    // Start with empty leaf node rather than nullptr for easier logic.
    this->root = new Node(true);
    this->order = order;
}

// B+ Tree Destructor
// Iterative Stack Approach:
// Push Node to Stack > If leafNode, delete > If not, collect its children, then delete.
// Once finished, set root to nullptr.
BPT::~BPT()
{
    if (this->root != nullptr)
    {
        return;
    }

    std::stack<Node*> myStack;
    myStack.push(this->root);

    while (!myStack.empty())
    {
        Node* current = myStack.top();
        myStack.pop();

        if (current->leafNode)
        {
            delete current;
        }
        else
        {
            for (Node* child : current->children)
            {
                myStack.push(child);
            }
            delete current;
        }
    }

    root = nullptr;
}

// main() insertion: hides Node structure and handles non-splitting insertion
void BPT::insert(const std::string& key)
{
    Node* currentNode = root;

    // Search for the correct insertion point
    // While the current node is neither null nor a leaf,
    while ((currentNode != nullptr) && (currentNode->leafNode == false))
    {
        int i = 0;
        // iterate through the keys of the current node until the prefix is less than the ith key.
        while (i < currentNode->keys.size() && key >= currentNode->keys[i])
        {
            i++;
        }
        // This should yield the index of the child that has keys that are either greater than or equal to the key.
        // So, update the current node to the child at that index.
        currentNode = currentNode->children[i];
    }

    // Start at the first element in the keys vector of the currentNode
    auto iter = currentNode->keys.begin();
    // While there are still more keys and the current key is less than the insertion key,
    while ((iter != currentNode->keys.end()) && (*iter < key))
    {
        // increment the iterator and index.
        iter++;
    }

    // Then, insert key at proper node.
    currentNode->keys.insert(iter, key);

    // After insertion, if the current number of keys violates maximum key rule,
    if (currentNode->keys.size() >= order)
    {
        // we must split it.
        splitNode(currentNode);
    }
}

// Iterative Level Order Key Print
void BPT::printBPT()
{
    // If empty tree, just return.
    if (this->root == nullptr)
    {
        return;
    }

    // Declare queue and push root to it.
    std::queue<Node*> myQ;
    myQ.push(this->root);

    // While queue is not empty,
    while (!myQ.empty())
    {
        // iterate through each node on a level,
        int size = myQ.size();
        for (size_t i = 0; i < size; i++)
        {
            // printing each key in a node in-between <>.
            Node* currentNode = myQ.front();
            myQ.pop();
            std::cout << "<";
            for (size_t j = 0; j < currentNode->keys.size(); j++)
            {
                std::cout << currentNode->keys[j];

                // If it is not a last key, add space. <- <app apple> instead of <app apple >
                if (j != currentNode->keys.size() - 1)
                {
                    std::cout << " ";
                }
            }
            std::cout << "> ";

            // If the current node is not a leaf, add its children to the queue
            if (!currentNode->leafNode)
            {
                for (Node* child : currentNode->children)
                {
                    myQ.push(child);
                }
            }
        }

        // Put each level on its own line.
        std::cout << std::endl;
    }
}
