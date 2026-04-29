/**
 * @file WebBuilder.cpp
 * @brief Implementation of WebBuilder - merges LiveRanges into Webs.
 *
 * This file is also part of Mafalda's work.
 * It takes the output of Parser (a map of varName -> LiveRanges) and
 * groups overlapping ranges into Webs using a greedy merge algorithm.
 *
 * ============================================================
 * WHAT IS A WEB?
 * ============================================================
 * A Web = the union of LiveRanges of the SAME variable that overlap.
 * Merging is TRANSITIVE: if A overlaps B and B overlaps C -> A, B, C same web.
 *
 * ============================================================
 * MERGE RULES (from the project spec):
 * ============================================================
 * Two live ranges of the same variable must be merged if:
 *   RULE 1: They share at least one line number.
 *   RULE 2: One ends on line X with '-' AND the other starts on line X with '+'.
 *           (This is actually covered by Rule 1 if both include line X,
 *            but check carefully - the '-' range's last point IS line X,
 *            and the '+' range's first point IS also line X.)
 *
 * ============================================================
 * ALGORITHM (greedy union-find style):
 * ============================================================
 * For each variable:
 *   1. Start: each LiveRange is its own "candidate web" (a list of ranges).
 *   2. Repeat until no more merges happen in a full pass:
 *      a. For each pair of candidate webs, check if any range in web A
 *         overlaps with any range in web B.
 *      b. If yes, merge web B into web A (combine their range lists).
 *      c. Remove web B from the candidate list.
 *   3. Each surviving candidate web becomes a final Web object.
 * After processing all variables, assign unique integer IDs (0, 1, 2, ...).
 *
 * ============================================================
 * EXAMPLE:
 * ============================================================
 * Input for variable "i":
 *   range1: 1+,2,3,4,5,6-
 *   range2: 9+,10,11,12-
 *   range3: 12+,13,14-      <- ends/starts at 12 -> merges with range2
 *   range4: 20+,11,12-      <- shares line 11,12 with range2 -> merges
 *
 * Expected output: TWO webs for "i":
 *   web_A: range1 alone    (lines 1-6, no overlap with others)
 *   web_B: range2+range3+range4 merged (lines 9-14 + 20)
 */

#include "WebBuilder.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// std::vector<Web> WebBuilder::buildWebs(...)
// -----------------------------------------------------------------------------
// Main entry point. Processes all variables and returns the final web list.
//
// HIGH-LEVEL STEPS:
//   1. For each variable in the map:
//      a. Get its list of LiveRanges.
//      b. Run the greedy merge loop (see algorithm above).
//      c. Each resulting merged group becomes one Web.
//   2. Assign a unique integer ID to each web (starting from 0).
//   3. For each web, call web.addRange() for each of its ranges
//      (this also populates web.allLines).
//   4. Set web.assignedRegister = -1 for all (unassigned initially).
//   5. Return the complete list of webs.
//
// TODO: implement the outer loop over the map
// TODO: implement the greedy merge inner loop (or delegate to a helper)
// TODO: assign IDs and initialize webs properly
std::vector<Web> WebBuilder::buildWebs(const std::map<std::string, std::vector<LiveRange>>& rangesByVar) {
    std::vector<Web> result;
    int nextId = 0;

    // TODO: for each variable in rangesByVar:
    //   - start with each LiveRange as its own candidate group
    //   - repeatedly merge groups that overlap (call shouldMerge)
    //   - create a Web for each final group, assign ID, call addRange for each range

    return result;
}

// -----------------------------------------------------------------------------
// bool WebBuilder::shouldMerge(const LiveRange& a, const LiveRange& b) const
// -----------------------------------------------------------------------------
// Returns true if two LiveRanges of the SAME variable should be merged
// into the same web.
//
// This is true if they share at least one line number (Rule 1 + Rule 2 combined,
// since the special '+'/'-' case still means they share a line number).
//
// STEPS:
//   1. Delegate to a.overlapsWith(b) (implemented in LiveRange.cpp).
//      That already handles the shared-line check.
//
// NOTE: This is a thin wrapper, but keeping it separate makes the
// WebBuilder logic easier to read and test independently.
//
// TODO: return a.overlapsWith(b)
bool WebBuilder::shouldMerge(const LiveRange& a, const LiveRange& b) const {
    // TODO: implement
    return false;
}

// -----------------------------------------------------------------------------
// void WebBuilder::mergeInto(LiveRange& a, const LiveRange& b) const
// -----------------------------------------------------------------------------
// Merges all points from LiveRange b into LiveRange a.
// After this call, 'a' contains the union of both ranges' points.
//
// NOTE: This is used internally during the candidate-group merging step.
// After merging into a Web, we use Web::addRange() instead.
//
// STEPS:
//   1. Append all points from b.points into a.points.
//   2. Optionally sort a.points by line number after merging
//      (makes debugging easier, not strictly required here).
//
// TODO: insert b.points into a.points
// TODO: (optional) sort a.points by line number
void WebBuilder::mergeInto(LiveRange& a, const LiveRange& b) const {
    // TODO: implement
}
