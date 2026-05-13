#include "FreeAllocator.h"
#include "RegisterAllocator.h"
#include "../graph/InterferenceGraphBuilder.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <climits>

int FreeAllocator::findMostCongestedWeb(const InterferenceGraph &ig,
                                         int numRegisters) {
    int best = -1;
    int bestDeg = -1;
    for (auto &[id, web] : ig.webs) {
        if (!web.active) continue;
        int deg = ig.degree(id);
        if (deg >= numRegisters && deg > bestDeg) {
            bestDeg = deg;
            best = id;
        }
    }
    return best;
}

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
    size_t mid = sorted.size() / 2;

    // Cannot split a single-line web, therefore spill it directly
    if (sorted.size() <= 1) return {-1, -1};

    // Build first half
    LiveRange lrA; lrA.varName = target->varName;
    for (size_t i = 0; i < mid; i++)
        lrA.points.push_back({sorted[i].first, sorted[i].second});
    if (!lrA.points.empty() && lrA.points.back().marker == ' ')
        lrA.points.back().marker = '-';

    // Build second half
    LiveRange lrB; lrB.varName = target->varName;
    for (size_t i = mid; i < sorted.size(); i++)
        lrB.points.push_back({sorted[i].first, sorted[i].second});
    if (!lrB.points.empty() && lrB.points.front().marker == ' ')
        lrB.points.front().marker = '+';

    int idA = nextId, idB = nextId + 1;

    Web webA(idA, target->varName); webA.ranges = {lrA};
    Web webB(idB, target->varName); webB.ranges = {lrB};

    std::cout << "[free] Split web" << webId
              << " (" << target->varName << ")"
              << " → web" << idA << " [" << lrA.firstLine() << ".." << lrA.lastLine() << "]"
              << " + web" << idB << " [" << lrB.firstLine() << ".." << lrB.lastLine() << "]\n";

    // Remove original, add two halves
    webs.erase(std::remove_if(webs.begin(), webs.end(),
                              [webId](const Web &w){ return w.webId == webId; }),
               webs.end());
    webs.push_back(webA);
    webs.push_back(webB);

    return {idA, idB};
}

InterferenceGraph FreeAllocator::allocate(std::vector<Web> &webs,
                                           int numRegisters) {
    int nextId = 0;
    for (auto &w : webs) nextId = std::max(nextId, w.webId + 1);

    // Safety limit: at most 2*W iterations to prevent infinite loops
    int maxIter = static_cast<int>(webs.size()) * 2;
    int iter = 0;

    while (iter++ < maxIter) {
        // Rebuild graph from current webs
        InterferenceGraph ig = InterferenceGraphBuilder::build(webs);

        // Find the most congested web (degree >= N)
        int congested = findMostCongestedWeb(ig, numRegisters);

        // No congested webs — graph is colorable as-is
        if (congested == -1) {
            std::cout << "[free] Graph colorable — running greedy coloring.\n";
            RegisterAllocator::allocateBasic(ig, numRegisters);
            return ig;
        }

        std::cout << "[free] web" << congested
                  << " (" << ig.webs.at(congested).varName << ")"
                  << " has degree=" << ig.degree(congested)
                  << " >= N=" << numRegisters << "\n";

        // Try to split it
        auto [idA, idB] = splitWeb(webs, congested, nextId);

        if (idA == -1) {
            // Cannot split (single-line web) — spill it directly
            std::cout << "[free] Cannot split web" << congested
                      << " — spilling instead.\n";
            // Mark as spilled in the webs vector
            for (auto &w : webs)
                if (w.webId == congested) { w.color = COLOR_SPILLED; break; }
        } else {
            nextId += 2;

            // Check if the split helped — rebuild and see if halves are colorable
            InterferenceGraph igNew = InterferenceGraphBuilder::build(webs);
            int degA = igNew.degree(idA);
            int degB = igNew.degree(idB);

            // If a half still has degree >= N and is not splittable further,
            // spill it
            for (int halfId : {idA, idB}) {
                if (igNew.degree(halfId) >= numRegisters) {
                    // Check if it has more than 1 line (can be split again next iter)
                    auto it = std::find_if(webs.begin(), webs.end(),
                                           [halfId](const Web &w){ return w.webId == halfId; });
                    if (it != webs.end() && it->allLines().size() <= 1) {
                        std::cout << "[free] web" << halfId << " unsplittable — spilling.\n";
                        it->color = COLOR_SPILLED;
                    }
                    // Otherwise: leave it, since next iteration will try to split it again
                }
            }
            (void)degA; (void)degB; // used implicitly above
        }
    }

    // Fallback: run greedy coloring on whatever remains
    std::cout << "[free] Max iterations reached — running final greedy coloring.\n";
    InterferenceGraph ig = InterferenceGraphBuilder::build(webs);

    // Pre-mark spilled webs from our webs vector
    for (auto &w : webs)
        if (w.color == COLOR_SPILLED && ig.webs.count(w.webId))
            ig.webs.at(w.webId).color  = COLOR_SPILLED,
            ig.webs.at(w.webId).active = false;

    RegisterAllocator::allocateBasic(ig, numRegisters);
    return ig;
}