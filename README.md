# DA2026 - PRJ2

## About the Project
This project implements a compiler back-end register allocation tool using graph coloring algorithms. It parses program variables from a three-address intermediate representation, builds an interference graph by fusing overlapping live ranges into **webs**, and assigns these webs to physical registers.

This project was developed for the **Analysis and Synthesis of Algorithms / Design of Algorithms (DA)** course at FEUP (Spring 2026).

## Features and Algorithms
The allocator supports four different strategies:

- **Basic (Greedy Graph Coloring):** Implements Chaitin's graph coloring approach. If the graph cannot be colored with $$K$$ registers, it safely aborts and spills all webs to memory.
- **Spilling:** Uses a heuristic to pre-spill the $$K$$ webs with the longest lifetimes (most lines covered) before attempting the basic coloring.
- **Splitting:** Slices the most congested webs in half to break interference edges, reducing the overall chromatic number of the graph.
- **Free (Adaptive Split-then-Spill):** A surgical, custom-designed algorithm that targets only the most congested webs with degree $$\geq N$$. It attempts to split them first, and if the split does not resolve the interference, it falls back to spilling.

## Prerequisites
- **C++17** or higher
- **CMake** (for building)
- **Doxygen** (for documentation)
- **Graphviz** (optional, to visualize `.dot` graph exports)

## Build Instructions
To compile the project, run the following commands from the root directory:

```bash
mkdir -p build
cd build
cmake ..
make
```

This generates the `regalloc` executable.

## Usage
The application supports two execution modes.

### 1. Batch Mode
Designed for automated testing and scripting.

```bash
./regalloc -b <path/to/ranges.txt> <path/to/registers.txt> <path/to/output.txt>
```

### 2. Interactive Mode
A text-based user interface (TUI) for manual testing and demonstration.

```bash
./regalloc
```

From the interactive menu, it is possible to load files, build the interference graph, run the allocation, and export the graph to a Graphviz `.dot` file.

## Documentation
The source code is fully documented. To generate the HTML documentation:

```bash
doxygen Doxyfile
```

Then open `docs/html/index.html` in a web browser. The documentation includes explicit time complexity analysis for the core heuristic algorithms.

## Authors
- João Lage
- Mafalda Pacheco
- Group - Class 15
