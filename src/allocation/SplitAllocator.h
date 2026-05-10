#ifndef SPLITALLOCATOR_H
#define SPLITALLOCATOR_H

#include "../graph/InterferenceGraph.h"
#include "../graph/Web.h"
#include <vector>

/**
 * @brief Register allocator with web splitting (T2.3).
 *
 * Instead of spilling an entire web, splitting breaks a web into smaller
 * pieces, reducing its live range and therefore its interference with
 * other webs. This gives the coloring algorithm more flexibility.
 *
 * Split strategy:
 *   Select the K webs with the most lines covered.
 *   Split each chosen web into two halves (first half / second half).
 *   Rebuild the interference graph with the new webs.
 *   Run basic greedy coloring on the new graph.
 *
 * @note Splitting increases the number of webs. The new webs may or may
 *       not interfere with each other depending on the split point.
 */
class SplitAllocator {
public:
    /**
     * @brief Runs allocation with K web splits.
     * @param webs          Original webs (will be modified).
     * @param numRegisters  Number of available physical registers (N).
     * @param numSplits     Number of webs to split (K).
     * @return New interference graph after splitting and coloring.
     */
    static InterferenceGraph allocate(std::vector<Web> &webs,
                                      int numRegisters,
                                      int numSplits);

private:
    /**
     * @brief Splits a web into two halves at its midpoint.
     *
     * The original web is replaced by two new webs:
     *   - webA: first half of the sorted line points
     *   - webB: second half of the sorted line points
     *
     * Each half becomes a new LiveRange with the same varName.
     * Markers ('+', '-') are preserved where they fall; the split point
     * gets a synthetic '-' on the first half and '+' on the second half.
     *
     * @param web       The web to split.
     * @param idA       WebId for the first half.
     * @param idB       WebId for the second half.
     * @param outA      Output: first half web.
     * @param outB      Output: second half web.
     */
    static void splitWeb(const Web &web, int idA, int idB,
                         Web &outA, Web &outB);
};

#endif // SPLITALLOCATOR_H
