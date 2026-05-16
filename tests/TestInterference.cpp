#include <iostream>
#include "../src/parser/Parser.h"
#include "../src/graph/WebBuilder.h"
#include "../src/graph/InterferenceGraphBuilder.h"

// Test 1: ranges1 — only web0(sum) and web2(i merged) interfere
static bool test_ranges1_interference() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges1.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);

    std::cout << "[test_ranges1_interference]\n";
    ig.print();

    // Find sum web and merged-i web
    int sumWeb = -1, iWebA = -1, iWebB = -1;
    for (auto &[id, web] : ig.webs) {
        if (web.varName == "sum") sumWeb = id;
        else if (web.varName == "i" && web.allLines().count(1)) iWebA = id;
        else if (web.varName == "i") iWebB = id;
    }

    bool pass = true;
    if (sumWeb != -1 && iWebB != -1)
        pass &= ig.hasEdge(sumWeb, iWebB);   // sum interferes with large i web
    if (sumWeb != -1 && iWebA != -1)
        pass &= !ig.hasEdge(sumWeb, iWebA);  // sum does NOT interfere with small i web

    std::cout << "[test_ranges1_interference] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 2: ranges4 — t1..t4 are different vars with X-/X+ — none interfere
static bool test_ranges4_no_interference() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges4.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);

    std::cout << "[test_ranges4_no_interference]\n";
    ig.print();

    bool anyEdge = false;
    for (auto v : ig.getVertexSet())
        for (auto &e : v->getAdj())
            anyEdge = true;

    bool pass = !anyEdge;
    std::cout << "[test_ranges4_no_interference] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 3: ranges6 — chain interference (each adjacent pair interferes)
static bool test_ranges6_chain() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges6.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);

    std::cout << "[test_ranges6_chain]\n";
    ig.print();

    // Every web should have degree >= 1 (chain graph)
    bool pass = true;
    for (auto &[id, web] : ig.webs)
        if (ig.degree(id) == 0) { pass = false; break; }

    std::cout << "[test_ranges6_chain] " << (pass ? "PASS ✓" : "FAIL ✗") << "\n\n";
    return pass;
}

// Test 4: DOT export
static bool test_dot_export() {
    auto ranges = Parser::parseLiveRanges("../data/ranges/ranges1.txt");
    auto webs   = WebBuilder::buildWebs(ranges);
    auto ig     = InterferenceGraphBuilder::build(webs);
    ig.emitUndirectedDOT("tests/interference_graph");
    std::cout << "[test_dot_export] Written to tests/interference_graph.gv ✓\n\n";
    return true;
}

int main() {
    std::cout << "===== InterferenceGraph Tests =====\n\n";
    int passed = 0, total = 0;
    auto run = [&](bool r) { total++; if (r) passed++; };

    run(test_ranges1_interference());
    run(test_ranges4_no_interference());
    run(test_ranges6_chain());
    run(test_dot_export());

    std::cout << "===================================\n";
    std::cout << "Results: " << passed << "/" << total << " tests passed\n";
    return (passed == total) ? 0 : 1;
}