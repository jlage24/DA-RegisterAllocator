#ifndef INTERFERENCEGRAPH_H
#define INTERFERENCEGRAPH_H

#include "../../include/Graph.h"
#include "Web.h"
#include <map>
#include <string>
#include <fstream>

/**
 * @brief Interference Graph for register allocation.
 *
 * Extends Graph<int> where each node's key is a webId.
 * Edges are undirected (added in both directions).
 * Provides graph-coloring and degree utilities needed by the allocation algorithms.
 *
 * @note The base Graph<int> stores directed edges. We simulate undirected
 *       behaviour by always adding edges in both directions via addUndirectedEdge().
 */
class InterferenceGraph : public Graph<int> {
public:
    /// Map from webId → Web metadata (color, varName, ranges, active flag)
    std::map<int, Web> webs;

    /**
     * @brief Adds a web as a node in the graph.
     * @param web The web to add.
     */
    void addWeb(const Web &web);

    /**
     * @brief Adds an undirected interference edge between two webs.
     * @param webIdA First web.
     * @param webIdB Second web.
     * @note No-op if edge already exists or if a == b.
     * @complexity O(degree(a) + degree(b))
     */
    void addUndirectedEdge(int webIdA, int webIdB);

    /**
     * @brief Checks if an undirected edge exists between two webs.
     * @complexity O(degree(a))
     */
    bool hasEdge(int webIdA, int webIdB) const;

    /**
     * @brief Returns the effective degree of a web node,
     *        counting only active neighbors.
     * @complexity O(degree(webId))
     */
    int degree(int webId) const;

    /**
     * @brief Emits an undirected DOT file for visualization.
     * @param filename Output filename (without extension).
     */
    void emitUndirectedDOT(const std::string &filename) const;

    /**
     * @brief Prints a summary of the graph to stdout.
     */
    void print() const;
};

#endif // INTERFERENCEGRAPH_H