/**
 * @file Web.cpp
 * @brief Implementation of the Web helper methods.
 *
 * A Web is the central concept of this project.
 * It represents the union of one or more LiveRanges of the SAME variable
 * that overlap at some program point.
 *
 * Each Web becomes ONE NODE in the Interference Graph.
 *
 * This file implements:
 *   - addRange()         : merges a new LiveRange into this web
 *   - interfersWith()    : checks if this web overlaps with another web
 *                          (used to decide if an edge exists in the graph)
 *   - getSortedPoints()  : returns all line points sorted by line number
 *                          (used for output formatting)
 */

#include "Web.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// void Web::addRange(const LiveRange& lr)
// -----------------------------------------------------------------------------
// Adds a LiveRange to this web and updates the allLines set.
//
// allLines is a std::set<int> that holds the union of ALL line numbers
// covered by this web across all its ranges. It is used for fast
// interference checking (see interfersWith below).
//
// STEPS:
//   1. Push 'lr' into the ranges vector.
//   2. For each LinePoint in lr.points, insert its line number into allLines.
//
// TODO: ranges.push_back(lr)
// TODO: for each point in lr.points -> allLines.insert(point.line)
void Web::addRange(const LiveRange& lr) {
    // TODO: implement
}

// -----------------------------------------------------------------------------
// bool Web::interfersWith(const Web& other) const
// -----------------------------------------------------------------------------
// Returns true if this web and 'other' interfere (i.e., an edge must exist
// between them in the interference graph).
//
// Two webs INTERFERE if there is at least one program point (line number)
// where BOTH webs are alive simultaneously.
//
// IMPORTANT EXCEPTION - they do NOT interfere at line X if:
//   - One web has a DEFINITION ('+') at line X   (it starts being written)
//   - The other web has a LAST USE ('-') at line X (it just finished being read)
//   This means the new value starts exactly when the old one is no longer needed.
//   This check is done more precisely in InterferenceGraphBuilder,
//   but a basic version can live here too.
//
// SIMPLE ALGORITHM (basic version, no exception handling):
//   1. Find the intersection of this->allLines and other.allLines.
//   2. If the intersection is non-empty, return true.
//
// NOTE: The full exception check ('+'/'-' on the same line) is handled
// in InterferenceGraphBuilder::interferes() for precision.
// This method can do the quick set-intersection check.
//
// TODO: iterate over the smaller of the two allLines sets
// TODO: check if each element exists in the other set
// TODO: return true on first hit, false if no shared line found
bool Web::interfersWith(const Web& other) const {
    // TODO: implement
    return false;
}

// -----------------------------------------------------------------------------
// std::vector<LinePoint> Web::getSortedPoints() const
// -----------------------------------------------------------------------------
// Returns ALL line points from ALL ranges of this web, sorted by line number
// in ascending order.
//
// This is used by OutputWriter to format the web listing in the output file.
// Example output: "web0: 1+,2,3,4,5,6-,9+,10,11,12-"
//
// STEPS:
//   1. Collect all LinePoints from all ranges into one vector.
//   2. Sort by line number (ascending).
//   3. Remove duplicate line numbers if any (keep the one with +/- markers).
//
// TODO: iterate over all ranges, collect all points into a single vector
// TODO: sort the vector by point.line
// TODO: (optional) deduplicate: if two points have the same line,
//       keep the one that has isDef=true or isLastUse=true (has a marker)
std::vector<LinePoint> Web::getSortedPoints() const {
    // TODO: implement
    return {};
}
