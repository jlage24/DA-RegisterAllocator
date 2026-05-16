#include <iostream>
#include <fstream>
#include "../src/parser/Parser.h"
#include "../src/graph/WebBuilder.h"
#include "../src/graph/InterferenceGraphBuilder.h"
#include "../src/allocation/RegisterAllocator.h"
#include "../src/utils/OutputWriter.h"

static bool coloringValid(const InterferenceGraph &ig) {
    for (auto v : ig.getVertexSet()) {
        int id = v->getInfo();
        int ca = ig.webs.at(id).color;
        if (ca == COLOR_SPILLED || ca == COLOR_NONE) continue;
        for (auto &e : v->getAdj()) {
            int nb = e.getDest()->getInfo();
            int cb = ig.webs.at(nb).color;
            if (cb == COLOR_SPILLED || cb == COLOR_NONE) continue;
            if (ca == cb) {
                std::cerr << "  CONFLICT: web" << id << " and web" << nb << " both r" << ca << "\n";
                return false;
            }
        }
    }
    return true;
}

static bool runBasic(const std::string &rangesFile,
                     const std::string &registersFile,
                     const std::string &label,
                     int expectedWebs, int expectedRegs) {
    std::cout << "[" << label << "]\n";
    auto ranges = Parser::parseLiveRanges(rangesFile);
    auto config = Parser::parseRegisters(registersFile);
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);
    RegisterAllocator::allocateBasic(ig, config.numRegisters);
    OutputWriter::printToConsole(ig);

    bool validColoring = coloringValid(ig);
    bool correctWebs   = ((int)webs.size() == expectedWebs);

    // Count registers actually used
    int maxColor = -1;
    for (auto &[id, web] : ig.webs)
        if (web.color >= 0 && web.color > maxColor) maxColor = web.color;
    bool correctRegs = (maxColor + 1 <= expectedRegs);

    bool pass = validColoring && correctWebs && correctRegs;
    std::cout << "  Webs=" << webs.size() << " (expected " << expectedWebs << ")"
              << "  Regs used=" << (maxColor+1) << " (expected <=" << expectedRegs << ")"
              << "  Valid=" << (validColoring?"YES":"NO")
              << "  " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

int main() {
    std::cout << "===== Basic Allocation Tests =====\n\n";
    int passed = 0, total = 0;
    auto run = [&](bool r) { total++; if (r) passed++; };

    // All 6 cases from the teacher's README
    run(runBasic("../data/ranges/ranges1.txt", "../data/registers/registers2.txt", "ranges1+reg2", 3, 2));
    run(runBasic("../data/ranges/ranges2.txt", "../data/registers/registers2.txt", "ranges2+reg2", 2, 2));
    run(runBasic("../data/ranges/ranges3.txt", "../data/registers/registers2.txt", "ranges3+reg2", 2, 2));
    run(runBasic("../data/ranges/ranges4.txt", "../data/registers/registers1.txt", "ranges4+reg1", 4, 1));
    run(runBasic("../data/ranges/ranges5.txt", "../data/registers/registers1.txt", "ranges5+reg1", 3, 1));
    run(runBasic("../data/ranges/ranges6.txt", "../data/registers/registers3.txt", "ranges6+reg3", 5, 3));

    std::cout << "==================================\n";
    std::cout << "Results: " << passed << "/" << total << " tests passed\n";
    return (passed == total) ? 0 : 1;
}