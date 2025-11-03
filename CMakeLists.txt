cmake_minimum_required(VERSION 3.16)
project(TrieAndBPlusTreeAutocomplete)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# B+ Tree main executable
add_executable(bpt_demo
        mainBPT.cpp
        BPT.cpp
)

# Copy the data folder after building so the program can read text files
add_custom_command(TARGET bpt_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:bpt_demo>/data)

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

# Combined autocomplete demo (Trie + B+ Tree)
add_executable(complete_demo
        mainCombined.cpp
        Trie.cpp
        BPT.cpp
        mainCombined.cpp
)

# Copy the data folder for combined demo as well
add_custom_command(TARGET complete_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:complete_demo>/data)