/**
 * @file main.cpp
 * @brief Entry point of the Register Allocation tool.
 *
 * ============================================================
 * TWO EXECUTION MODES
 * ============================================================
 *
 * MODE 1 - BATCH MODE (flag: -b)
 *   Usage: ./regalloc -b ranges.txt registers.txt output.txt
 *   - Reads both input files.
 *   - Runs the allocation algorithm configured in registers.txt.
 *   - Writes the result to output.txt.
 *   - Prints errors/warnings to std::cerr (not stdout).
 *   - No user interaction, suitable for scripting.
 *
 * MODE 2 - INTERACTIVE MODE (no flags)
 *   Usage: ./regalloc
 *   - Shows a step-by-step menu (see Menu.cpp).
 *   - User loads files, builds structures, and runs algorithms manually.
 *   - Good for demos and debugging.
 *
 * ============================================================
 * ARGUMENT PARSING
 * ============================================================
 *   argc == 1              -> interactive mode
 *   argc == 5 and argv[1] == "-b" -> batch mode
 *     argv[2] = ranges file path
 *     argv[3] = registers/config file path
 *     argv[4] = output file path
 *   anything else          -> print usage and exit with error code 1
 *
 * ============================================================
 * ERROR HANDLING
 * ============================================================
 *   - File not found / unreadable -> print to std::cerr, exit(1)
 *   - Invalid file format         -> print to std::cerr, exit(1)
 *   - Allocation infeasible       -> print warning to std::cerr,
 *                                    still write the M-format output file,
 *                                    exit(0) (not an error, just a result)
 */

#include <iostream>
#include <string>
#include "Menu.h"
#include "Parser.h"
#include "RegisterAllocator.h"
#include "OutputWriter.h"

// -----------------------------------------------------------------------------
// void printUsage(const std::string& programName)
// -----------------------------------------------------------------------------
// Prints the correct usage of the program to std::cerr.
// Called when the user passes wrong arguments.
//
// TODO: print something like:
//   "Usage: ./regalloc [-b ranges.txt registers.txt output.txt]"
void printUsage(const std::string& programName) {
    std::cerr << "Usage: " << programName
              << " [-b ranges.txt registers.txt output.txt]" << std::endl;
}

// -----------------------------------------------------------------------------
// void runBatch(...)
// -----------------------------------------------------------------------------
// Runs the full pipeline in batch mode (no user interaction).
//
// STEPS:
//   1. Create a Parser and parse both input files.
//   2. Create a RegisterAllocator and run allocate().
//   3. Create an OutputWriter and write the result to the output file.
//   4. If infeasible, OutputWriter already prints the warning to std::cerr.
//
// TODO: implement
void runBatch(const std::string& rangesFile,
              const std::string& configFile,
              const std::string& outputFile)
{
    // TODO: parse rangesFile with Parser::parseRangesFile()
    // TODO: parse configFile with Parser::parseConfigFile()
    // TODO: run RegisterAllocator::allocate()
    // TODO: write result with OutputWriter::write()
}

// -----------------------------------------------------------------------------
// int main(int argc, char* argv[])
// -----------------------------------------------------------------------------
// Entry point: determines mode and dispatches accordingly.
//
// TODO: check argc
// TODO: if argc == 1 -> create Menu and call menu.show()
// TODO: if argc == 5 and argv[1] == "-b" -> call runBatch(argv[2], argv[3], argv[4])
// TODO: otherwise -> call printUsage and return 1
int main(int argc, char* argv[]) {

    if (argc == 1) {
        // TODO: interactive mode -> Menu::show()
        Menu menu;
        menu.show();

    } else if (argc == 5 && std::string(argv[1]) == "-b") {
        // TODO: batch mode -> runBatch
        runBatch(argv[2], argv[3], argv[4]);

    } else {
        // TODO: invalid arguments
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
