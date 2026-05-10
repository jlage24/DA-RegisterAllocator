#include "OutputWriter.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <sstream>

std::string OutputWriter::formatWeb(const Web &web) {
    // Collect all points across all ranges, sorted by line
    std::map<int, char> lineMarkers;
    for (auto &r : web.ranges) {
        for (auto &p : r.points) {
            // If a line appears in multiple ranges, prefer '+' and '-' over ' '
            if (lineMarkers.find(p.line) == lineMarkers.end())
                lineMarkers[p.line] = p.marker;
            else if (p.marker != ' ')
                lineMarkers[p.line] = p.marker;
        }
    }

    std::ostringstream oss;
    bool first = true;
    for (auto &[line, marker] : lineMarkers) {
        if (!first) oss << ",";
        first = false;
        oss << line;
        if (marker != ' ') oss << marker;
    }
    return oss.str();
}

// Core output logic
void OutputWriter::write(const InterferenceGraph &ig, std::ostream &out) {
    // Webs
    out << "webs: " << ig.webs.size() << "\n";
    for (auto &[id, web] : ig.webs)
        out << "web" << id << ": " << formatWeb(web) << "\n";

    // Register assignments
    // Find max color used (to report number of registers actually used)
    int maxColor = -1;
    for (auto &[id, web] : ig.webs)
        if (web.color >= 0 && web.color > maxColor)
            maxColor = web.color;

    out << "registers: " << (maxColor + 1) << "\n";

    // Group webs by color
    std::map<int, std::vector<int>> byColor;
    std::vector<int> spilled;
    for (auto &[id, web] : ig.webs) {
        if (web.color == COLOR_SPILLED) spilled.push_back(id);
        else if (web.color >= 0)        byColor[web.color].push_back(id);
    }

    // Print register assignments
    for (auto &[color, webIds] : byColor)
        for (int wid : webIds)
            out << "r" << color << ": web" << wid << "\n";

    // Print spilled webs
    for (int wid : spilled)
        out << "M: web" << wid << "\n";
}

// Public interface
void OutputWriter::writeToFile(const InterferenceGraph &ig,
                                const std::string &filename) {
    std::ofstream f(filename);
    if (!f.is_open())
        throw std::runtime_error("Cannot open output file: " + filename);
    write(ig, f);
    std::cout << "[output] Written to " << filename << "\n";
}

void OutputWriter::printToConsole(const InterferenceGraph &ig) {
    write(ig, std::cout);
}