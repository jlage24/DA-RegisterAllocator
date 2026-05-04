#ifndef INTERFERENCEGRAPHBUILDER_H
#define INTERFERENCEGRAPHBUILDER_H

#include "../graph/Web.h"
#include "../graph/InterferenceGraph.h"
#include <vector>

/**
 * @brief Builds the Interference Graph from a list of Webs.
 *
 * Two webs interfere if there exists at least one program point where
 * both are simultaneously live.
 *
 * Special case (no interference):
 *   If webA ends at line X with marker '-' and webB starts at line X
 *   with marker '+', they do NOT interfere — the use of A happens before
 *   the definition of B at that line.
 *
 * @complexity O(W^2 * L) where W = number of webs, L = avg lines per web.
 */
class InterferenceGraphBuilder {
public:
    /**
     * @brief Builds and returns the interference graph.
     * @param webs The list of webs produced by WebBuilder.
     * @return Fully constructed InterferenceGraph.
     */
    static InterferenceGraph build(const std::vector<Web> &webs);

private:
    /**
     * @brief Checks if two webs interfere.
     * @param a First web.
     * @param b Second web.
     * @return true if they must have an interference edge.
     */
    static bool interferes(const Web &a, const Web &b);
};

#endif // INTERFERENCEGRAPHBUILDER_H