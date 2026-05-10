#ifndef REGISTERALLOCATOR_H
#define REGISTERALLOCATOR_H

#include "../graph/InterferenceGraph.h"
#include <vector>
#include <stack>

/**
 * @brief Greedy graph-coloring register allocator.
 *
 * Phase 1 — Simplification:
 *   Repeatedly find a node with degree < N, remove it from the active graph
 *   and push it onto a stack. If no such node exists, pick one to spill
 *   (lowest degree heuristic), mark it as COLOR_SPILLED, and remove it.
 *   Repeat until the graph is empty.
 *
 * Phase 2 — Coloring:
 *   Pop each node from the stack, reactivate it, and assign the lowest
 *   available color not used by any active neighbour.
 *
 * @note Colors are 0-indexed (r0, r1, ..., r(N-1)).
 *       Spilled webs are marked with COLOR_SPILLED (-2) → output as "M".
 */
class RegisterAllocator {
public:
    /**
     * @brief Runs the basic greedy coloring algorithm.
     * @param ig    The interference graph (modified in place).
     * @param numRegisters  Number of available physical registers (N).
     */
    static void allocateBasic(InterferenceGraph &ig, int numRegisters);

private:
    /// Returns the webId of the active node with the lowest degree.
    /// Used as spill candidate when no node has degree < N.
    static int pickSpillCandidate(const InterferenceGraph &ig);

    /// Assigns the lowest color not used by active neighbours of webId.
    /// Returns the assigned color.
    static int assignColor(const InterferenceGraph &ig, int webId, int numRegisters);
};

#endif // REGISTERALLOCATOR_H