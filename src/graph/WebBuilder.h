#ifndef WEBBUILDER_H
#define WEBBUILDER_H

#include "../graph/LiveRange.h"
#include "../graph/Web.h"
#include <vector>

/**
 * @brief Builds Webs from a list of LiveRanges.
 *
 * A Web is the union of one or more live ranges of the same variable
 * that must be merged according to the following rules:
 *   1. Two ranges of the same variable share at least one program line.
 *   2. One range ends at line X with marker '-' and another begins at
 *      line X with marker '+' (same variable — e.g. i = i + 1).
 *
 * The merging is iterative: after each fusion, the loop restarts because
 * a new merged group may now share lines with another group.
 *
 * @complexity O(R^3 * L) worst case, where R = number of live ranges
 *             and L = average lines per range. Acceptable for typical
 *             compiler inputs where R and L are small.
 */
class WebBuilder {
public:
    /**
     * @brief Builds webs from a flat list of live ranges.
     * @param ranges All live ranges parsed from the input file.
     * @return Vector of fully merged webs, each with a unique webId.
     */
    static std::vector<Web> buildWebs(const std::vector<LiveRange> &ranges);

private:
    /**
     * @brief Checks if two groups of live ranges should be merged.
     *
     * Two groups must merge if:
     *   - They belong to the same variable, AND
     *   - At least one range in groupA shares a line with one in groupB
     *     (normal case), OR one ends at X- and the other starts at X+
     *     (special case).
     *
     * @param groupA First group of live ranges.
     * @param groupB Second group of live ranges.
     * @return true if the groups must be merged.
     */
    static bool shouldMerge(const std::vector<LiveRange> &groupA,
                            const std::vector<LiveRange> &groupB);

    /**
     * @brief Checks if two individual live ranges should trigger a merge.
     * @param ra First live range.
     * @param rb Second live range.
     * @return true if ra and rb share a line or satisfy the special case.
     */
    static bool rangesOverlap(const LiveRange &ra, const LiveRange &rb);
};

#endif // WEBBUILDER_H