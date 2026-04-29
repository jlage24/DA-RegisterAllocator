#ifndef LIVERANGE_H
#define LIVERANGE_H

#include <vector>
#include <string>
#include <set>

/**
 * @brief Represents a single live range for a variable.
 *
 * A live range is a list of program line numbers over which a variable's
 * value is alive. Each line is annotated with a marker:
 *   '+' = definition (LHS assignment) at this line
 *   '-' = last use (RHS read) at this line
 *   ' ' = intermediate line (variable is live but neither defined nor last-used here)
 *
 * Example: "i: 1+, 2, 3, 4-" → lines = {(1,'+'), (2,' '), (3,' '), (4,'-')}
 */
struct LinePoint {
    int line;       ///< Program line number
    char marker;    ///< '+', '-', or ' ' (none)

    bool operator<(const LinePoint &o) const { return line < o.line; }
    bool operator==(const LinePoint &o) const { return line == o.line; }
};

/**
 * @brief A live range: ordered sequence of line points for one variable.
 */
struct LiveRange {
    std::string varName;            ///< Variable this range belongs to
    std::vector<LinePoint> points;  ///< Ordered list of program points

    /// Returns the set of plain line numbers in this range
    std::set<int> lineSet() const {
        std::set<int> s;
        for (auto &p : points) s.insert(p.line);
        return s;
    }

    /// Returns the first line of this range
    int firstLine() const { return points.empty() ? -1 : points.front().line; }

    /// Returns the last line of this range
    int lastLine() const { return points.empty() ? -1 : points.back().line; }

    /// Returns true if this range has a definition ('+') at any point
    bool hasDef() const {
        for (auto &p : points) if (p.marker == '+') return true;
        return false;
    }

    /// Returns true if this range has a last-use ('-') at any point
    bool hasUse() const {
        for (auto &p : points) if (p.marker == '-') return true;
        return false;
    }
};

#endif // LIVERANGE_H