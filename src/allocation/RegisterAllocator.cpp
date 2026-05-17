#include "RegisterAllocator.h"
#include <set>
#include <iostream>
#include <climits>

// Lowest degree active node
int RegisterAllocator::pickSpillCandidate(const InterferenceGraph &ig) {
    int best = -1;
    int bestDeg = INT_MAX;
    for (auto v : ig.getVertexSet()) {
        int id = v->getInfo();
        auto it = ig.webs.find(id);
        if (it == ig.webs.end() || !it->second.active) continue;
        int deg = ig.degree(id);
        if (deg < bestDeg) {
            bestDeg = deg;
            best = id;
        }
    }
    return best;
}

// Lowest color not used by active neighbors
int RegisterAllocator::assignColor(const InterferenceGraph &ig,
                                    int webId, int numRegisters) {
    std::set<int> usedColors;
    auto v = ig.findVertex(webId);
    if (v) {
        for (auto &e : v->getAdj()) {
            int neighId = e.getDest()->getInfo();
            auto it = ig.webs.find(neighId);
            if (it != ig.webs.end() && it->second.active && it->second.color >= 0)
                usedColors.insert(it->second.color);
        }
    }
    for (int c = 0; c < numRegisters; c++)
        if (!usedColors.count(c)) return c;

    // No color available: this node must be spilled
    return COLOR_SPILLED;
}

// Two-phase greedy coloring
void RegisterAllocator::allocateBasic(InterferenceGraph &ig, int numRegisters) {
    std::stack<int> stk;
    // Count active nodes
    auto activeCount = [&]() {
        int cnt = 0;
        for (auto &[id, web] : ig.webs)
            if (web.active) cnt++;
        return cnt;
    };

    while (activeCount() > 0) {
        // Find an active node with degree < N
        int candidate = -1;
        for (auto &[id, web] : ig.webs) {
            if (!web.active) continue;
            if (ig.degree(id) < numRegisters) {
                candidate = id;
                break;
            }
        }

        if (candidate != -1) {
            // Found a simplifiable node —> push and deactivate
            stk.push(candidate);
            ig.webs.at(candidate).active = false;
        } else {
            // All active nodes have degree >= N —> must spill one
            int spill = pickSpillCandidate(ig);
            if (spill == -1) break; // safety
            ig.webs.at(spill).color  = COLOR_SPILLED;
            ig.webs.at(spill).active = false;
            std::cout << "[allocator] Spilling web" << spill
                      << " (" << ig.webs.at(spill).varName << ")\n";
        }
    }

    while (!stk.empty()) {
        int id = stk.top(); stk.pop();
        ig.webs.at(id).active = true;
        ig.webs.at(id).color  = assignColor(ig, id, numRegisters);
    }
}