#include <iostream>
#include <string>
#include <stdexcept>
#include "parser/Parser.h"
#include "graph/LiveRange.h"

// Forward declarations (to be implemented)
// #include "allocation/AllocationEngine.h"
// #include "utils/OutputWriter.h"

static void printBanner() {
    std::cout << "=========================================\n";
    std::cout << "  DA2026 - Compiler Register Allocator   \n";
    std::cout << "=========================================\n\n";
}

static void printMenu() {
    std::cout << "\n--- MENU ---\n";
    std::cout << "1. Load live ranges file\n";
    std::cout << "2. Load registers configuration file\n";
    std::cout << "3. Build webs from live ranges\n";
    std::cout << "4. Build interference graph\n";
    std::cout << "5. Run register allocation\n";
    std::cout << "6. Show interference graph\n";
    std::cout << "7. Export interference graph (DOT)\n";
    std::cout << "8. Save allocation to output file\n";
    std::cout << "0. Exit\n";
    std::cout << "Option: ";
}

/**
 * @brief Batch mode: myProg -b ranges.txt registers.txt allocation.txt
 */
static int runBatch(const std::string &rangesFile,
                    const std::string &registersFile,
                    const std::string &outputFile) {
    try {
        std::cout << "[batch] Parsing live ranges from: " << rangesFile << "\n";
        auto ranges = Parser::parseLiveRanges(rangesFile);
        std::cout << "[batch] Loaded " << ranges.size() << " live range(s).\n";

        std::cout << "[batch] Parsing register config from: " << registersFile << "\n";
        auto config = Parser::parseRegisters(registersFile);
        std::cout << "[batch] Registers: " << config.numRegisters
                  << "  Algorithm: " << config.algorithm;
        if (config.algorithmParam > 0)
            std::cout << " (param=" << config.algorithmParam << ")";
        std::cout << "\n";

        // TODO: build webs, build interference graph, run allocation, write output
        std::cout << "[batch] Output will be written to: " << outputFile << "\n";
        std::cout << "[batch] (Allocation engine not yet implemented)\n";

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
        return 1;
    }
}

/**
 * @brief Interactive menu mode.
 */
static void runInteractive() {
    printBanner();

    std::vector<LiveRange> ranges;
    RegisterConfig config;
    bool rangesLoaded = false;
    bool configLoaded = false;

    int choice = -1;
    while (choice != 0) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string filename;
                std::cout << "Live ranges file path: ";
                std::getline(std::cin, filename);
                try {
                    ranges = Parser::parseLiveRanges(filename);
                    rangesLoaded = true;
                    std::cout << "Loaded " << ranges.size() << " live range(s).\n";
                    for (auto &r : ranges) {
                        std::cout << "  " << r.varName << ": ";
                        for (auto &p : r.points) {
                            std::cout << p.line;
                            if (p.marker != ' ') std::cout << p.marker;
                            std::cout << " ";
                        }
                        std::cout << "\n";
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 2: {
                std::string filename;
                std::cout << "Registers config file path: ";
                std::getline(std::cin, filename);
                try {
                    config = Parser::parseRegisters(filename);
                    configLoaded = true;
                    std::cout << "Registers: " << config.numRegisters << "\n";
                    std::cout << "Algorithm: " << config.algorithm;
                    if (config.algorithmParam > 0)
                        std::cout << " (K=" << config.algorithmParam << ")";
                    std::cout << "\n";
                } catch (const std::exception &e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 3:
                if (!rangesLoaded) { std::cerr << "Load live ranges first.\n"; break; }
                std::cout << "[TODO] Build webs -> coming soon.\n";
                break;
            case 4:
                std::cout << "[TODO] Build interference graph -> coming soon.\n";
                break;
            case 5:
                if (!rangesLoaded || !configLoaded) {
                    std::cerr << "Load both files first.\n"; break;
                }
                std::cout << "[TODO] Run allocation -> coming soon.\n";
                break;
            case 6:
                std::cout << "[TODO] Show interference graph -> coming soon.\n";
                break;
            case 7:
                std::cout << "[TODO] Export DOT file -> coming soon.\n";
                break;
            case 8:
                std::cout << "[TODO] Save output -> coming soon.\n";
                break;
            case 0:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cerr << "Invalid option.\n";
        }
    }
}

int main(int argc, char *argv[]) {
    // Batch mode: ./regalloc -b ranges.txt registers.txt output.txt
    if (argc == 5 && std::string(argv[1]) == "-b") {
        return runBatch(argv[2], argv[3], argv[4]);
    }

    if (argc > 1 && std::string(argv[1]) != "-b") {
        std::cerr << "Usage: " << argv[0] << " [-b ranges.txt registers.txt output.txt]\n";
        return 1;
    }

    runInteractive();
    return 0;
}