# 🚦 Smart City Traffic Optimizer

An algorithmic simulation engine designed in standard C to optimize, route, and map urban vehicular networks using classic Graph Theory and Sorting algorithms.

---

## 🚀 Key Frameworks & Algorithms
This platform utilizes several data structures and core computer science algorithms:
* **Greedy Signal Allocation**: Dynamic distribution of dynamic green light cycles depending on cross-lane traffic density.
* **Merge Sort ($O(n \log n)$)**: Structural ranking of city intersections prioritized by vehicle counts.
* **Breadth-First Search (BFS)**: Level-by-level road segment tracking and network discovery.
* **Dijkstra's Algorithm**: Finding the absolute fastest transit path minimizing trip durations.
* **Prim's & Kruskal's MST**: Calculating optimal structural cost frameworks for minimizing interconnected spanning road networks.

---

## 🛠 Compilation and Setup

### Prerequisites
Make sure you have a working C compiler installed:
* **GCC / MinGW**
* **Make tool engine** (Optional, but highly recommended)

### Quick Start CLI Execution
Clone the repository and run the following terminal lines:

```bash
# Clone the repository
git clone https://github.com
cd smart-city-traffic-optimizer

# Build using Makefile
make

# Execute application
./traffic_optimizer
```

*If you do not have `make` installed, build natively with GCC:*
```bash
gcc -Wall src/main.c -o traffic_optimizer
./traffic_optimizer
```

---

## 📊 Technical Complexity Matrix

| Algorithm | Data Structure Used | Time Complexity | Purpose |
| :--- | :--- | :--- | :--- |
| **Greedy Allocation** | C Structure Structs | $O(1)$ | Adaptive light scaling |
| **Merge Sort** | Arrays / Pointers | $O(V \log V)$ | Structural traffic rankings |
| **BFS** | Custom Array Queue | $O(V^2)$ (Matrix-bound) | Map level connectivity check |
| **Dijkstra** | Parent Reference Map | $O(V^2)$ | Fastest target single-source path |
| **Prim's MST** | Matrix Lookups | $O(V^2)$ | Node connectivity layout optimization |
| **Kruskal's MST** | Disjoint Sets / Union-Find | $O(E \log E)$ | Edge-weighted mesh optimizations |
