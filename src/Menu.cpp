/**
 * @file Menu.cpp
 * @brief Implementation of the interactive command-line menu (T1.1).
 *
 * This file is part of Mafalda's work.
 * It provides the user-facing interface when the program runs WITHOUT "-b".
 *
 * ============================================================
 * MENU FLOW
 * ============================================================
 * The menu is stateful: the user must complete steps in order.
 * Internal state tracks what has been loaded/built so far:
 *
 *   State 0: Nothing loaded yet.
 *   State 1: Ranges file loaded (rangesByVar is populated).
 *   State 2: Config file loaded (config is populated).
 *   State 3: Webs built (webs list is populated).
 *   State 4: Graph built (graph is constructed).
 *   State 5: Allocation run (result is populated).
 *
 * ============================================================
 * MENU OPTIONS
 * ============================================================
 *   [1] Load ranges file
 *       -> Prompt: "Enter path to ranges file: "
 *       -> Call Parser::parseRangesFile(filename)
 *       -> On success: print "Loaded X variables with Y live ranges."
 *       -> On failure: print error to std::cerr, don't change state
 *
 *   [2] Load config file
 *       -> Prompt: "Enter path to config file: "
 *       -> Call Parser::parseConfigFile(filename)
 *       -> On success: print "Config loaded: N registers, algorithm = X"
 *       -> On failure: print error to std::cerr
 *
 *   [3] Build webs
 *       -> Requires state >= 1 (ranges loaded)
 *       -> Call WebBuilder::buildWebs(rangesByVar)
 *       -> Print each web: "web0 (var=i): lines 1,2,3,4,5,6"
 *
 *   [4] Build interference graph
 *       -> Requires state >= 3 (webs built)
 *       -> Call InterferenceGraphBuilder::build(webs)
 *       -> Print edges: "web0 -- web1 (interfere)"
 *
 *   [5] Run allocation algorithm
 *       -> Requires state >= 4 (graph built) and state >= 2 (config loaded)
 *       -> Call RegisterAllocator::allocate(rangesByVar, config)
 *       -> Print result: register assignments or "INFEASIBLE"
 *
 *   [6] Save output to file
 *       -> Requires state >= 5 (allocation done)
 *       -> Prompt: "Enter output filename: "
 *       -> Call OutputWriter::write(result, filename)
 *       -> Print: "Output written to <filename>"
 *
 *   [0] Exit
 *       -> Print "Goodbye." and return
 *
 * ============================================================
 * ERROR HANDLING
 * ============================================================
 *   - If user tries to skip a step, print:
 *     "Please complete step X first."
 *   - If user enters an invalid menu option, print:
 *     "Invalid option. Please enter a number between 0 and 6."
 *   - All error messages go to std::cerr.
 *   - The menu loop continues after any error (don't exit on error).
 */

#include "Menu.h"
#include "Parser.h"
#include "WebBuilder.h"
#include "InterferenceGraphBuilder.h"
#include "RegisterAllocator.h"
#include "OutputWriter.h"
#include <iostream>
#include <string>

// -----------------------------------------------------------------------------
// void Menu::show()
// -----------------------------------------------------------------------------
// Main loop. Displays the menu and dispatches user choices until exit.
//
// STEPS:
//   1. Loop forever (until user selects 0).
//   2. Print the menu options.
//   3. Read an integer from std::cin.
//   4. Dispatch to the correct private method.
//   5. Handle invalid input (non-integer or out-of-range).
//
// TODO: implement the main loop with switch/case dispatch
void Menu::show() {
    // TODO: implement the loop
    // TODO: print menu header
    // TODO: read user choice
    // TODO: switch/case: 0=exit, 1=loadRangesFile, 2=loadConfigFile,
    //        3=buildWebs, 4=buildGraph, 5=runAllocation, 6=saveOutput
    // TODO: handle invalid input
}

// -----------------------------------------------------------------------------
// void Menu::loadRangesFile()
// -----------------------------------------------------------------------------
// Prompts the user for a filename and calls the parser.
//
// TODO: std::cout << "Enter path to ranges file: ";
// TODO: read filename from std::cin
// TODO: try { rangesByVar = parser.parseRangesFile(filename); }
// TODO: catch exceptions, print to std::cerr
// TODO: on success, print summary (number of variables loaded)
void Menu::loadRangesFile() {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Menu::loadConfigFile()
// -----------------------------------------------------------------------------
// Prompts the user for a config filename and calls the parser.
//
// TODO: std::cout << "Enter path to config file: ";
// TODO: read filename, call parser.parseConfigFile(filename)
// TODO: on success, print: "Registers: N, Algorithm: X"
void Menu::loadConfigFile() {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Menu::buildWebs()
// -----------------------------------------------------------------------------
// Builds webs from the loaded live ranges.
// Must check that ranges have been loaded first (state >= 1).
//
// TODO: check rangesByVar is not empty, else print error
// TODO: call WebBuilder::buildWebs(rangesByVar)
// TODO: print a summary: number of webs, each web's variable and lines
void Menu::buildWebs() {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Menu::buildGraph()
// -----------------------------------------------------------------------------
// Builds the interference graph from the webs.
// Must check that webs have been built first (state >= 3).
//
// TODO: check webs list is not empty, else print error
// TODO: call InterferenceGraphBuilder::build(webs)
// TODO: print the edges of the graph for the user to inspect
void Menu::buildGraph() {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Menu::runAllocation()
// -----------------------------------------------------------------------------
// Runs the selected allocation algorithm.
// Must check that graph and config are ready (state >= 4 and >= 2).
//
// TODO: check prerequisites, else print error
// TODO: call RegisterAllocator::allocate(rangesByVar, config)
// TODO: print the result (register assignments or INFEASIBLE message)
// TODO: if infeasible, also print warning to std::cerr
void Menu::runAllocation() {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Menu::saveOutput()
// -----------------------------------------------------------------------------
// Saves the allocation result to a file chosen by the user.
// Must check that allocation has been run (state >= 5).
//
// TODO: check result is available, else print error
// TODO: prompt for output filename
// TODO: call OutputWriter::write(result, filename)
// TODO: confirm success to the user
void Menu::saveOutput() {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Menu::printCurrentState() const
// -----------------------------------------------------------------------------
// Prints a summary of what has been loaded/built so far.
// Useful for debugging and for showing the user their progress.
//
// Example output:
//   === Current State ===
//   Ranges file:  LOADED (3 variables)
//   Config file:  LOADED (2 registers, algorithm=basic)
//   Webs:         BUILT  (4 webs)
//   Graph:        NOT BUILT
//   Allocation:   NOT RUN
//
// TODO: print each state line based on which members are populated
void Menu::printCurrentState() const {
    // TODO: implement
}
