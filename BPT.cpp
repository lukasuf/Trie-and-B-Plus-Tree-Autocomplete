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
