#ifndef WEB_H
#define WEB_H

#include "LiveRange.h"
#include <vector>
#include <string>
#include <set>

/// Special color values
static constexpr int COLOR_NONE    = -1;  ///< Not yet colored
static constexpr int COLOR_SPILLED = -2;  ///< Spilled to memory

/**
 * @brief A Web is the union of one or more live ranges of the same variable
 *        that share at least one program point.
 *
 * Webs are the nodes of the interference graph. Two webs interfere if
 * there exists at least one program point where both are simultaneously live.
 */
struct Web {
    int webId;                      ///< Unique identifier (used as Graph node key)
    std::string varName;            ///< Variable name this web belongs to
    std::vector<LiveRange> ranges;  ///< Constituent live ranges (after merging)

    int color;                      ///< Assigned register index, or COLOR_NONE / COLOR_SPILLED
    bool active;                    ///< Used during graph coloring simplification phase

    Web(int id, const std::string &var)
        : webId(id), varName(var), color(COLOR_NONE), active(true) {}

    /**
     * @brief Returns the set of all program lines covered by this web.
     * @complexity O(R * L) where R = number of ranges, L = avg lines per range
     */
    std::set<int> allLines() const {
        std::set<int> s;
        for (auto &r : ranges)
            for (auto &p : r.points)
                s.insert(p.line);
        return s;
    }

    /**
     * @brief Returns the marker ('+', '-', ' ') for a given line, or ' ' if not present.
     */
    char markerAt(int line) const {
        for (auto &r : ranges)
            for (auto &p : r.points)
                if (p.line == line) return p.marker;
        return ' ';
    }

    /// True if this web is assigned to memory (spilled)
    bool isSpilled() const { return color == COLOR_SPILLED; }

    /// True if this web has a register assigned
    bool isColored() const { return color >= 0; }
};

#endif // WEB_H