/**
 * @file LiveRange.cpp
 * @brief Implementation of the LiveRange helper methods.
 *
 * A LiveRange represents one contiguous "alive" interval of a variable.
 * It holds an ordered list of LinePoints (program line numbers with optional
 * '+' / '-' markers).
 *
 * This file implements the three helper methods declared in LiveRange.h:
 *   - startLine()     : returns the first line number in this range
 *   - endLine()       : returns the last line number in this range
 *   - overlapsWith()  : checks if two ranges share at least one line number
 *
 * These helpers are used heavily by WebBuilder when deciding whether to
 * merge two live ranges into the same web.
 */

#include "LiveRange.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// int LiveRange::startLine() const
// -----------------------------------------------------------------------------
// Returns the line number of the FIRST point in this live range.
// The points vector is assumed to be ordered (as read from the input file).
// Example: for range "1+,2,3,4,5,6-" this returns 1.
//
// TODO: return points.front().line
//       (add a safety check: if points is empty, return -1)
int LiveRange::startLine() const {
    // TODO: implement
    return -1;
}

// -----------------------------------------------------------------------------
// int LiveRange::endLine() const
// -----------------------------------------------------------------------------
// Returns the line number of the LAST point in this live range.
// Example: for range "1+,2,3,4,5,6-" this returns 6.
//
// TODO: return points.back().line
//       (add a safety check: if points is empty, return -1)
int LiveRange::endLine() const {
    // TODO: implement
    return -1;
}

// -----------------------------------------------------------------------------
// bool LiveRange::overlapsWith(const LiveRange& other) const
// -----------------------------------------------------------------------------
// Returns true if this live range and 'other' share at least one line number.
//
// This is used by WebBuilder to decide if two ranges of the same variable
// should be merged into the same web.
//
// NORMAL OVERLAP: any line number that appears in both ranges.
//
// SPECIAL CASE (must also return true):
//   If this range ends on line X with '-' (last use)
//   AND the other range starts on line X with '+' (definition),
//   they share line X and must be merged.
//   This covers patterns like "i = i + 1" where the old value of i
//   is read and a new value is written on the same line.
//
// ALGORITHM:
//   1. Collect all line numbers from this range into a set.
//   2. For each line number in 'other', check if it exists in the set.
//   3. If any match is found, return true.
//   4. Return false if loop finishes with no match.
//
// Time complexity: O(n log n) where n = total number of points across both ranges.
//
// TODO: build a set of line numbers from this->points
// TODO: iterate over other.points and check if any line is in the set
// TODO: return true on first match, false if no match found
bool LiveRange::overlapsWith(const LiveRange& other) const {
    // TODO: implement
    return false;
}
