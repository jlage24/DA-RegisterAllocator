#include "Parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

std::string Parser::trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

LinePoint Parser::parseLinePoint(const std::string &token) {
    std::string t = trim(token);
    if (t.empty())
        throw std::runtime_error("Empty line-point token");

    LinePoint lp;
    lp.marker = ' ';

    // Check last character for marker
    char last = t.back();
    if (last == '+' || last == '-') {
        lp.marker = last;
        t = t.substr(0, t.size() - 1);
    }

    try {
        lp.line = std::stoi(t);
    } catch (...) {
        throw std::runtime_error("Invalid line number in token: '" + token + "'");
    }

    if (lp.line <= 0)
        throw std::runtime_error("Line numbers must be positive, got: " + std::to_string(lp.line));

    return lp;
}

std::vector<LiveRange> Parser::parseLiveRanges(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open live ranges file: " + filename);

    std::vector<LiveRange> ranges;
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        std::string trimmed = trim(line);

        // Skip empty lines and comments
        if (trimmed.empty() || trimmed[0] == '#')
            continue;

        // Find the colon separating variable name from points
        size_t colonPos = trimmed.find(':');
        if (colonPos == std::string::npos)
            throw std::runtime_error("Missing ':' on line " + std::to_string(lineNum) + ": " + trimmed);

        std::string varName = trim(trimmed.substr(0, colonPos));
        std::string pointsPart = trim(trimmed.substr(colonPos + 1));

        if (varName.empty())
            throw std::runtime_error("Empty variable name on line " + std::to_string(lineNum));

        LiveRange lr;
        lr.varName = varName;

        // Parse comma-separated line points
        std::stringstream ss(pointsPart);
        std::string token;
        while (std::getline(ss, token, ',')) {
            std::string t = trim(token);
            if (!t.empty())
                lr.points.push_back(parseLinePoint(t));
        }

        if (lr.points.empty())
            throw std::runtime_error("Live range with no points on line " + std::to_string(lineNum));

        // Sort points by line number (defensive -> input should be ordered)
        std::sort(lr.points.begin(), lr.points.end());

        ranges.push_back(lr);
    }

    if (ranges.empty())
        std::cerr << "Warning: no live ranges found in " << filename << "\n";

    return ranges;
}

RegisterConfig Parser::parseRegisters(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open registers file: " + filename);

    RegisterConfig config;
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#')
            continue;

        size_t colonPos = trimmed.find(':');
        if (colonPos == std::string::npos)
            throw std::runtime_error("Missing ':' on line " + std::to_string(lineNum));

        std::string key   = trim(trimmed.substr(0, colonPos));
        std::string value = trim(trimmed.substr(colonPos + 1));

        if (key == "registers") {
            try {
                config.numRegisters = std::stoi(value);
            } catch (...) {
                throw std::runtime_error("Invalid register count: '" + value + "'");
            }
            if (config.numRegisters <= 0)
                throw std::runtime_error("Number of registers must be positive");

        } else if (key == "algorithm") {
            // Possible formats: "basic", "free", "spilling, 2", "splitting, 2"
            size_t commaPos = value.find(',');
            if (commaPos == std::string::npos) {
                config.algorithm = trim(value);
                config.algorithmParam = 0;
            } else {
                config.algorithm = trim(value.substr(0, commaPos));
                std::string paramStr = trim(value.substr(commaPos + 1));
                try {
                    config.algorithmParam = std::stoi(paramStr);
                } catch (...) {
                    throw std::runtime_error("Invalid algorithm parameter: '" + paramStr + "'");
                }
            }
        } else {
            std::cerr << "Warning: unknown key '" << key << "' on line " << lineNum << "\n";
        }
    }

    // Validate
    if (config.numRegisters == 0)
        throw std::runtime_error("registers.txt missing 'registers: N' entry");
    if (config.algorithm.empty())
        throw std::runtime_error("registers.txt missing 'algorithm: ...' entry");

    return config;
}