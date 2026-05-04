#include "InterferenceGraphBuilder.h"
#include <set>


bool InterferenceGraphBuilder::interferes(const Web &a, const Web &b) {
    std::set<int> linesA = a.allLines();
    std::set<int> linesB = b.allLines();

    for (int line : linesA) {
        if (!linesB.count(line)) continue;

        // Both webs cover this line — check the special non-interference case:
        // If one web ends here with '-' and the other starts here with '+'
        // they do not interfere (use happens before def at the same line)
        char markerA = a.markerAt(line);
        char markerB = b.markerAt(line);

        bool aEndsHere  = (markerA == '-');
        bool bStartsHere = (markerB == '+');
        bool bEndsHere  = (markerB == '-');
        bool aStartsHere = (markerA == '+');

        // Special case: A ends (use) and B starts (def) at same line -> no interference
        if (aEndsHere && bStartsHere) continue;
        // Symmetric: B ends (use) and A starts (def) at same line -> no interference
        if (bEndsHere && aStartsHere) continue;

        // All other cases of shared line -> interference
        return true;
    }

    return false;
}


InterferenceGraph InterferenceGraphBuilder::build(const std::vector<Web> &webs) {
    InterferenceGraph ig;

    // Add all webs as nodes
    for (const auto &web : webs)
        ig.addWeb(web);

    // Check every pair of webs for interference
    for (size_t i = 0; i < webs.size(); i++) {
        for (size_t j = i + 1; j < webs.size(); j++) {
            if (interferes(webs[i], webs[j]))
                ig.addUndirectedEdge(webs[i].webId, webs[j].webId);
        }
    }

    return ig;
}