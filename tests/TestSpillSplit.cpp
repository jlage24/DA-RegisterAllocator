#include <iostream>
#include <fstream>
#include "../src/parser/Parser.h"
#include "../src/graph/WebBuilder.h"
#include "../src/graph/InterferenceGraphBuilder.h"
#include "../src/allocation/RegisterAllocator.h"
#include "../src/allocation/SpillAllocator.h"
#include "../src/allocation/SplitAllocator.h"
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

// Test 1: spilling K=1, N=1 on ranges1
static bool test_spilling_ranges1() {
    std::cout << "[test_spilling ranges1 N=1 K=1]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges1.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);
    SpillAllocator::allocate(ig, 1, 1);
    OutputWriter::printToConsole(ig);
    bool pass = coloringValid(ig);
    std::cout << "  Valid: " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 2: spilling K=1, N=1 on ranges6 — tight graph
static bool test_spilling_ranges6() {
    std::cout << "[test_spilling ranges6 N=1 K=1]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);
    SpillAllocator::allocate(ig, 1, 1);
    OutputWriter::printToConsole(ig);
    bool pass = coloringValid(ig);
    std::cout << "  Valid: " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 3: splitting K=1, N=1 on ranges1
static bool test_splitting_ranges1() {
    std::cout << "[test_splitting ranges1 N=1 K=1]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges1.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = SplitAllocator::allocate(webs, 1, 1);
    OutputWriter::printToConsole(ig);
    bool pass = coloringValid(ig);
    std::cout << "  Webs after split: " << webs.size() << "\n";
    std::cout << "  Valid: " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 4: splitting K=1, N=2 on ranges6 — should fit with 2 regs after split
static bool test_splitting_ranges6() {
    std::cout << "[test_splitting ranges6 N=2 K=1]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = SplitAllocator::allocate(webs, 2, 1);
    OutputWriter::printToConsole(ig);
    bool pass = coloringValid(ig);
    std::cout << "  Webs after split: " << webs.size() << "\n";
    std::cout << "  Valid: " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 5: infeasibility — N=0 registers forces all to memory
static bool test_infeasible() {
    std::cout << "[test_infeasible N=1 all interfering]\n";
    // ranges6 with N=1 — impossible without spills
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);
    RegisterAllocator::allocateBasic(ig, 1);
    OutputWriter::printToConsole(ig);
    bool pass = coloringValid(ig);
    std::cout << "  Valid: " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

int main() {
    std::cout << "===== Spill & Split Tests =====\n\n";
    int passed = 0, total = 0;
    auto run = [&](bool r) { total++; if (r) passed++; };

    run(test_spilling_ranges1());
    run(test_spilling_ranges6());
    run(test_splitting_ranges1());
    run(test_splitting_ranges6());
    run(test_infeasible());

    std::cout << "===============================\n";
    std::cout << "Results: " << passed << "/" << total << " tests passed\n";
    return (passed == total) ? 0 : 1;
}