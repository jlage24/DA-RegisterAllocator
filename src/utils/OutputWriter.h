#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H

#include "../graph/InterferenceGraph.h"
#include <string>

/**
 * @brief Writes the register allocation result to a file and/or stdout.
 *
 * Output format:
 * @code
 *   webs: N
 *   web0: 1+,2,3,4,5,6-
 *   web1: 9+,10,11,12-
 *   ...
 *   registers: K
 *   r0: web0
 *   r0: web2
 *   r1: web1
 *   M: web3
 * @endcode
 */
class OutputWriter {
public:
    /**
     * @brief Writes allocation result to a file.
     * @param ig       The interference graph with colored webs.
     * @param filename Output file path.
     */
    static void writeToFile(const InterferenceGraph &ig, const std::string &filename);

    /**
     * @brief Prints allocation result to stdout.
     * @param ig The interference graph with colored webs.
     */
    static void printToConsole(const InterferenceGraph &ig);

private:
    /// Formats a single web's ranges as "1+,2,3,4-"
    static std::string formatWeb(const Web &web);

    /// Core writer — outputs to any ostream
    static void write(const InterferenceGraph &ig, std::ostream &out);
};

#endif // OUTPUTWRITER_H