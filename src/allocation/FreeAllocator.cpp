#include "FreeAllocator.h"
#include "RegisterAllocator.h"
#include "../graph/InterferenceGraphBuilder.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <climits>

// Highest-degree active web with degree >= N
int FreeAllocator::findMostCongestedWeb(const InterferenceGraph &ig,
                                         int numRegisters) {
    int best = -1, bestDeg = -1;
    for (auto &[id, web] : ig.webs) {
        if (!web.active) continue;
        int deg = ig.degree(id);
        if (deg >= numRegisters && deg > bestDeg) { bestDeg = deg; best = id; }
    }
    return best;
}

// Split one web into two halves, update webs vector
std::pair<int,int> FreeAllocator::splitWeb(std::vector<Web> &webs,
                                            int webId, int nextId) {
    // Find the web to split
    Web *target = nullptr;
    for (auto &w : webs)
        if (w.webId == webId) { target = &w; break; }
    if (!target) return {-1, -1};

    // Collect all line points sorted by line
    std::map<int,char> lineMarkers;
    for (auto &r : target->ranges)
        for (auto &p : r.points) {
            if (!lineMarkers.count(p.line)) lineMarkers[p.line] = p.marker;
            else if (p.marker != ' ')       lineMarkers[p.line] = p.marker;
        }

    std::vector<std::pair<int,char>> sorted(lineMarkers.begin(), lineMarkers.end());
    if (sorted.size() <= 1) return {-1, -1};  // unsplittable

    size_t mid = sorted.size() / 2;

    LiveRange lrA; lrA.varName = target->varName;
    for (size_t i = 0; i < mid; i++)
        lrA.points.push_back({sorted[i].first, sorted[i].second});
    if (!lrA.points.empty() && lrA.points.back().marker == ' ')
        lrA.points.back().marker = '-';

    LiveRange lrB; lrB.varName = target->varName;
    for (size_t i = mid; i < sorted.size(); i++)
        lrB.points.push_back({sorted[i].first, sorted[i].second});
    if (!lrB.points.empty() && lrB.points.front().marker == ' ')
        lrB.points.front().marker = '+';

    int idA = nextId, idB = nextId + 1;
    Web webA(idA, target->varName); webA.ranges = {lrA};
    Web webB(idB, target->varName); webB.ranges = {lrB};

    std::cout << "[free] Split web" << webId << " (" << target->varName << ")"
              << " → web" << idA << " [" << lrA.firstLine() << ".." << lrA.lastLine() << "]"
              << " + web" << idB << " [" << lrB.firstLine() << ".." << lrB.lastLine() << "]\n";

    // Remove original, add two halves
    webs.erase(std::remove_if(webs.begin(), webs.end(),
               [webId](const Web &w){ return w.webId == webId; }), webs.end());
    webs.push_back(webA);
    webs.push_back(webB);
    return {idA, idB};
}

InterferenceGraph FreeAllocator::allocate(std::vector<Web> &webs,
                                           int numRegisters) {
    int nextId = 0;
    for (auto &w : webs) nextId = std::max(nextId, w.webId + 1);

    // Track which webs we already tried splitting (avoid infinite loops)
    std::set<int> triedSplit;

    // At most W rounds (one intervention per original web)
    int maxRounds = (int)webs.size() * 3;

    for (int round = 0; round < maxRounds; round++) {
        InterferenceGraph ig = InterferenceGraphBuilder::build(webs);

        // Pre-mark webs already spilled in our vector
        for (auto &w : webs)
            if (w.color == COLOR_SPILLED && ig.webs.count(w.webId)) {
                ig.webs.at(w.webId).color  = COLOR_SPILLED;
                ig.webs.at(w.webId).active = false;
            }

        int congested = findMostCongestedWeb(ig, numRegisters);
        if (congested == -1) {
            // All remaining active webs are colorable
            std::cout << "[free] Graph colorable — running greedy coloring.\n";
            RegisterAllocator::allocateBasic(ig, numRegisters);
            return ig;
        }

        std::cout << "[free] web" << congested
                  << " (" << ig.webs.at(congested).varName << ")"
                  << " degree=" << ig.degree(congested) << " >= N=" << numRegisters << "\n";

        // If we already tried splitting this web, spill it instead
        if (triedSplit.count(congested)) {
            std::cout << "[free] Already tried split on web" << congested << " — spilling.\n";
            for (auto &w : webs)
                if (w.webId == congested) { w.color = COLOR_SPILLED; break; }
            continue;
        }

        // Try to split
        auto [idA, idB] = splitWeb(webs, congested, nextId);

        if (idA == -1) {
            // Unsplittable (1 line) — spill directly
            std::cout << "[free] web" << congested << " unsplittable — spilling.\n";
            for (auto &w : webs)
                if (w.webId == congested) { w.color = COLOR_SPILLED; break; }
        } else {
            nextId += 2;
            triedSplit.insert(congested);

            // Check if split actually helped (at least one half has degree < N)
            InterferenceGraph igCheck = InterferenceGraphBuilder::build(webs);
            bool helpedA = igCheck.degree(idA) < numRegisters;
            bool helpedB = igCheck.degree(idB) < numRegisters;

            if (!helpedA && !helpedB) {
                // Split didn't help — undo by spilling both halves
                std::cout << "[free] Split didn't reduce interference — spilling halves.\n";
                for (auto &w : webs)
                    if (w.webId == idA || w.webId == idB) w.color = COLOR_SPILLED;
            }
        }
    }

    // Final fallback
    std::cout << "[free] Final greedy coloring.\n";
    InterferenceGraph ig = InterferenceGraphBuilder::build(webs);
    for (auto &w : webs)
        if (w.color == COLOR_SPILLED && ig.webs.count(w.webId)) {
            ig.webs.at(w.webId).color  = COLOR_SPILLED;
            ig.webs.at(w.webId).active = false;
        }
    RegisterAllocator::allocateBasic(ig, numRegisters);
    return ig;
}