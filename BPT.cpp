#include "BPT.h"

// Handles Insertions that Cause Splitting
void BPT::splitNode(Node* originalNode)
{
    // Find the mid/split point
    int midPoint = (order + 1) / 2;

    // Leaf Node Case:
    if (originalNode->leafNode)
    {
        // Create a new leaf node and assign the keys at and after the midpoint of the original to it.
        Node* newLeafNode = new Node(true);
        newLeafNode->keys.assign(originalNode->keys.begin() + midPoint, originalNode->keys.end());
        // Then, efficiently resize the vector, deleting the duplicate keys through truncation.
        originalNode->keys.resize(midPoint);

        // Set the next pointer for the new leaf node equal to the next pointer of the original node.
        newLeafNode->next = originalNode->next;
        // If that next pointer points to an existing node, set its prev to the new leaf node.
        if (newLeafNode->next != nullptr)
        {
            newLeafNode->next->prev = newLeafNode;
        }

        // Then, set the original node's next pointer to the new leaf node, and the new leaf node's prev to the original.
        originalNode->next = newLeafNode;
        newLeafNode->prev = originalNode;

        // Set the parent of the new leaf node to the parent of the original leaf node.
        newLeafNode->parent = originalNode->parent;

        // If the original node is the root,
        if (originalNode == root)
        {
            // we need to create a new root Node that is not a leaf.
            Node* newRootNode = new Node(false);

            // Then, push the first key of the new leaf to the new root, and add both to the new root's children vector.
            newRootNode->keys.push_back(newLeafNode->keys[0]);
            newRootNode->children.push_back(originalNode);
            newRootNode->children.push_back(newLeafNode);

            // Next, update the parent pointers of the original and new leaf to point to the new root.
            originalNode->parent = newRootNode;
            newLeafNode->parent = newRootNode;

            // Lastly, set the root of the tree to the new root.
            this->root = newRootNode;
        }
        // If the original node isn't the root, we are promoting up into an existing node and need a recursion check.
        else
        {
            insertIntoExisting(originalNode, newLeafNode->keys[0], newLeafNode);
        }
    }
    // Internal Node Case:
    else
    {
        // Create a new internal node and assign the keys and corresponding children that come after the midpoint to it.
        Node* newInternalNode = new Node(false);
        newInternalNode->keys.assign(originalNode->keys.begin() + midPoint + 1, originalNode->keys.end());
        newInternalNode->children.assign(originalNode->children.begin() + midPoint + 1, originalNode->children.end());

        // For each child in the new internal node, update its parent pointer to the new internal node.
        for (Node*& child : newInternalNode->children)
        {
            child->parent = newInternalNode;
        }

        // Then, efficiently resize the key and children vectors of the original, deleting the duplicates through truncation.
        originalNode->keys.resize(midPoint);
        originalNode->children.resize(midPoint + 1);

        // Set the parent pointer of the new internal node to the parent of the original internal node.
        newInternalNode->parent = originalNode->parent;

        // If the original node is the root,
        if (originalNode == this->root)
        {
            // we need to create a new root node.
            Node* newRootNode = new Node(false);

            // Then, push the middle key to the new root, and add the original and new internal nodes to the new root's children vector.
            newRootNode->keys.push_back(originalNode->keys[midPoint]);
            newRootNode->children.push_back(originalNode);
            newRootNode->children.push_back(newInternalNode);

            // Next, update the parent pointers of the original and new leaf to point to the new root.
            originalNode->parent = newRootNode;
            newInternalNode->parent = newRootNode;

            //Lastly, set the root of the tree to the new root.
            this->root = newRootNode;
        }
        // If the original node is not the root, we are promoting up into a parent and need a recursion check.
        else
        {
            insertIntoExisting(originalNode, originalNode->keys[midPoint], newInternalNode);
        }
    }
}

// Handles Promotions from Splitting that Insert into an Existing Parent Node
void BPT::insertIntoExisting(Node* originalNode, const std::string& key, Node* newNode)
{
    // Get the parent of the original node
    Node* parentNode = originalNode->parent;

    // Iterate through the keys of the parentNode to find the proper insertion index for the promoted key.
    int i = 0;
    while (i < parentNode->keys.size() && key >= parentNode->keys[i])
    {
        i++;
    }
    // Then, insert it.
    parentNode->keys.insert(parentNode->keys.begin() + i, key);

    // Add the pointer to the new node to the proper insertion index (1 after the key's) of the parent's children vector.
    parentNode->children.insert(parentNode->children.begin() + i + 1, newNode);
    // Then, update the parent pointer of the new node to the parent node.
    newNode->parent = parentNode;

    // Check if the parent now exceeds max keys after insertion.
    if (parentNode->keys.size() >= order)
    {
        //If so, split it recursively.
        splitNode(parentNode);
    }
}

// Finds the node with key starting with the prefix
BPT::Node* BPT::findNearestNode(const std::string& prefix)
{
    // Start with the root.
    Node* currentNode = this->root;

    // While the current node is not null nor a leaf,
    while ((currentNode != nullptr) && (currentNode->leafNode == false))
    {
        int i = 0;
        // iterate through the keys of the current node until the prefix is less than the ith key.
        while (i < currentNode->keys.size() && prefix >= currentNode->keys[i])
        {
            i++;
        }
        // This should yield the index of the child that has keys that are either greater than or equal to the prefix.
        // So, update the current node to the child at that index.
        currentNode = currentNode->children[i];
    }

    //If currentNode is nullptr after the while loop, we want to return that. If not, that is good too.
    return currentNode;
}

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
    if (this->root == nullptr)
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

//Find and Return Queue of Autocomplete Options
std::queue<std::string> BPT::findAutoCompleteOptions(const std::string& prefix)
{
    Node* nearestNode = findNearestNode(prefix);

    if (nearestNode == nullptr || nearestNode->keys.empty()) {
        // no matching node or keys at all
        return {};
    }

    std::queue<std::string> autoCompleteOptions;

    if (nearestNode != nullptr)
    {
        Node* currentNode = nearestNode;
        bool more = true;
        while (more)
        {
            // Start at the first element in the keys vector of the currentNode
            auto iter = currentNode->keys.begin();
            size_t i = 0;
            // While there are still more keys and the current key is less than the prefix,
            while ((iter != currentNode->keys.end()) && (*iter < prefix))
            {
                // increment the iterator and index.
                iter++;
                i++;
            }
            // if this leaf has no keys >= prefix, decide whether to move or stop
            if (i >= currentNode->keys.size()) {
                if (currentNode->next == nullptr) {
                    // we're at the last leaf and still no keys >= prefix => no matches
                    break;  // exit the while(more) loop
                } else {
                    currentNode = currentNode->next;  // try the next leaf
                    continue;                         // restart with the next leaf
                }
            }
            // Print all matching keys in current node
            bool foundMatch = false;
            while (i < currentNode->keys.size())
            {
                const std::string& key = currentNode->keys[i];
                if (key.rfind(prefix, 0) == 0)  // true prefix match
                {
                    if (key != prefix)  // skip the prefix itself as a completion
                    {
                        autoCompleteOptions.push(key);
                    }
                    foundMatch = true;
                }
                else if (foundMatch)
                {
                    more = false;
                    break;
                }
                else if (key > prefix)
                {
                    more = false;
                    break;
                }
                i++;
            }
            if (more && currentNode->next != nullptr)
            {
                currentNode = currentNode->next;
            }
        }
    }
    else
    {
        return {};
    }

    return autoCompleteOptions;
}

