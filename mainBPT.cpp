#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include "BPT.h"


int main()
{
    std::vector<std::string> words = {"app", "appl", "ban", "baa", "bana", "basi", "best", "brea", "bat", "boss", "band", "cat", "catr", "dog", "dove"};
    //ACCORDING TO VISUALIZER, SHOULD MATCH   <basi>
    //                                        <baa bana> <best brea catr>
    //                                        <app appl> <baa ban> <bana band> <basi bat> <best boss> <brea cat> <catr dog dove>

    //Check words size
    std::cout << "Words Size: " << words.size() << std::endl << std::endl;

    //Initialize n-Order B+ Tree
    size_t n = 4;
    BPT myBPT(n);

    //Insert each word into the B+ Tree
    for (const std::string& word : words)
    {
        myBPT.insert(word);
    }

    //Check B+ tree structure
    std::cout << "B+ Tree structure:" << std::endl;
    myBPT.printBPT();

    return 0;
}

