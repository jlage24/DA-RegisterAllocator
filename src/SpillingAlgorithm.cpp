/**
 * @file SpillingAlgorithm.cpp
 * @brief Implementation of register allocation with web spilling (T2.2, João's part).
 *
 * Used when "algorithm: spilling, K" is in the config file.
 *
 * ============================================================
 * WHAT IS WEB SPILLING?
 * ============================================================
 * When the basic greedy coloring fails (too many interferences for N registers),
 * we can "spill" a web to memory. This means:
 *   - The web's node and ALL its edges are removed from the interference graph.
 *   - The web gets assignedRegister = -1 (marked as "M" in output).
 *   - With fewer nodes/edges, the remaining graph may now be colorable.
 *
 * This simulates what a real compiler does: instead of keeping the variable
 * in a register, it stores it in RAM and loads it when needed.
 *
 * ============================================================
 * ALGORITHM
 * ============================================================
 *   1. Try basic coloring with N registers.
 *   2. If it succeeds -> done (0 spills needed).
 *   3. If it fails:
 *      a. Select the best candidate to spill (selectSpillCandidate).
 *      b. Permanently remove that web's node from the graph.
 *      c. Mark it as spilled (assignedRegister = -1).
 *      d. Retry coloring.
 *   4. Repeat steps 3a-3d up to K times.
 *   5. If still failing after K spills -> return false (infeasible).
 *
 * ============================================================
 * SPILL CANDIDATE SELECTION STRATEGY
 * ============================================================
 * Goal: choose the web whose removal most helps the remaining graph.
 *
 * HEURISTIC: Choose the web with the HIGHEST DEGREE.
 *   Rationale: removing the most-connected node removes the most edges,
 *   which most reduces the degrees of other nodes -> easier to color.
 *
 * TIE-BREAKING: If two nodes have the same degree, prefer the one
 * with the LARGEST allLines set (widest live range).
 * Rationale: a wider live range is more "expensive" in a register anyway.
 *
 * Document your chosen strategy in your presentation (the spec requires it).
 */

#include "SpillingAlgorithm.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// bool SpillingAlgorithm::allocate(Graph& graph, int N, int K)
// -----------------------------------------------------------------------------
// Tries to allocate registers with up to K spills.
//
// STEPS:
//   1. Try GraphColoringAlgorithm::colorGraph(graph, N).
//   2. If it returns true -> return true (no spills needed).
//   3. Loop up to K times:
//      a. Call selectSpillCandidate(graph) to get the node ID to spill.
//      b. Set graph.getWeb(id).assignedRegister = -1.
//      c. Physically remove the node (or permanently disable it).
//      d. Try colorGraph again on the reduced graph.
//      e. If it returns true -> return true.
//   4. After K iterations, if still failing -> return false.
//
// TODO: implement
bool SpillingAlgorithm::allocate(Graph& graph, int N, int K) {
    // TODO: try basic coloring first
    // TODO: loop K times: select spill candidate, remove, retry coloring
    // TODO: return true if coloring eventually succeeds, false otherwise
    return false;
}

// -----------------------------------------------------------------------------
// int SpillingAlgorithm::selectSpillCandidate(const Graph& graph) const
// -----------------------------------------------------------------------------
// Returns the ID of the best node to spill next.
//
// STEPS:
//   1. Get all active nodes from graph.getActiveNodes().
//   2. Find the one with the highest degree (graph.degree(id)).
//   3. In case of tie, pick the one with the largest web.allLines.size().
//   4. Return its ID.
//
// TODO: implement
int SpillingAlgorithm::selectSpillCandidate(const Graph& graph) const {
    // TODO: implement
    return -1;
}
