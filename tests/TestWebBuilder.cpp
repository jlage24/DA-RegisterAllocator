#include <iostream>
#include "../src/parser/Parser.h"
#include "../src/graph/WebBuilder.h"

static void printWeb(const Web &w) {
    std::cout << "  web" << w.webId << " [" << w.varName << "] — "
              << w.ranges.size() << " range(s), lines: { ";
    for (int l : w.allLines()) std::cout << l << " ";
    std::cout << "}\n";
}

// Test 1: assignment example — variable i with 4 ranges → 2 webs
static bool test_i_merging() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges1.txt");
    auto webs   = WebBuilder::buildWebs(ranges);

    std::cout << "[test_i_merging] Expected 3 webs, got " << webs.size() << "\n";
    for (auto &w : webs) printWeb(w);

    bool pass = (webs.size() == 3);
    std::cout << "[test_i_merging] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 2: ranges2 — x has 4 ranges all sharing lines → 1 web for x, 1 for y
static bool test_x_all_merge() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges2.txt");
    auto webs   = WebBuilder::buildWebs(ranges);

    std::cout << "[test_x_all_merge] Expected 2 webs, got " << webs.size() << "\n";
    for (auto &w : webs) printWeb(w);

    bool pass = (webs.size() == 2);
    std::cout << "[test_x_all_merge] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 3: different variables never merge (ranges4 — t1...t4)
static bool test_different_variables() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges4.txt");
    auto webs   = WebBuilder::buildWebs(ranges);

    std::cout << "[test_different_variables] Expected 4 webs, got " << webs.size() << "\n";
    for (auto &w : webs) printWeb(w);

    bool pass = (webs.size() == 4);
    std::cout << "[test_different_variables] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 4: special case X-/X+ — ranges5 x,y,z with 3-/3+ and 5-/5+
static bool test_special_case() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges5.txt");
    auto webs   = WebBuilder::buildWebs(ranges);

    std::cout << "[test_special_case X-/X+] Expected 3 webs (different vars), got " << webs.size() << "\n";
    for (auto &w : webs) printWeb(w);

    bool pass = (webs.size() == 3);
    std::cout << "[test_special_case X-/X+] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 5: chain merge — ranges3 has i with multiple '-' markers
static bool test_chain_merge() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges3.txt");
    auto webs   = WebBuilder::buildWebs(ranges);

    std::cout << "[test_chain_merge] Expected 2 webs, got " << webs.size() << "\n";
    for (auto &w : webs) printWeb(w);

    bool pass = (webs.size() == 2);
    std::cout << "[test_chain_merge] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

int main() {
    std::cout << "========== WebBuilder Tests ==========\n\n";
    int passed = 0, total = 0;
    auto run = [&](bool r) { total++; if (r) passed++; };

    run(test_i_merging());
    run(test_x_all_merge());
    run(test_different_variables());
    run(test_special_case());
    run(test_chain_merge());

    std::cout << "======================================\n";
    std::cout << "Results: " << passed << "/" << total << " tests passed\n";
    return (passed == total) ? 0 : 1;
}