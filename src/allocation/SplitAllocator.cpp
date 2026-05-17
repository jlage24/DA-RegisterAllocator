#include "SplitAllocator.h"
#include "RegisterAllocator.h"
#include "../graph/InterferenceGraphBuilder.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <map>

// Break one web into two halves
void SplitAllocator::splitWeb(const Web &web, int idA, int idB,
                               Web &outA, Web &outB) {
    // Collect all line points sorted by line number
    std::map<int,char> lineMarkers;
    for (auto &r : web.ranges)
        for (auto &p : r.points) {
            if (!lineMarkers.count(p.line)) lineMarkers[p.line] = p.marker;
            else if (p.marker != ' ')       lineMarkers[p.line] = p.marker;
        }

    std::vector<std::pair<int,char>> sorted(lineMarkers.begin(), lineMarkers.end());
    size_t mid = sorted.size() / 2;
    if (sorted.size() <= 1) { outA = web; outB = Web(-1, ""); return; }

    LiveRange lrA; lrA.varName = web.varName;
    for (size_t i = 0; i < mid; i++)
        lrA.points.push_back({sorted[i].first, sorted[i].second});
    // Ensure last point of first half has '-' marker (last use)
    if (!lrA.points.empty() && lrA.points.back().marker == ' ')
        lrA.points.back().marker = '-';

    LiveRange lrB; lrB.varName = web.varName;
    for (size_t i = mid; i < sorted.size(); i++)
        lrB.points.push_back({sorted[i].first, sorted[i].second});
    // Ensure first point of second half has '+' marker (new def)
    if (!lrB.points.empty() && lrB.points.front().marker == ' ')
        lrB.points.front().marker = '+';

    outA = Web(idA, web.varName); outA.ranges = {lrA};
    outB = Web(idB, web.varName); outB.ranges = {lrB};

    std::cout << "[split] web" << web.webId << " (" << web.varName << ")"
              << " → web" << idA << " [" << lrA.firstLine() << ".." << lrA.lastLine() << "]"
              << " + web" << idB << " [" << lrB.firstLine() << ".." << lrB.lastLine() << "]\n";
}

// Try splitting 1, 2, ... up to numSplits until colorable
InterferenceGraph SplitAllocator::allocate(std::vector<Web> &webs,
                                            int numRegisters, int numSplits) {
    numSplits = std::min(numSplits, (int)webs.size());

    // Sort by line count desc —> always split the biggest first
    auto getBiggest = [](const std::vector<Web> &ws, const std::set<int> &alreadySplit) -> int {
        int best = -1; size_t bestSize = 0;
        for (auto &w : ws) {
            if (alreadySplit.count(w.webId)) continue;
            if (w.allLines().size() > bestSize) { bestSize = w.allLines().size(); best = w.webId; }
        }
        return best;
    };

    std::vector<Web> current = webs;
    int nextId = 0;
    for (auto &w : current) nextId = std::max(nextId, w.webId + 1);

    std::set<int> splitDone;

    for (int k = 1; k <= numSplits; k++) {
        // Find next best web to split
        int targetId = getBiggest(current, splitDone);
        if (targetId == -1) break;

        // Find the web and split it
        Web *target = nullptr;
        for (auto &w : current) if (w.webId == targetId) { target = &w; break; }
        if (!target) break;

        Web outA(0,""), outB(0,"");
        splitWeb(*target, nextId, nextId+1, outA, outB);
        splitDone.insert(targetId);

        // Replace in current
        current.erase(std::remove_if(current.begin(), current.end(),
            [targetId](const Web &w){ return w.webId == targetId; }), current.end());
        if (outB.webId != -1) { current.push_back(outA); current.push_back(outB); nextId += 2; }

        // Try coloring
        auto ig = InterferenceGraphBuilder::build(current);
        RegisterAllocator::allocateBasic(ig, numRegisters);

        bool forcedSpill = false;
        for (auto &[id, web] : ig.webs)
            if (web.color == COLOR_SPILLED) { forcedSpill = true; break; }

        if (!forcedSpill) {
            std::cout << "[split] Success with k=" << k << " split(s).\n";
            webs = current;
            return ig;
        }
        std::cout << "[split] k=" << k << " not enough, trying more...\n";
    }

    // Final attempt with all splits done
    std::cout << "[split] Final coloring attempt after " << numSplits << " split(s).\n";
    webs = current;
    auto ig = InterferenceGraphBuilder::build(current);
    RegisterAllocator::allocateBasic(ig, numRegisters);
    return ig;
}