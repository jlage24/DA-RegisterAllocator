#ifndef PARSER_H
#define PARSER_H

#include "../graph/LiveRange.h"
#include <vector>
#include <string>

/**
 * @brief Configuration loaded from the registers input file.
 */
struct RegisterConfig {
    int numRegisters = 0;       ///< K -> number of physical registers available
    std::string algorithm;      ///< "basic", "spilling", "splitting", or "free"
    int algorithmParam = 0;     ///< Numeric parameter K for spilling/splitting (0 if unused)
};

/**
 * @brief Parses the two input files required by the register allocator.
 *
 * Live ranges file format (ranges.txt):
 * @code
 *   # comment
 *   varName: line+, line, line, line-
 *   varName: line+, line-
 * @endcode
 *
 * Registers file format (registers.txt):
 * @code
 *   # comment
 *   registers: N
 *   algorithm: basic         (or spilling,K or splitting,K or free)
 * @endcode
 */
class Parser {
public:
    /**
     * @brief Parses a live ranges file.
     * @param filename Path to the ranges file.
     * @return Vector of LiveRange structs (one per input line, before web merging).
     * @throws std::runtime_error on file not found or malformed input.
     * @complexity O(L) where L = total number of line-point tokens in the file.
     */
    static std::vector<LiveRange> parseLiveRanges(const std::string &filename);

    /**
     * @brief Parses a register configuration file.
     * @param filename Path to the registers file.
     * @return Populated RegisterConfig struct.
     * @throws std::runtime_error on file not found or malformed input.
     * @complexity O(1) — file has constant number of meaningful lines.
     */
    static RegisterConfig parseRegisters(const std::string &filename);

private:
    /// Strips leading/trailing whitespace from a string
    static std::string trim(const std::string &s);

    /// Parses a single line-point token like "7+", "8", "10-"
    static LinePoint parseLinePoint(const std::string &token);
};

#endif // PARSER_H