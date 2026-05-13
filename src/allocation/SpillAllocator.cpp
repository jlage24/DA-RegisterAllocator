#include "SpillAllocator.h"
#include "RegisterAllocator.h"
#include "../graph/InterferenceGraphBuilder.h"
#include <algorithm>
#include <iostream>
#include <vector>

// Mark the top 'numSpills' webs by line count as COLOR_SPILLED
void SpillAllocator::preSpill(InterferenceGraph &ig, int numSpills) {
    std::vector<std::pair<int,int>> candidates;
    for (auto &[id, web] : ig.webs) {
        if (!web.active) continue;
        candidates.push_back({id, (int)web.allLines().size()});
    }
    // Sort descending by line count (heuristic: most lines = spill first)
    std::sort(candidates.begin(), candidates.end(),
              [](const auto &a, const auto &b){ return a.second > b.second; });

    // Spill the top K
    int spilled = 0;
    for (auto &[id, lineCount] : candidates) {
        if (spilled >= numSpills) break;
        ig.webs.at(id).color  = COLOR_SPILLED;
        ig.webs.at(id).active = false;
        std::cout << "[spill] Pre-spilling web" << id
                  << " (" << ig.webs.at(id).varName << ")"
                  << "  lines=" << lineCount << "\n";
        spilled++;
    }
}

// Try spilling 1, 2, ... up to numSpills until graph is colorable
void SpillAllocator::allocate(InterferenceGraph &ig,
                               int numRegisters, int numSpills) {
    int totalWebs = (int)ig.webs.size();
    numSpills = std::min(numSpills, totalWebs - 1);

    for (int k = 1; k <= numSpills; k++) {
        // Reset all webs to active/uncolored before each attempt
        for (auto &[id, web] : ig.webs) {
            web.active = true;
            web.color  = COLOR_NONE;
        }

        std::cout << "[spill] Trying with k=" << k << " pre-spill(s)...\n";
        preSpill(ig, k);

        // Run basic greedy coloring on remaining active webs
        RegisterAllocator::allocateBasic(ig, numRegisters);

        // Check if any forced spills happened during coloring
        bool forcedSpill = false;
        for (auto &[id, web] : ig.webs)
            if (web.color == COLOR_SPILLED && web.active) { forcedSpill = true; break; }

        if (!forcedSpill) {
            std::cout << "[spill] Success with k=" << k << " pre-spill(s).\n";
            return;
        }
    }

    std::cout << "[spill] Could not fully allocate with up to " << numSpills << " spill(s).\n";
}