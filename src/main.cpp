#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include "parser/Parser.h"
#include "graph/LiveRange.h"
#include "graph/WebBuilder.h"
#include "graph/InterferenceGraphBuilder.h"
#include "allocation/RegisterAllocator.h"
#include "allocation/SpillAllocator.h"
#include "allocation/SplitAllocator.h"
#include "allocation/FreeAllocator.h"
#include "utils/OutputWriter.h"

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

static InterferenceGraph runAllocation(std::vector<Web> &webs,
                                        const RegisterConfig &config) {
    auto ig = InterferenceGraphBuilder::build(webs);
    if (config.algorithm == "basic") {
        RegisterAllocator::allocateBasic(ig, config.numRegisters);
        
        // If there was at least one spill, it failed. Send everything to memory.
        bool basicFailed = false;
        for (auto &[id, web] : ig.webs) {
            if (web.color == -2) { // COLOR_SPILLED
                basicFailed = true; break;
            }
        }
        
        if (basicFailed) {
            for (auto &[id, web] : ig.webs) {
                web.color = -2; // Force all to COLOR_SPILLED
                web.active = false;
            }
        }
    } else if (config.algorithm == "spilling") {
        SpillAllocator::allocate(ig, config.numRegisters, config.algorithmParam);
    } else if (config.algorithm == "splitting") {
        ig = SplitAllocator::allocate(webs, config.numRegisters, config.algorithmParam);
    } else if (config.algorithm == "free") {
        ig = FreeAllocator::allocate(webs, config.numRegisters);
    } else {
        throw std::runtime_error("Unknown algorithm: " + config.algorithm);
    }
    return ig;
}

static int runBatch(const std::string &rangesFile,
                    const std::string &registersFile,
                    const std::string &outputFile) {
    try {
        auto ranges = Parser::parseLiveRanges(rangesFile);
        auto config = Parser::parseRegisters(registersFile);
        auto webs   = WebBuilder::buildWebs(ranges);
        auto ig     = runAllocation(webs, config);
        OutputWriter::writeToFile(ig, outputFile);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
        return 1;
    }
}

static void runInteractive() {
    printBanner();
    std::vector<LiveRange> ranges;
    RegisterConfig config;
    std::vector<Web> webs;
    InterferenceGraph ig;
    bool rangesLoaded=false, configLoaded=false, websBuilt=false, graphBuilt=false, allocated=false;

    int choice = -1;
    while (choice != 0) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
            case 1: {
                std::string f; std::cout << "Live ranges file path: "; std::getline(std::cin, f);
                try { ranges=Parser::parseLiveRanges(f); rangesLoaded=true; websBuilt=graphBuilt=allocated=false;
                      std::cout << "Loaded " << ranges.size() << " range(s).\n";
                } catch(const std::exception &e){std::cerr<<"Error: "<<e.what()<<"\n";} break;
            }
            case 2: {
                std::string f; std::cout << "Registers config file path: "; std::getline(std::cin, f);
                try { config=Parser::parseRegisters(f); configLoaded=true;
                      std::cout<<"Registers: "<<config.numRegisters<<"  Algorithm: "<<config.algorithm;
                      if(config.algorithmParam>0) std::cout<<" (K="<<config.algorithmParam<<")";
                      std::cout<<"\n";
                } catch(const std::exception &e){std::cerr<<"Error: "<<e.what()<<"\n";} break;
            }
            case 3: {
                if(!rangesLoaded){std::cerr<<"Load live ranges first.\n";break;}
                webs=WebBuilder::buildWebs(ranges); websBuilt=true; graphBuilt=allocated=false;
                std::cout<<"Built "<<webs.size()<<" web(s).\n";
                for(auto &w:webs){std::cout<<"  web"<<w.webId<<" ["<<w.varName<<"]: ";
                    for (int l : w.allLines()) {
                        std::cout << l << " ";
                    }
                    std::cout << "\n";
                } 
                break;
            }
            case 4: {
                if(!websBuilt){std::cerr<<"Build webs first.\n";break;}
                ig=InterferenceGraphBuilder::build(webs); graphBuilt=true; allocated=false;
                std::cout<<"Interference graph built.\n"; ig.print(); break;
            }
            case 5: {
                if(!graphBuilt){std::cerr<<"Build interference graph first.\n";break;}
                if(!configLoaded){std::cerr<<"Load registers config first.\n";break;}
                try { ig=runAllocation(webs,config); allocated=true;
                      std::cout<<"\n--- Allocation Result ---\n"; OutputWriter::printToConsole(ig);
                } catch(const std::exception &e){std::cerr<<"Error: "<<e.what()<<"\n";} break;
            }
            case 6: {
                if(!graphBuilt){std::cerr<<"Build interference graph first.\n";break;}
                ig.print(); break;
            }
            case 7: {
                if(!graphBuilt){std::cerr<<"Build interference graph first.\n";break;}
                std::string f; std::cout<<"DOT filename (no .gv): "; std::getline(std::cin,f);
                ig.emitUndirectedDOT(f); std::cout<<"Written to "<<f<<".gv\n"; break;
            }
            case 8: {
                if(!allocated){std::cerr<<"Run allocation first.\n";break;}
                std::string f; std::cout<<"Output file path: "; std::getline(std::cin,f);
                try{OutputWriter::writeToFile(ig,f);}catch(const std::exception &e){std::cerr<<"Error: "<<e.what()<<"\n";} break;
            }
            case 0: std::cout<<"Goodbye!\n"; break;
            default: std::cerr<<"Invalid option.\n";
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc==5 && std::string(argv[1])=="-b")
        return runBatch(argv[2],argv[3],argv[4]);
    if(argc>1){std::cerr<<"Usage: "<<argv[0]<<" [-b ranges.txt registers.txt output.txt]\n";return 1;}
    runInteractive();
    return 0;
}
