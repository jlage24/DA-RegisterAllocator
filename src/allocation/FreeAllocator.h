#ifndef FREEALLOCATOR_H
#define FREEALLOCATOR_H

#include "../graph/InterferenceGraph.h"
#include "../graph/Web.h"
#include <vector>

/**
 * @brief Free register allocator — Adaptive Split-then-Spill (T2.4).
 *
 * Strategy:
 *   For each web with degree >= N (cannot be trivially colored):
 *     1. Try to SPLIT it first — splitting reduces live range and may
 *        eliminate interference edges, allowing coloring to succeed.
 *     2. If after splitting the halves still have degree >= N,
 *        SPILL the problematic half instead.
 *
 *   More conservative than blind spilling (T2.2) and more targeted
 *   than blind splitting (T2.3): only intervenes on webs that actually
 *   block coloring, preferring split over spill whenever possible.
 *
 * @note Time Complexity: O(W^2 * L) per iteration, at most W iterations total.
 *             W = number of webs, L = average lines per web.
 */
class FreeAllocator {
public:
    /**
     * @brief Runs the adaptive split-then-spill allocation.
     * @param webs         Original webs (may grow due to splitting).
     * @param numRegisters Number of available physical registers (N).
     * @return Fully colored interference graph.
     */
    static InterferenceGraph allocate(std::vector<Web> &webs, int numRegisters);

private:
    /**
     * @brief Returns the webId of the active web with highest degree >= N.
     *        Returns -1 if all active webs have degree < N (colorable).
     */
    static int findMostCongestedWeb(const InterferenceGraph &ig, int numRegisters);

    /**
     * @brief Splits a web into two halves, updates the webs vector,
     *        and returns the IDs of the two new webs.
     * @param webs   Web list (modified in place).
     * @param webId  ID of the web to split.
     * @param nextId Next available web ID.
     * @return Pair (idA, idB) of the two new webs.
     */
    static std::pair<int,int> splitWeb(std::vector<Web> &webs,
                                       int webId, int nextId);
};

#endif // FREEALLOCATOR_H