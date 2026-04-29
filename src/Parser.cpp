/**
 * @file Parser.cpp
 * @brief Implementation of the Parser - reads and interprets all input files.
 *
 * This is one of the most important files for Mafalda's part.
 * It reads the raw text files and converts them into usable C++ data structures.
 *
 * ============================================================
 * FILE 1: RANGES FILE  (e.g., ranges.txt)
 * ============================================================
 * Each non-comment line has the format:
 *     varName: point1, point2, point3, ...
 *
 * Where each point is a line number optionally followed by '+' or '-':
 *     "7+"   -> line 7, isDef = true  (variable is DEFINED/written here)
 *     "10-"  -> line 10, isLastUse = true  (variable's last use/read here)
 *     "8"    -> line 8, no marker  (variable is alive here, neither def nor last use)
 *
 * Lines starting with '#' are comments -> SKIP THEM.
 * Empty lines -> SKIP THEM.
 *
 * Example input:
 *     # comment
 *     sum: 7+,8,9,10-
 *     i: 1+,2,3,4,5,6-
 *     i: 9+,10,11,12-
 *
 * Expected output (map):
 *     "sum" -> [ LiveRange{ points: [{7,true,false},{8,false,false},{9,false,false},{10,false,true}] } ]
 *     "i"   -> [ LiveRange{ points: [1+,2,3,4,5,6-] },
 *                LiveRange{ points: [9+,10,11,12-] } ]
 *
 * ============================================================
 * FILE 2: CONFIG FILE  (e.g., registers.txt)
 * ============================================================
 * Format:
 *     registers: N          -> N is an integer (number of physical registers)
 *     algorithm: basic      -> use the basic greedy coloring
 *     algorithm: spilling,2 -> use spilling with K=2
 *     algorithm: splitting,3-> use splitting with K=3
 *     algorithm: free       -> use the custom algorithm
 *
 * Lines starting with '#' are comments -> SKIP THEM.
 */

#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// -----------------------------------------------------------------------------
// std::map<std::string, std::vector<LiveRange>>
// Parser::parseRangesFile(const std::string& filename)
// -----------------------------------------------------------------------------
// Opens the ranges file and builds a map: varName -> list of LiveRanges.
//
// HIGH-LEVEL STEPS:
//   1. Open the file. If it fails, throw a std::runtime_error with the filename.
//   2. Read line by line using std::getline.
//   3. Skip lines that are empty or start with '#'.
//   4. For each valid line:
//      a. Find the ':' separator -> everything before it is the variable name.
//      b. Everything after ':' is a comma-separated list of line points.
//      c. Split by ',' to get individual tokens like "7+", "8", "10-".
//      d. Call parseLinePoint() on each token to get a LinePoint struct.
//      e. Build a LiveRange from these points, set its varName.
//      f. Add the LiveRange to the map under that variable name.
//   5. Return the completed map.
//
// EDGE CASES TO HANDLE:
//   - Extra whitespace around the variable name or point tokens (use trim()).
//   - Variable name appearing multiple times -> append to its vector, don't overwrite.
//
// TODO: implement step by step following the description above
std::map<std::string, std::vector<LiveRange>> Parser::parseRangesFile(const std::string& filename) {
    std::map<std::string, std::vector<LiveRange>> result;

    // TODO: open file, check for errors
    // TODO: read line by line
    // TODO: skip comments and empty lines
    // TODO: parse varName and list of points
    // TODO: build LiveRange and add to map

    return result;
}

// -----------------------------------------------------------------------------
// AllocConfig Parser::parseConfigFile(const std::string& filename)
// -----------------------------------------------------------------------------
// Opens the config file and extracts the number of registers and algorithm type.
//
// HIGH-LEVEL STEPS:
//   1. Open the file. If it fails, throw a std::runtime_error.
//   2. Read line by line, skip comments and empty lines.
//   3. For the "registers: N" line -> parse N as an integer.
//   4. For the "algorithm: X" line:
//      a. If X is "basic" -> AlgorithmType::BASIC, param = 0
//      b. If X starts with "spilling" -> AlgorithmType::SPILLING
//         Extract K from "spilling, K" (split by ',')
//      c. If X starts with "splitting" -> AlgorithmType::SPLITTING
//         Extract K from "splitting, K"
//      d. If X is "free" -> AlgorithmType::FREE, param = 0
//   5. Return the filled AllocConfig struct.
//
// TODO: implement step by step following the description above
AllocConfig Parser::parseConfigFile(const std::string& filename) {
    AllocConfig config;
    config.numRegisters = 0;
    config.algorithm = AlgorithmType::BASIC;
    config.algorithmParam = 0;

    // TODO: open file, check for errors
    // TODO: read line by line, skip comments
    // TODO: parse "registers: N"
    // TODO: parse "algorithm: X" with optional ",K"

    return config;
}

// -----------------------------------------------------------------------------
// LinePoint Parser::parseLinePoint(const std::string& token)
// -----------------------------------------------------------------------------
// Parses a single token from the ranges file into a LinePoint struct.
//
// A token is a string like:
//   "7+"  -> LinePoint{ line=7, isDef=true,  isLastUse=false }
//   "10-" -> LinePoint{ line=10, isDef=false, isLastUse=true  }
//   "8"   -> LinePoint{ line=8,  isDef=false, isLastUse=false }
//
// STEPS:
//   1. Trim whitespace from the token.
//   2. Check if the last character is '+' or '-'.
//   3. If '+': set isDef=true, remove the '+' before converting to int.
//   4. If '-': set isLastUse=true, remove the '-' before converting to int.
//   5. Convert the remaining string to int using std::stoi.
//   6. Return the filled LinePoint.
//
// TODO: implement step by step
LinePoint Parser::parseLinePoint(const std::string& token) {
    LinePoint lp;
    lp.isDef = false;
    lp.isLastUse = false;
    lp.line = -1;

    // TODO: trim the token
    // TODO: check last character for '+' or '-'
    // TODO: extract line number with std::stoi

    return lp;
}

// -----------------------------------------------------------------------------
// std::string Parser::trim(const std::string& s)
// -----------------------------------------------------------------------------
// Removes leading and trailing whitespace (spaces, tabs) from a string.
//
// This is needed because input lines may have extra spaces around tokens,
// e.g., "  i : 1+ , 2 , 3- " should parse correctly.
//
// STEPS:
//   1. Find the first non-whitespace character (std::string::find_first_not_of).
//   2. Find the last non-whitespace character (std::string::find_last_not_of).
//   3. Return the substring between those positions.
//   4. If the string is all whitespace, return "".
//
// TODO: implement using find_first_not_of and find_last_not_of
std::string Parser::trim(const std::string& s) {
    // TODO: implement
    return s;
}
