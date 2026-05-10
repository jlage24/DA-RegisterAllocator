#include "SplitAllocator.h"
#include "RegisterAllocator.h"
#include "../graph/InterferenceGraphBuilder.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <map>

// ---------------------------------------------------------------------------
// splitWeb — break one web into two halves at its midpoint
// ---------------------------------------------------------------------------
void SplitAllocator::splitWeb(const Web &web, int idA, int idB,
                               Web &outA, Web &outB) {
    // Collect all line points sorted by line number
    std::map<int, char> lineMarkers;
    for (auto &r : web.ranges)
        for (auto &p : r.points) {
            if (lineMarkers.find(p.line) == lineMarkers.end())
                lineMarkers[p.line] = p.marker;
            else if (p.marker != ' ')
                lineMarkers[p.line] = p.marker;
        }

    std::vector<std::pair<int,char>> sorted(lineMarkers.begin(), lineMarkers.end());
    // Already sorted by line (std::map orders by key)

    size_t mid = sorted.size() / 2;

    // Build first half live range
    LiveRange lrA;
    lrA.varName = web.varName;
    for (size_t i = 0; i < mid; i++)
        lrA.points.push_back({sorted[i].first, sorted[i].second});
    // Ensure last point of first half has '-' marker (last use)
    if (!lrA.points.empty() && lrA.points.back().marker == ' ')
        lrA.points.back().marker = '-';

    // Build second half live range
    LiveRange lrB;
    lrB.varName = web.varName;
    for (size_t i = mid; i < sorted.size(); i++)
        lrB.points.push_back({sorted[i].first, sorted[i].second});
    // Ensure first point of second half has '+' marker (new def)
    if (!lrB.points.empty() && lrB.points.front().marker == ' ')
        lrB.points.front().marker = '+';

    outA = Web(idA, web.varName);
    outA.ranges = {lrA};

    outB = Web(idB, web.varName);
    outB.ranges = {lrB};

    std::cout << "[split] web" << web.webId << " (" << web.varName << ")"
              << " → web" << idA << " [" << lrA.firstLine() << ".." << lrA.lastLine() << "]"
              << " + web" << idB << " [" << lrB.firstLine() << ".." << lrB.lastLine() << "]\n";
}

// ---------------------------------------------------------------------------
// allocate — split K largest webs, rebuild graph, run coloring
// ---------------------------------------------------------------------------
InterferenceGraph SplitAllocator::allocate(std::vector<Web> &webs,
                                            int numRegisters,
                                            int numSplits) {
    // Sort webs by line count descending — split the biggest ones
    std::vector<Web> sorted = webs;
    std::sort(sorted.begin(), sorted.end(), [](const Web &a, const Web &b){
        return a.allLines().size() > b.allLines().size();
    });

    // Clamp numSplits
    numSplits = std::min(numSplits, static_cast<int>(webs.size()));

    // Build new web list: replace top K with their two halves
    std::vector<Web> newWebs;
    int nextId = 0;

    // First, add all webs NOT being split (with new sequential IDs)
    std::set<int> splitIds;
    for (int i = 0; i < numSplits; i++)
        splitIds.insert(sorted[i].webId);

    for (auto &w : webs) {
        if (!splitIds.count(w.webId)) {
            Web copy = w;
            copy.webId = nextId++;
            newWebs.push_back(copy);
        }
    }

    // Then add the split halves
    for (int i = 0; i < numSplits; i++) {
        Web outA(0, ""), outB(0, "");
        splitWeb(sorted[i], nextId, nextId + 1, outA, outB);
        nextId += 2;
        if (!outA.ranges.empty() && !outA.ranges[0].points.empty())
            newWebs.push_back(outA);
        if (!outB.ranges.empty() && !outB.ranges[0].points.empty())
            newWebs.push_back(outB);
    }

    // Update caller's webs
    webs = newWebs;

    // Rebuild interference graph with new webs
    auto ig = InterferenceGraphBuilder::build(newWebs);

    // Run basic greedy coloring
    RegisterAllocator::allocateBasic(ig, numRegisters);

    return ig;
}
