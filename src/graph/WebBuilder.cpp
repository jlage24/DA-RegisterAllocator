#include "WebBuilder.h"
#include <set>
#include <iostream>


bool WebBuilder::rangesOverlap(const LiveRange &ra, const LiveRange &rb) {
    // Must be the same variable
    if (ra.varName != rb.varName) return false;

    // Normal case: shared line
    std::set<int> linesA = ra.lineSet();
    std::set<int> linesB = rb.lineSet();

    for (int line : linesA)
        if (linesB.count(line)) return true;

    // Special case: ra ends at X- and rb starts at X+
    if (!ra.points.empty() && !rb.points.empty()) {
        // ra ends X-, rb starts X+
        if (ra.points.back().line   == rb.points.front().line &&
            ra.points.back().marker == '-' &&
            rb.points.front().marker == '+')
            return true;

        // rb ends X-, ra starts X+  (symmetric)
        if (rb.points.back().line   == ra.points.front().line &&
            rb.points.back().marker == '-' &&
            ra.points.front().marker == '+')
            return true;
    }

    return false;
}


bool WebBuilder::shouldMerge(const std::vector<LiveRange> &groupA,
                              const std::vector<LiveRange> &groupB) {
    for (const auto &ra : groupA)
        for (const auto &rb : groupB)
            if (rangesOverlap(ra, rb)) return true;
    return false;
}


std::vector<Web> WebBuilder::buildWebs(const std::vector<LiveRange> &ranges) {
    // Each live range starts in its own group
    // A group is a vector of LiveRanges that will become one Web
    std::vector<std::vector<LiveRange>> groups;
    for (const auto &r : ranges)
        groups.push_back({r});

    // Iteratively merge groups until no more merges happen
    bool merged = true;
    while (merged) {
        merged = false;

        for (size_t i = 0; i < groups.size() && !merged; i++) {
            for (size_t j = i + 1; j < groups.size() && !merged; j++) {

                // Groups must belong to the same variable
                if (groups[i][0].varName != groups[j][0].varName)
                    continue;

                if (shouldMerge(groups[i], groups[j])) {
                    // Merge group j into group i
                    for (auto &r : groups[j])
                        groups[i].push_back(r);

                    // Remove group j
                    groups.erase(groups.begin() + j);

                    // A merge happened: restart the outer loop
                    merged = true;
                }
            }
        }
    }

    // Convert each group into a Web with a unique ID
    std::vector<Web> webs;
    int id = 0;
    for (auto &group : groups) {
        Web web(id++, group[0].varName);
        web.ranges = group;
        webs.push_back(web);
    }

    return webs;
}