/**
 * @file OutputWriter.cpp
 * @brief Implementation of OutputWriter - writes results to the output file.
 *
 * This file is part of Mafalda's work.
 * It takes the AllocationResult and writes it in the exact format
 * specified in Figures 11 and 12 of the project spec.
 *
 * ============================================================
 * OUTPUT FORMAT - FEASIBLE ALLOCATION (Figure 11)
 * ============================================================
 *
 * # Total number of webs followed by the listing of the program points of each one
 * # program points in each web are sorted in ascending order
 * webs: 4
 * web0: 1+,2,3,4,5,6-
 * web1: 9+,10,11,12-,20+
 * web1: 12+,13,14-
 * web2: 7+,8,9,10-
 * # Total number of registers used, followed by assignment to webs
 * registers: 2
 * r0: web0
 * r0: web1
 * r0: web2
 * r1: web3
 *
 * KEY FORMATTING RULES:
 *   - "webs: N" where N is the total number of webs.
 *   - For each web, list its line points SORTED IN ASCENDING ORDER by line number.
 *   - A web's points that span multiple original ranges are listed on one line,
 *     all merged together and sorted.
 *   - "registers: N" where N is the number of registers actually USED.
 *   - For each register rK, list all webs assigned to it: "rK: webM".
 *   - Multiple webs can share the same register (if they don't interfere).
 *
 * ============================================================
 * OUTPUT FORMAT - INFEASIBLE ALLOCATION (Figure 12)
 * ============================================================
 *
 * webs: 3
 * web0: 1+,2,3,4,5,6-,9-,10,11,12-,20+
 * web1: 12+,13,14-
 * web2: 7+,8,9,10-
 * registers: 0
 * M: web0
 * M: web1
 * M: web2
 *
 * KEY RULES:
 *   - "registers: 0" (zero registers used).
 *   - Every web is assigned to memory: "M: webN".
 *   - Also print a WARNING to std::cerr:
 *     "WARNING: Register allocation was not possible with N registers."
 */

#include "OutputWriter.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>

// -----------------------------------------------------------------------------
// void OutputWriter::write(const AllocationResult& result, const std::string& filename)
// -----------------------------------------------------------------------------
// Writes the full output file.
//
// HIGH-LEVEL STEPS:
//   1. Open the output file for writing. Throw if it fails.
//   2. Write the comment header lines (lines starting with '#').
//   3. Write "webs: N" where N = result.webs.size().
//   4. For each web in result.webs:
//      a. Call getSortedPoints() to get all points sorted by line number.
//      b. Call formatWebPoints() to get the formatted string "1+,2,3,4,5,6-".
//      c. Write "webN: <formatted points>" to the file.
//   5. Write the register section:
//      a. If feasible (result.feasible == true):
//         - Write "registers: K" where K = result.registersUsed.
//         - For each register color 0..K-1:
//           For each web assigned to that register:
//             Write "rK: webN"
//      b. If infeasible (result.feasible == false):
//         - Write "registers: 0"
//         - For each web, write "M: webN"
//         - Print warning to std::cerr
//
// TODO: implement step by step
void OutputWriter::write(const AllocationResult& result, const std::string& filename) {
    // TODO: open the file
    // TODO: write header comments
    // TODO: write webs section (call formatWebPoints for each web)
    // TODO: write registers section (feasible or infeasible format)
    // TODO: if infeasible, print warning to std::cerr
}

// -----------------------------------------------------------------------------
// std::string OutputWriter::formatWebPoints(const Web& web) const
// -----------------------------------------------------------------------------
// Converts a web's sorted line points into the output string format.
//
// OUTPUT FORMAT:
//   Points are comma-separated, sorted by line number (ascending).
//   Each point is a number optionally followed by '+' or '-':
//     isDef=true     -> append '+'
//     isLastUse=true -> append '-'
//     neither        -> just the number
//
// EXAMPLE:
//   Points: [{1,true,false},{2,false,false},{3,false,false},{4,false,false},{5,false,false},{6,false,true}]
//   Output: "1+,2,3,4,5,6-"
//
// STEPS:
//   1. Call web.getSortedPoints() to get sorted points.
//   2. For each point, build the token: std::to_string(point.line) + suffix.
//   3. Join all tokens with ',' separator.
//   4. Return the resulting string.
//
// TODO: iterate over sorted points
// TODO: build each token with the correct suffix ('+', '-', or "")
// TODO: join with commas and return
std::string OutputWriter::formatWebPoints(const Web& web) const {
    // TODO: implement
    return "";
}
