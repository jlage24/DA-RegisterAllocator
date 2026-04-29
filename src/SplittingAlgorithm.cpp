/**
 * @file SplittingAlgorithm.cpp
 * @brief Implementation of register allocation with web splitting (T2.3, João's part).
 *
 * Used when "algorithm: splitting, K" is in the config file.
 *
 * ============================================================
 * WHAT IS WEB SPLITTING?
 * ============================================================
 * Instead of entirely removing a web (spilling), we SPLIT it into two
 * smaller sub-webs. Each sub-web covers a portion of the original live range.
 *
 * Benefits over spilling:
 *   - The variable stays in a register for PART of its life.
 *   - Only at the split boundary does it need a load/store to memory.
 *   - Can result in better performance than full spilling.
 *
 * In a real compiler, the split boundary would require inserting
 * save/restore instructions (we note this but don't generate actual code).
 *
 * ============================================================
 * HOW TO SPLIT A WEB
 * ============================================================
 * Given a web W with sorted line points [p1, p2, ..., pN]:
 *   - Choose a cut index C (e.g., the midpoint, or the best split point).
 *   - Sub-web A = points [p1 .. pC]
 *   - Sub-web B = points [pC+1 .. pN]
 *   - Remove web W from the graph.
 *   - Add sub-web A and sub-web B as new nodes.
 *   - Recalculate edges for both sub-webs with all existing nodes.
 *
 * CHOOSING THE CUT POINT:
 *   A good heuristic: find the point that minimizes the sum of degrees
 *   of the two resulting sub-webs.
 *   Simple version: just split at the midpoint of the sorted line list.
 *
 * ============================================================
 * ALGORITHM
 * ============================================================
 *   1. Try basic coloring with N registers.
 *   2. If it succeeds -> done.
 *   3. Loop up to K times:
 *      a. Select the best web to split (selectSplitCandidate).
 *      b. Split it into subA and subB (splitWeb).
 *      c. Remove the original web node from the graph.
 *      d. Add subA and subB as new nodes.
 *      e. Add edges for subA and subB (check interference with all other nodes).
 *      f. Retry coloring.
 *   4. Return false if still failing after K splits.
 */

#include "SplittingAlgorithm.h"
#include "InterferenceGraphBuilder.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// bool SplittingAlgorithm::allocate(Graph& graph, std::vector<Web>& webs, int N, int K)
// -----------------------------------------------------------------------------
// Tries to allocate registers with up to K web splits.
//
// TODO: implement following the algorithm description above
bool SplittingAlgorithm::allocate(Graph& graph, std::vector<Web>& webs, int N, int K) {
    // TODO: try basic coloring first
    // TODO: loop K times: select split candidate, split it, update graph, retry coloring
    return false;
}

// -----------------------------------------------------------------------------
// int SplittingAlgorithm::selectSplitCandidate(const Graph& graph) const
// -----------------------------------------------------------------------------
// Returns the ID of the web most worth splitting.
// Same heuristic as spilling: highest-degree node.
//
// TODO: find and return the active node with the highest degree
int SplittingAlgorithm::selectSplitCandidate(const Graph& graph) const {
    // TODO: implement
    return -1;
}

// -----------------------------------------------------------------------------
// void SplittingAlgorithm::splitWeb(const Web& web, Web& subA, Web& subB) const
// -----------------------------------------------------------------------------
// Splits 'web' into two sub-webs at the midpoint of its sorted line list.
//
// STEPS:
//   1. Get web.getSortedPoints() -> sorted list of all line points.
//   2. Find the midpoint: mid = sortedPoints.size() / 2
//   3. subA gets points [0 .. mid-1], subB gets points [mid .. end].
//   4. Assign new unique IDs to subA and subB.
//   5. Set subA.varName = subB.varName = web.varName + "_split".
//   6. Call subA.addRange() and subB.addRange() to populate allLines.
//
// TODO: implement
void SplittingAlgorithm::splitWeb(const Web& web, Web& subA, Web& subB) const {
    // TODO: get sorted points
    // TODO: split at midpoint
    // TODO: build two LiveRange objects from the two halves
    // TODO: assign new IDs and call addRange on each sub-web
}
