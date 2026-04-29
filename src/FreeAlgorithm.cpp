/**
 * @file FreeAlgorithm.cpp
 * @brief Implementation of the custom allocation algorithm (T2.4, both members).
 *
 * Used when "algorithm: free" is in the config file.
 * Worth 4 points - worth investing time here!
 *
 * ============================================================
 * SUGGESTED APPROACH: DSatur Heuristic
 * ============================================================
 * DSatur (Degree of Saturation) is a well-known graph coloring heuristic
 * that typically outperforms simple greedy coloring.
 *
 * KEY IDEA:
 *   Instead of processing nodes in a fixed order, always color the node
 *   with the HIGHEST SATURATION next.
 *
 *   Saturation of node n = number of DISTINCT colors used by n's neighbors.
 *   (A neighbor with no color yet contributes 0 to saturation.)
 *
 * WHY IS THIS BETTER?
 *   - Simple greedy can make poor color choices early that force many colors later.
 *   - DSatur prioritizes the "most constrained" nodes first, reducing conflicts.
 *   - In practice, DSatur often finds colorings with fewer colors than simple greedy.
 *
 * ============================================================
 * DSatur ALGORITHM
 * ============================================================
 *   1. Initialize: all nodes uncolored, saturation[n] = 0 for all n.
 *   2. Repeat until all nodes are colored:
 *      a. Select the uncolored node n with the HIGHEST saturation.
 *         Tie-break: pick the one with the highest degree.
 *      b. Assign n the lowest color not used by any of n's already-colored neighbors.
 *      c. For each uncolored neighbor m of n:
 *         - If the color just assigned to n is new for m's neighborhood,
 *           increment saturation[m].
 *   3. Count the number of distinct colors used.
 *   4. If all N colors fit within config.numRegisters -> feasible.
 *      Otherwise -> try spilling the highest-saturation node and repeat.
 *
 * ============================================================
 * ALTERNATIVE IDEAS (if you prefer a different approach)
 * ============================================================
 *   - Chaitin-Briggs: full industrial-strength register allocator.
 *   - Interval graph coloring: treat webs as intervals, use sweep-line O(n log n).
 *   - Combined auto spill+split: automatically decide for each problematic web
 *     whether to spill or split based on a cost function.
 *
 * Whatever you implement, DOCUMENT the rationale in your presentation.
 */

#include "FreeAlgorithm.h"
#include <algorithm>
#include <map>
#include <set>

// -----------------------------------------------------------------------------
// bool FreeAlgorithm::allocate(Graph& graph, std::vector<Web>& webs, int N)
// -----------------------------------------------------------------------------
// Runs the custom allocation algorithm.
//
// SUGGESTED STEPS (DSatur):
//   1. Initialize saturation map: saturation[webId] = 0 for all nodes.
//   2. Initialize colored map: colored[webId] = false for all nodes.
//   3. Loop until all nodes are colored:
//      a. Find uncolored node with highest saturation (tie-break: highest degree).
//      b. Find the lowest color not used by any colored neighbor.
//      c. If that color < N -> assign it (web.assignedRegister = color).
//         Otherwise -> spill this node (assignedRegister = -1).
//      d. Mark node as colored.
//      e. Update saturation of all uncolored neighbors.
//   4. Return true if no spills occurred and all registers fit within N.
//
// TODO: implement
bool FreeAlgorithm::allocate(Graph& graph, std::vector<Web>& webs, int N) {
    // TODO: implement DSatur or your chosen algorithm
    return false;
}
e "FreeAlgorithm.h"