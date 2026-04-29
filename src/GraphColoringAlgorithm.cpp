/**
 * @file GraphColoringAlgorithm.cpp
 * @brief Implementation of the greedy graph coloring algorithm (T2.1, João's part).
 *
 * This is the most important algorithm of the project.
 * It implements the pseudocode from Figure 9 of the project spec.
 *
 * ============================================================
 * ALGORITHM (Figure 9 from spec)
 * ============================================================
 *
 * PHASE 1 - Simplification:
 *   while graph is not empty:
 *     for all active nodes n:
 *       if degree(n) < N:
 *         disableNode(n)     <- soft remove from graph
 *         push n onto stack S
 *     if no node was pushed this iteration (all have degree >= N):
 *       select node K to spill (choose highest-degree node)
 *       disableNode(K)       <- remove from graph, do NOT push to stack
 *       mark K as spilled    <- K gets assignedRegister = -1
 *
 * PHASE 2 - Coloring:
 *   while stack S is not empty:
 *     n = pop from stack S
 *     enableNode(n)          <- re-activate in graph
 *     assign n the lowest color not used by any active neighbor of n
 *     (a valid color always exists because degree(n) was < N when pushed)
 *
 * ============================================================
 * COLOR ASSIGNMENT DETAIL
 * ============================================================
 * "Lowest color not used by neighbors":
 *   1. Collect the set of colors used by all active neighbors of n.
 *   2. Try color 0, 1, 2, ... until one is not in that set.
 *   3. Assign that color as n's register (web.assignedRegister = color).
 *
 * ============================================================
 * FINDING MINIMUM N
 * ============================================================
 * To find the minimum number of registers needed:
 *   - Try N = 1, 2, 3, ... up to the number of nodes in the graph.
 *   - For each N, run colorGraph on a FRESH COPY of the graph.
 *   - Return the first N where colorGraph returns true (no spills).
 */

#include "GraphColoringAlgorithm.h"
#include <stack>
#include <set>
#include <algorithm>

// -----------------------------------------------------------------------------
// bool GraphColoringAlgorithm::colorGraph(Graph& graph, int N)
// -----------------------------------------------------------------------------
// Tries to color the graph with at most N colors (registers).
// Returns true if coloring succeeded with NO spills.
//
// After this call, each web in the graph has:
//   assignedRegister >= 0  : colored successfully
//   assignedRegister == -1 : spilled to memory (if any spill happened)
//
// STEPS:
//   1. Create an empty stack S and a spilled set.
//   2. PHASE 1 loop (while graph is not empty):
//      a. Scan all active nodes.
//      b. If any node has degree < N -> disable it and push ID onto S.
//      c. If NO node was pushed (all degree >= N) -> select spill candidate,
//         disable it, mark its web.assignedRegister = -1.
//   3. PHASE 2 loop (while S is not empty):
//      a. Pop node ID from S.
//      b. Enable the node.
//      c. Collect colors of active neighbors.
//      d. Assign lowest color not in that set.
//      e. Set web.assignedRegister = chosen color.
//   4. Return true if no spills occurred, false otherwise.
//
// TODO: implement phase 1 and phase 2 as described
bool GraphColoringAlgorithm::colorGraph(Graph& graph, int N) {
    // TODO: implement phase 1 - simplification
    // TODO: implement phase 2 - coloring
    // TODO: return true if no node was spilled
    return false;
}

// -----------------------------------------------------------------------------
// int GraphColoringAlgorithm::findMinColors(Graph& graph)
// -----------------------------------------------------------------------------
// Finds the minimum number of registers (colors) needed to color the graph.
//
// STEPS:
//   1. Try N = 1 first.
//   2. Run colorGraph on a COPY of the graph (don't modify the original).
//   3. If it returns true -> return N.
//   4. Otherwise increment N and try again.
//   5. Upper bound: N = number of nodes (trivially colorable).
//
// IMPORTANT: Use a copy of the graph for each attempt, because colorGraph
// modifies the graph (disables/enables nodes, sets register assignments).
//
// TODO: implement the loop with copies
// TODO: return the minimum N found
int GraphColoringAlgorithm::findMinColors(Graph& graph) {
    // TODO: implement
    return -1;
}
