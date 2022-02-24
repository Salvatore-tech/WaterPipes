set(FIND_HASHING_STRATEGY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../Libraries/hashing_strategy)

find_path(HASHING_STRATEGY_INCLUDE_DIR HashingStrategy.h LinearProbingStrategy.h DoubleHashingStrategy.h
        PATH_SUFFIXES include
        PATHS ${FIND_HASHING_STRATEGY_PATHS})

find_library(HASHING_STRATEGY_LIBRARY               # The variable to store where it found the .a files
        NAMES hashing_strategy                      # The name of the .a file (without the extension and without the 'lib')
        PATH_SUFFIXES build                   # The folder the .a file is in
        PATHS ${FIND_HASHING_STRATEGY_PATHS})