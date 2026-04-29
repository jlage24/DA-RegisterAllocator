/**
 * @file Graph.cpp
 * @brief Implementation of the Interference Graph (João's part).
 *
 * This is the CORE data structure of the entire project.
 * It must be based on the graph structure from the TP lectures.
 *
 * ============================================================
 * STRUCTURE
 * ============================================================
 * Nodes  = Webs (one node per web, identified by web.id)
 * Edges  = Interference between webs (undirected)
 *
 * Internally, represent the graph using an ADJACENCY LIST:
 *   - A vector of Web objects (the nodes).
 *   - A vector<vector<int>> or vector<set<int>> for the adjacency list.
 *     adjacency[i] = list of node IDs that node i is connected to.
 *   - A vector<bool> active: active[i] = true if node i is still in the graph.
 *     Used during the coloring algorithm (soft removal without physical deletion).
 *
 * ============================================================
 * SOFT REMOVAL (disableNode / enableNode)
 * ============================================================
 * The greedy coloring algorithm needs to "remove" nodes during Phase 1
 * and "restore" them during Phase 2. Instead of actually deleting nodes,
 * we use the active[] boolean array:
 *   - disableNode(id): active[id] = false
 *   - enableNode(id):  active[id] = true
 *   - degree(id): count only neighbors where active[neighbor] = true
 *   - getActiveNodes(): return IDs where active[id] = true
 */

#include "Graph.h"

// -----------------------------------------------------------------------------
// void Graph::addNode(const Web& web)
// -----------------------------------------------------------------------------
// Adds a web as a new node in the graph.
//
// STEPS:
//   1. Store the web in the nodes vector (or a map by web.id).
//   2. Expand the adjacency list by one empty entry.
//   3. Set active[web.id] = true.
//
// TODO: implement
void Graph::addNode(const Web& web) {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Graph::addEdge(int webId1, int webId2)
// -----------------------------------------------------------------------------
// Adds an undirected interference edge between two nodes.
//
// STEPS:
//   1. Add webId2 to adjacency[webId1].
//   2. Add webId1 to adjacency[webId2].
//   (The graph is undirected, so both directions must be stored.)
//
// TODO: implement (avoid adding duplicate edges)
void Graph::addEdge(int webId1, int webId2) {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// int Graph::degree(int webId) const
// -----------------------------------------------------------------------------
// Returns the number of ACTIVE neighbors of a node.
// (Inactive/disabled neighbors are NOT counted.)
//
// This is critical for the coloring algorithm:
//   - In Phase 1, we remove nodes with degree < N.
//   - After a node is disabled, its contribution to neighbors' degree drops.
//
// STEPS:
//   1. Iterate over adjacency[webId].
//   2. Count only neighbors where active[neighbor] == true.
//   3. Return the count.
//
// TODO: implement
int Graph::degree(int webId) const {
    // TODO: implement
    return 0;
}

// -----------------------------------------------------------------------------
// void Graph::disableNode(int webId)
// -----------------------------------------------------------------------------
// Marks a node as inactive (soft removal from the graph).
// Used in Phase 1 of the coloring algorithm.
//
// TODO: active[webId] = false
void Graph::disableNode(int webId) {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// void Graph::enableNode(int webId)
// -----------------------------------------------------------------------------
// Re-activates a previously disabled node.
// Used in Phase 2 of the coloring algorithm when popping from the stack.
//
// TODO: active[webId] = true
void Graph::enableNode(int webId) {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// bool Graph::allNodesHighDegree(int N) const
// -----------------------------------------------------------------------------
// Returns true if ALL active nodes have degree >= N.
// This is the "stuck" condition in Phase 1 of the coloring algorithm.
// When stuck, we must select a node to spill and remove it.
//
// TODO: iterate over all active nodes, return false if any has degree < N
bool Graph::allNodesHighDegree(int N) const {
    // TODO: implement
    return false;
}

// -----------------------------------------------------------------------------
// std::vector<int> Graph::getActiveNodes() const
// -----------------------------------------------------------------------------
// Returns the IDs of all currently active nodes.
// Used by the coloring algorithm to iterate over the remaining graph.
//
// TODO: iterate over all nodes, collect IDs where active[id] == true
std::vector<int> Graph::getActiveNodes() const {
    // TODO: implement
    return {};
}

// -----------------------------------------------------------------------------
// std::vector<int> Graph::getNeighbors(int webId) const
// -----------------------------------------------------------------------------
// Returns the IDs of all ACTIVE neighbors of a node.
// Used in Phase 2 to find which colors are already taken by neighbors.
//
// TODO: filter adjacency[webId] to only include active neighbors
std::vector<int> Graph::getNeighbors(int webId) const {
    // TODO: implement
    return {};
}

// -----------------------------------------------------------------------------
// bool Graph::isEmpty() const
// -----------------------------------------------------------------------------
// Returns true if there are no active nodes left in the graph.
// Used as the loop termination condition in Phase 1.
//
// TODO: return true if getActiveNodes().empty()
bool Graph::isEmpty() const {
    // TODO: implement
    return true;
}

// -----------------------------------------------------------------------------
// Web& Graph::getWeb(int webId)
// -----------------------------------------------------------------------------
// Returns a reference to the Web stored at this node.
// Used to read/write the web's assignedRegister after coloring.
//
// TODO: return the web object associated with webId
Web& Graph::getWeb(int webId) {
    // TODO: implement (use a map or find in vector by id)
    static Web dummy;
    return dummy;
}
