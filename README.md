<H2> <p> COP3530 - Project 2 - Trie/B+ Tree Autocomplete </p> By Group 136 (Megan Enochs, Lukas Zhukauskas) </H2> 
	
Link to Original Project 2 Template: https://github.com/COP3530/P2-template/blob/main/README.md 

<h2> Table of Contents </h2>

- [Goals, Problem, Features, Motivation, & Responsibility](#goals-problem-features-motivation--responsibility)
	- [Dataset](#dataset)
    - Implementation Complexity
    	- [Trie Tree](#trie-complexity)
     	- [B+ Tree](#b-tree-complexity)
	- [Testing Instructions](#testing-instructions)
    - [References](#references) 

# Goals, Problem, Features, Motivation, & Responsibility
By implementing an autocomplete algorithm, we aim to improve typing efficiency and reduce typos, thereby reducing problematic incidents of miscommunication and lost productivity. 
By using a large data set of English words and two different data structures, Tries and B+ trees, we will create a command-line interface where partial word entries can be autocompleted to the whole word nearest to a typed entry and further word option selection. For example, if you enter “redu”, the nearest option would be “reduce” instead of “reduced” or “reducer” because ‘d’ is earlier in the alphabet compared to ‘r’.
Then, you can either select type ‘t’ (for tab) to cycle to the next nearest option through if it exists or type ‘e’ (for enter) to lock in the current option.

To implement the data structures and command-line interface, we will use the CLion IDE, the C++ programming language, and some C++ standard libraries, including the iostream, vector, string, map, queue, stack, regex, fstream, and sstream libraries. Then, once implemented, we aim to calculate and compare the asymptotic complexities of each implementation, allowing us to ascertain which implementation is more efficient.

Megan Enochs was responsible for the Trie tree implementation, handling the integration and merging both implementations into the main command-line interface, finishing and improving the user I/O, and creating the readme instructions and presentation. Lukas Zhukauskas was responsible for data set parsing, the B+ tree implementation, initial implementation of the user I/O, GitHub description, and writing this report. Both Megan and Lukas were responsible for debugging, complexity analysis, and proofreading and adding our reflections to this report.

## Dataset
<p> Our data will be derived from the A-Z text files of English words from this GitHub repository: <br>
https://github.com/kloge/The-English-Open-Word-List/tree/master/EOWL%20LF%20Delimited%20Format

We will be using the English Open Word List (EOWL) data set, which is published by Ken Loge (@kloge on GitHub) and almost entirely derived from the “UK Advanced Cryptics Dictionary” (UKACD) Version 1.6, by J. Ross Beresford. The EOWL contains 128,985 English words. We will utilize the LF delimited format, where words are separated into 26 different text files according to the starting letter of the word. In each text file, the words are each on their own line. Then, we will utilize a regex filter to eliminate all words that do not entirely consist of lower-case alphabetical characters and/or do not have at least 3 letters, yielding a final total of 128,600 English words.

## Trie Complexity
TBD

## B+ Tree Complexity
In the context of the underlying B+ tree implementation, the key operations are the insertion of a large set of words into the tree, searching for the node that contains the nearest complete word to the search query, and gathering the range of valid autocomplete options into a vector. The insertion occurs at startup after selecting the B+ tree implementation. Then, when a user inputs an initial (partial) word, searching and gathering occurs.

In the case of insertion, several functions were implemented, including splitNode, insertIntoExisting, and the main insert function. The Big O time complexity for insertion is O(log(n)), where n is the total number of nodes in the tree. This is because a properly functioning B+ implementation should be balanced, thereby yielding log(n) height.

For search, findAutoCompleteOptions and findNearestNode are used. findAutoCompleteOptions calls findNearestNode, a typical B+ tree search function, which has a Big O time complexity of O(log(n)), where n is the total number of nodes in the tree. Once it finds the nearest node, it iterates through the node’s key vector, which has a maximum of 1 less than the tree’s order keys (a constant), adding each valid key to a queue in constant time. If it reaches the end of the key vector and the last key was a valid autocomplete option, it will move to the next node in the linked list of the B+ tree leaf nodes in constant time as well. In the scenario that the initial search query is “a” and every word in the tree starts with a, all nodes of the tree would be added to the queue, yielding a worse case time complexity of O(n). Thus, after eliminating the lower order term, the worst case time complexity for search is O(n), where n is the total number of nodes in the tree.

Additionally, the class destructor and a level order key print function for debugging both have a big O time complexity of O(n), where n is the total number of nodes in the tree.

Notice that, in this particular application with this data set, the number of nodes is 128,600 and the order 220. Also, since the words (nodes) have a wide distribution of starting letters, the worse-case gathering scenario is impossible. Also, the height of the tree would be 3, drastically reducing costs related to the height of the tree.


## Testing Instructions
If testing B+ Tree implementation in CLion, keep the .cpp and .h files in the project folder. Then, place the data folder containing all of the "_ Words.txt" files in the cmake-build-debug folder.
Then,...

## References
BPT References (Used in the research and understanding of B+ Trees)</p>
     1. [Edugator "9.6 B/B+ Trees" Lesson](https://edugator.app/courses/2eff7d57-3f4b-4297-ac6c-0bb5ce67348b/lesson/d54ad28e-5b7f-4cbd-91ae-87172912aa79) </p>
     2. [Open-DSA "17.6 B Trees"](https://opendsa-server.cs.vt.edu/OpenDSA/Books/Everything/html/BTree.html#id2) </p>
     3. Weiss "4.7 B-Trees" [Page 168 of Data Structures and Algorithm Analysis in C++, 4th Edition] </p>
     4. [B-Tree Wikipedia](https://en.wikipedia.org/wiki/B%2B_tree) </p>
     5. [B+ Tree Visualizer](https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html) </p>

