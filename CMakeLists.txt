cmake_minimum_required(VERSION 3.16)
project(TrieAndBPlusTreeAutocomplete)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# B+ Tree main executable
add_executable(autocomplete
        mainBPT.cpp
        BPT.cpp
        Trie.cpp
)

# Copy the data folder after building so the program can read text files
add_custom_command(TARGET autocomplete POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:autocomplete>/data)

# Trie demo (for independent testing)
add_executable(trie_demo
        mainTrie.cpp
        Trie.cpp
)

# copy data folder for the Trie demo
add_custom_command(TARGET trie_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:trie_demo>/data)