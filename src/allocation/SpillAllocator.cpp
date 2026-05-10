#include "SpillAllocator.h"
#include "RegisterAllocator.h"
#include <algorithm>
#include <iostream>
#include <vector>

// ---------------------------------------------------------------------------
// preSpill — mark the K webs with most lines as COLOR_SPILLED
// ---------------------------------------------------------------------------
void SpillAllocator::preSpill(InterferenceGraph &ig, int numSpills) {
    // Build a list of (webId, lineCount) for all active webs
    std::vector<std::pair<int,int>> candidates; // (webId, lineCount)
    for (auto &[id, web] : ig.webs) {
        if (!web.active) continue;
        int lineCount = static_cast<int>(web.allLines().size());
        candidates.push_back({id, lineCount});
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

// ---------------------------------------------------------------------------
// allocate — pre-spill K webs, then run basic greedy coloring on the rest
// ---------------------------------------------------------------------------
void SpillAllocator::allocate(InterferenceGraph &ig,
                               int numRegisters, int numSpills) {
    int totalWebs = static_cast<int>(ig.webs.size());

    // Clamp: cannot spill more webs than we have
    if (numSpills >= totalWebs) {
        std::cerr << "[spill] Warning: numSpills (" << numSpills
                  << ") >= total webs (" << totalWebs
                  << "). Clamping to " << totalWebs - 1 << ".\n";
        numSpills = totalWebs - 1;
    }

    // Step 1: intentionally spill K webs
    preSpill(ig, numSpills);

    // Step 2: run basic greedy coloring on the remaining active webs
    RegisterAllocator::allocateBasic(ig, numRegisters);
}
