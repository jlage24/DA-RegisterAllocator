#include <iostream>
#include "../src/parser/Parser.h"
#include "../src/graph/WebBuilder.h"
#include "../src/graph/InterferenceGraphBuilder.h"
#include "../src/allocation/FreeAllocator.h"
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

// Test 1: N=2 on ranges1 — should fit with 0 spills
static bool test_free_ranges1_fits() {
    std::cout << "[test_free ranges1 N=2 — expect 0 spills]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges1.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = FreeAllocator::allocate(webs, 2);
    OutputWriter::printToConsole(ig);

    bool noSpill = true;
    for (auto &[id, web] : ig.webs)
        if (web.isSpilled()) { noSpill = false; break; }

    bool pass = coloringValid(ig) && noSpill;
    std::cout << "  NoSpill=" << (noSpill?"YES":"NO")
              << "  " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 2: N=2 on ranges6 — should fit with 0 spills
static bool test_free_ranges6_fits() {
    std::cout << "[test_free ranges6 N=2 — expect 0 spills]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = FreeAllocator::allocate(webs, 2);
    OutputWriter::printToConsole(ig);

    bool noSpill = true;
    for (auto &[id, web] : ig.webs)
        if (web.isSpilled()) { noSpill = false; break; }

    bool pass = coloringValid(ig) && noSpill;
    std::cout << "  NoSpill=" << (noSpill?"YES":"NO")
              << "  " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 3: N=1 on ranges6 — tight, free should do better than basic
static bool test_free_vs_basic_ranges6() {
    std::cout << "[test_free vs basic ranges6 N=1]\n";

    // Basic
    auto ranges1 = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs1   = WebBuilder::buildWebs(ranges1);
    auto ig1     = InterferenceGraphBuilder::build(webs1);
    RegisterAllocator::allocateBasic(ig1, 1);
    int basicSpills = 0;
    for (auto &[id, web] : ig1.webs) if (web.isSpilled()) basicSpills++;

    // Free
    auto ranges2 = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs2   = WebBuilder::buildWebs(ranges2);
    auto ig2     = FreeAllocator::allocate(webs2, 1);
    int freeSpills = 0;
    for (auto &[id, web] : ig2.webs) if (web.isSpilled()) freeSpills++;

    std::cout << "  Basic spills=" << basicSpills << "  Free spills=" << freeSpills << "\n";
    bool pass = coloringValid(ig2);
    std::cout << "  " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 4: N=3 on ranges6 — should color perfectly
static bool test_free_ranges6_perfect() {
    std::cout << "[test_free ranges6 N=3 — expect perfect coloring]\n";
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = FreeAllocator::allocate(webs, 3);
    OutputWriter::printToConsole(ig);

    bool noSpill = true;
    for (auto &[id, web] : ig.webs)
        if (web.isSpilled()) { noSpill = false; break; }

    bool pass = coloringValid(ig) && noSpill;
    std::cout << "  " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

int main() {
    std::cout << "===== FreeAllocator Tests =====\n\n";
    int passed = 0, total = 0;
    auto run = [&](bool r) { total++; if (r) passed++; };

    run(test_free_ranges1_fits());
    run(test_free_ranges6_fits());
    run(test_free_vs_basic_ranges6());
    run(test_free_ranges6_perfect());

    std::cout << "===============================\n";
    std::cout << "Results: " << passed << "/" << total << " tests passed\n";
    return (passed == total) ? 0 : 1;
}