<H2> <p> COP3530 - Project 2 - Trie/B+ Tree Autocomplete </p> By Group 136 (Megan Enochs, Lukas Zhukauskas) </H2> 
	
Link to Original Project 2 Template: https://github.com/COP3530/P2-template/blob/main/README.md 

<h2> Table of Contents </h2>

- [Goals, Problem, Features, Motivation, & Responsibility](#goals-problem-features-motivation--responsibility)
	- [Dataset](#dataset)
    - Implementation Complexity
    	- [Trie Tree](#trie-complexity)
     	- [B+ Tree](#b-tree-complexity)
      	- [Comparison](#complexity-comparison) 
	- [Testing Instructions](#testing-instructions)
    - [References](#references) 

# Goals, Problem, Features, Motivation, & Responsibility
By implementing an autocomplete algorithm, we aim to improve typing efficiency and reduce typos, thereby reducing problematic incidents of miscommunication and lost productivity. 
By using a large data set of English words and two different data structures, Tries and B+ trees, we will create a command-line interface where partial word entries can be autocompleted to the whole word nearest to a typed entry and further word option selection. For example, if you enter “redu”, the nearest option would be “reduce” instead of “reduced” or “reducer” because ‘d’ is earlier in the alphabet compared to ‘r’.
Then, you can either select type ‘t’ (for tab) to cycle to the next nearest option through if it exists or type ‘e’ (for enter) to lock in the current option.
NOTE: Entering "reduce" will not find "reduce", it will skip "reduce" and go to "reduced", as the program assumes you are not trying to autocomplete an already complete word.

To implement the data structures and command-line interface, we will use the CLion IDE, the C++ programming language, and some C++ standard libraries, including the iostream, vector, string, map, queue, stack, regex, fstream, sstream, and chrono libraries. Then, once implemented, we aim to calculate and compare the asymptotic complexities of each implementation, allowing us to ascertain which implementation is more efficient.

Megan Enochs was responsible for the Trie tree implementation, handling the integration and merging both implementations into the main command-line interface, finishing and improving the user I/O, creating the readme instructions, and creating the presentation, and implementing the chrono library tracking. Lukas Zhukauskas was responsible for data set acquisition, parsing, and filtering, the B+ tree implementation, initial implementation of the user I/O, writing the GitHub description as a whole, and writing this report. Both Megan and Lukas were responsible for debugging the code, conducting complexity analysis, adding our reflections to this report, and proofreading or reviewing this report and the presentation.

## Dataset
<p> Our data will be derived from the A-Z text files of English words from this GitHub repository: <br>
https://github.com/kloge/The-English-Open-Word-List/tree/master/EOWL%20LF%20Delimited%20Format

We will be using the English Open Word List (EOWL) data set, which is published by Ken Loge (@kloge on GitHub) and almost entirely derived from the “UK Advanced Cryptics Dictionary” (UKACD) Version 1.6, by J. Ross Beresford. The EOWL contains 128,985 English words. We will utilize the LF delimited format, where words are separated into 26 different text files according to the starting letter of the word. In each text file, the words are each on their own line. Then, we will utilize a regex filter to eliminate all words that do not entirely consist of lower-case alphabetical characters and/or do not have at least 3 letters, yielding a final total of 128,600 English words.

UK Advanced Cryptics Dictionary Licensing Information:
Copyright © J Ross Beresford 1993-1999. All Rights Reserved. The following restriction is placed on the use of this publication: if the UK Advanced Cryptics Dictionary is used in a software package or redistributed in any form, the copyright notice must be prominently displayed and the text of this document must be included verbatim.

## Trie Complexity
The Trie in this project stores words one character at a time, where each node represents a single letter in a word. The main operations are inserting all words from the dataset, checking if a specific word exists, and finding autocomplete suggestions that begin with a given prefix.

For insertion, the algorithm moves through the Trie one letter at a time. If a node for that letter doesn’t exist, it creates a new one. Once all letters in the word are processed, the final node is marked as the end of a valid word. Because this process touches one node per character, inserting a single word of length m takes O(m) time. When building the full dictionary of n words, the total time is O(n × m). The Trie’s height is determined by the longest word in the dataset, not the total number of words, which keeps insertions efficient even as the vocabulary grows.

For searching and autocomplete, the algorithm first walks down the Trie following the prefix. This initial traversal takes O(m) time, where m is the number of characters in the prefix. Once it reaches the correct node, the helper function collectWords() explores all possible word endings below it. The time required for this step depends on how many valid completions exist. In the worst case, if the prefix is very common, like “a”, it may visit every word that starts with that letter, giving a complexity of about O(m + k), where k is the number of matching results.

The space complexity of a Trie is proportional to the number of nodes, which in turn depends on the total number of unique character combinations across all words. In the worst case, where no words share prefixes, the space cost is O(n × m). However, because many words reuse the same initial letters (“read,” “ready,” “real”), much of the structure is shared, so the actual space used is usually much lower in practice.

## B+ Tree Complexity
In the context of the underlying B+ tree implementation, the key operations are the insertion of a large set of words into the tree, searching for the node that contains the nearest complete word to the search query, and gathering the range of valid autocomplete options into a vector. The insertion occurs at startup after selecting the B+ tree implementation. Then, when a user inputs an initial (partial) word, searching and gathering occurs.
In the case of insertion, several functions were implemented, including splitNode, insertIntoExisting, and the main insert function. The Big O time complexity for insertion is O(log(n)), where n is the total number of nodes in the tree. This is because a properly functioning B+ implementation should be balanced, thereby yielding log(n) height.

For search, findAutoCompleteOptions and findNearestNode are used. findAutoCompleteOptions calls findNearestNode, a typical B+ tree search function, which has a Big O time complexity of O(log(n)), where n is the total number of nodes in the tree. Once it finds the nearest node, it iterates through the node’s key vector, which has a maximum of 1 less than the tree’s order keys (a constant), adding each valid key to a queue in constant time. If it reaches the end of the key vector and the last key was a valid autocomplete option, it will move to the next node in the linked list of the B+ tree leaf nodes in constant time as well. In the scenario that the initial search query is “a” and every word in the tree starts with a, all nodes of the tree would be added to the queue, yielding a worse case time complexity of O(n). Thus, after eliminating the lower order term, the worst case time complexity for search is O(n), where n is the total number of nodes in the tree.

Additionally, the class destructor and a level order key print function for debugging both have a big O time complexity of O(n), where n is the total number of nodes in the tree. Lastly, the space complexity for the B+ tree should be O(n), where n is the total number of nodes in the tree.

Notice that, in this particular application with this data set, the number of words is 128,600 and the order 220, which yields fewer than 600 actual nodes. Also, since the words have a wide distribution of starting letters, the worse-case gathering scenario is impossible. Also, the height of the tree would be 3, drastically reducing costs related to the height of the tree. Thus, the actual time complexities of the B+ tree for this specific application are much better than the worst cases.

## Complexity Comparison
Both structures produced accurate autocomplete results, but their performance differed slightly. In testing, the Trie built its structure faster because it only needed to process characters one by one, while the B+ Tree had to manage node splitting and key promotion during insertion. However, when it came to individual query time, the B+ Tree was slightly faster, since it performs logarithmic-time lookups across already balanced nodes.

This means that for large-scale data loading or frequent insertions, the Trie is more efficient, while for repeated queries on a static dataset, the B+ Tree offers slightly quicker response times. The Trie still maintains an advantage in simplicity and predictable performance when working with prefixes and partial words.

## Testing Instructions
To run this project, open CLion. Click File at the top, then click Open. Find the folder where you saved or downloaded this project and open it. Make sure to trust the project so that the IDE works, and ensure that the following files are inside: BPT.cpp, BPT.h, Trie.cpp, Trie.h, mainTrie.cpp, mainBPT.cpp, mainCombined.cpp, CMakeList.txt, and the folder called data that holds all the word files from A to Z. Once the project is open, look at the top right of CLion for a box that says trie_demo, bpt_demo, or combined_demo. Pick which one you want to run, then click the green Run or Build button. The program will load all the words and ask you to type a partial word. Note: If you type in "star" in hopes of adding "star", it will treat "star" as a prefix and ignore it, jumping directly to "starboard". Press 't' to cycle through more word suggestions (if they exist) or press 'e' to lock the current one in. If you run the combined version, you can also add words manually by pressing 's' and entering the desired entry. Moreover, in combined mode, you can save words to a text file and compare how fast the Trie and B+ Tree implementations perform initial building and average query functions. When finished, type 0 to exit the program. Also, note that the text file containing any saved words will be in the cmake-build-debug folder.

## References
0. [Dataset](https://github.com/kloge/The-English-Open-Word-List/tree/master/EOWL%20LF%20Delimited%20Format)
1. [Edugator "9.6 B/B+ Trees" Lesson](https://edugator.app/courses/2eff7d57-3f4b-4297-ac6c-0bb5ce67348b/lesson/d54ad28e-5b7f-4cbd-91ae-87172912aa79) </p>
2. [Open-DSA "17.6 B Trees"](https://opendsa-server.cs.vt.edu/OpenDSA/Books/Everything/html/BTree.html#id2) </p>
3. Weiss "4.7 B-Trees" [Page 168 of Data Structures and Algorithm Analysis in C++, 4th Edition] </p>
4. [B-Tree Wikipedia](https://en.wikipedia.org/wiki/B%2B_tree) </p>
5. [B+ Tree Visualizer](https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html) </p>
6. [C++ Timer](https://www.delftstack.com/howto/cpp/cpp-timer/)</p>
7. [Edugator "8.3 Tries" Lesson](https://edugator.app/courses/2eff7d57-3f4b-4297-ac6c-0bb5ce67348b/lesson/280c65a3-4c2e-45d9-837b-74125bee8519)</p>


UK Advanced Cryptics Dictionary Licensing Information:
Copyright © J Ross Beresford 1993-1999. All Rights Reserved. The following restriction is placed on the use of this publication: if the UK Advanced Cryptics Dictionary is used in a software package or redistributed in any form, the copyright notice must be prominently displayed and the text of this document must be included verbatim.
