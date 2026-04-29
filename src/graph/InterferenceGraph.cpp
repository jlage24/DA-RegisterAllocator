#include "InterferenceGraph.h"
#include <iostream>
#include <fstream>

void InterferenceGraph::addWeb(const Web &web) {
    addVertex(web.webId);
    webs.emplace(web.webId, web);
}

void InterferenceGraph::addUndirectedEdge(int a, int b) {
    if (a == b) return;
    if (!hasEdge(a, b)) {
        addEdge(a, b, 1.0);
        addEdge(b, a, 1.0);
    }
}

bool InterferenceGraph::hasEdge(int a, int b) const {
    auto va = findVertex(a);
    if (!va) return false;
    for (auto &e : va->getAdj())
        if (e.getDest()->getInfo() == b) return true;
    return false;
}

int InterferenceGraph::degree(int webId) const {
    auto v = findVertex(webId);
    if (!v) return 0;
    int deg = 0;
    for (auto &e : v->getAdj()) {
        int neighId = e.getDest()->getInfo();
        // Only count active neighbours
        auto it = webs.find(neighId);
        if (it != webs.end() && it->second.active)
            deg++;
    }
    return deg;
}

void InterferenceGraph::emitUndirectedDOT(const std::string &filename) const {
    std::ofstream f(filename + ".gv");
    f << "graph interference {\n";
    f << "  node [shape=ellipse fontname=calibri];\n\n";

    // Print nodes with label = webId (varName)
    for (auto &[id, web] : webs) {
        std::string label = "web" + std::to_string(id) + "\\n(" + web.varName + ")";
        if (web.isSpilled())
            f << "  " << id << " [label=\"" << label << "\" style=filled fillcolor=lightcoral];\n";
        else if (web.isColored())
            f << "  " << id << " [label=\"" << label << " r" << web.color << "\" style=filled fillcolor=lightblue];\n";
        else
            f << "  " << id << " [label=\"" << label << "\"];\n";
    }
    f << "\n";

    // Print edges (undirected — only once per pair)
    for (auto v : getVertexSet()) {
        int a = v->getInfo();
        for (auto &e : v->getAdj()) {
            int b = e.getDest()->getInfo();
            if (a < b) // avoid duplicates
                f << "  " << a << " -- " << b << ";\n";
        }
    }
    f << "}\n";
    f.close();
}

void InterferenceGraph::print() const {
    std::cout << "=== Interference Graph ===\n";
    std::cout << "Nodes (webs): " << getNumVertex() << "\n\n";
    for (auto &[id, web] : webs) {
        std::cout << "  web" << id << " (" << web.varName << ")"
                  << "  active=" << (web.active ? "yes" : "no")
                  << "  color=";
        if (web.color == COLOR_NONE)    std::cout << "none";
        else if (web.color == COLOR_SPILLED) std::cout << "M";
        else std::cout << "r" << web.color;
        std::cout << "  degree=" << degree(id) << "\n";

        std::cout << "    interferes with: ";
        auto v = findVertex(id);
        if (v) {
            for (auto &e : v->getAdj())
                std::cout << "web" << e.getDest()->getInfo() << " ";
        }
        std::cout << "\n";
    }
    std::cout << "==========================\n";
}