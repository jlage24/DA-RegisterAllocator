/**
 * @file InterferenceGraphBuilder.cpp
 * @brief Implementation of InterferenceGraphBuilder (João's part).
 *
 * Takes the list of Webs from WebBuilder and constructs the interference graph.
 *
 * ============================================================
 * INTERFERENCE RULE (from project spec, section 2.5)
 * ============================================================
 * Two webs A and B INTERFERE (edge exists) if there is at least one
 * program point (line number) where BOTH webs are alive at the same time.
 *
 * EXCEPTION - NO interference at line X if:
 *   Web A has a DEFINITION ('+') at line X   --> A is being written
 *   Web B has a LAST USE   ('-') at line X   --> B's value just finished being read
 *   (or vice versa: B has '+' and A has '-')
 *
 * Why? Because at the exact moment A is being written, B is no longer needed.
 * There is no point in time where BOTH values need to be in registers simultaneously.
 *
 * ============================================================
 * ALGORITHM
 * ============================================================
 * For every pair of webs (i, j) where i < j:
 *   1. Find all line numbers shared between web[i].allLines and web[j].allLines.
 *   2. For each shared line X:
 *      a. Check if web[i] has '+' at X AND web[j] has '-' at X -> NO interference at X
 *      b. Check if web[j] has '+' at X AND web[i] has '-' at X -> NO interference at X
 *      c. Otherwise -> INTERFERENCE EXISTS -> add edge and stop checking more lines
 *   3. If no interfering line found after checking all shared lines -> no edge.
 *
 * Time complexity: O(W^2 * L) where W = number of webs, L = average lines per web.
 */

#include "InterferenceGraphBuilder.h"

// -----------------------------------------------------------------------------
// Graph InterferenceGraphBuilder::build(const std::vector<Web>& webs)
// -----------------------------------------------------------------------------
// Constructs and returns the complete interference graph.
//
// STEPS:
//   1. Create a new Graph object.
//   2. Add each web as a node: graph.addNode(web).
//   3. For every pair (i, j) with i < j:
//      a. Call interferes(webs[i], webs[j]).
//      b. If true, call graph.addEdge(webs[i].id, webs[j].id).
//   4. Return the graph.
//
// TODO: implement
Graph InterferenceGraphBuilder::build(const std::vector<Web>& webs) {
    Graph graph;

    // TODO: add all webs as nodes
    // TODO: check every pair for interference
    // TODO: add edges where interference exists

    return graph;
}

// -----------------------------------------------------------------------------
// bool InterferenceGraphBuilder::interferes(const Web& a, const Web& b) const
// -----------------------------------------------------------------------------
// Returns true if webs a and b interfere (should have an edge between them).
//
// STEPS:
//   1. Find shared lines: iterate over a.allLines, check if each is in b.allLines.
//   2. For each shared line X:
//      a. Find the LinePoint for X in web a (search a's ranges for a point with line==X).
//      b. Find the LinePoint for X in web b (search b's ranges).
//      c. Apply the exception check:
//         - If a's point is isDef ('+') AND b's point is isLastUse ('-') -> skip (no interf.)
//         - If b's point is isDef ('+') AND a's point is isLastUse ('-') -> skip (no interf.)
//      d. If neither exception applies -> return true (they interfere).
//   3. If we checked all shared lines and all were exceptions -> return false.
//
// HELPER: to find the LinePoint for a given line in a web, search through
// all ranges of the web looking for a point where point.line == X.
// If the line has no special marker (neither isDef nor isLastUse), that's fine -
// it's just a "plain alive" point, and it DOES cause interference.
//
// TODO: implement step by step
bool InterferenceGraphBuilder::interferes(const Web& a, const Web& b) const {
    // TODO: find shared lines between a.allLines and b.allLines
    // TODO: for each shared line, check the def/lastuse exception
    // TODO: return true if any shared line causes interference
    return false;
}
