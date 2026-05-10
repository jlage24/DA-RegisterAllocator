#ifndef SPILLALLOCATOR_H
#define SPILLALLOCATOR_H

#include "../graph/InterferenceGraph.h"

/**
 * @brief Register allocator with intentional pre-spilling (T2.2).
 *
 * Before running the greedy coloring, intentionally spills the K webs
 * with the most program lines covered. This reduces the interference graph
 * and increases the chance that the remaining webs fit in N registers.
 *
 * Spill heuristic: largest number of lines covered (most live = most
 * likely to interfere with others, so most beneficial to remove).
 *
 * @note After pre-spilling, runs the same greedy coloring as basic (T2.1).
 */
class SpillAllocator {
public:
    /**
     * @brief Runs allocation with K intentional pre-spills.
     * @param ig            The interference graph (modified in place).
     * @param numRegisters  Number of available physical registers (N).
     * @param numSpills     Number of webs to pre-spill (K).
     */
    static void allocate(InterferenceGraph &ig, int numRegisters, int numSpills);

private:
    /**
     * @brief Selects and marks the K webs with the most lines as spilled.
     * @param ig        The interference graph.
     * @param numSpills K — number of webs to spill.
     */
    static void preSpill(InterferenceGraph &ig, int numSpills);
};

#endif // SPILLALLOCATOR_H
