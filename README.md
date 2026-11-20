# Dynamic Resource Allocation & Combinatorial Optimization Engine (ANSI C / C89)

A modular, event-driven simulation and combinatorial optimization engine implemented from scratch in **ANSI C (C89)** for the *Algorithms and Data Structures* laboratory (*Università degli Studi di Milano*).

The system models an annual funding allocation framework where public/private funding entities evaluate and finance project proposals based on multi-criteria preference scoring, strict budget constraints, and year-end diversification heuristics.

---

## Key Algorithmic & Software Engineering Features

* **Modular C89 Architecture & Manual Memory Management:**
  * Designed a clean multi-file architecture separating data structures (`item`, `listaelementi`), sorting routines (`heapsort`), dynamic programming solvers (`knapsack`), safe memory wrappers (`alloc`), and event orchestration (`finanziamenti`).
  * Managed dynamic sets of registered entities, available proposals, and funded projects using circular doubly linked lists with sentinel nodes.
* **Exact Combinatorial Optimization (0/1 Knapsack):**
  * Formulated each entity's budget-constrained project selection as a **0/1 Knapsack problem**, maximizing total multi-criteria preference score within the entity's available budget $V$.
  * Implemented an iterative bottom-up **Dynamic Programming** solver (`PDiterativaKP`) running in $\mathcal{O}(n \cdot V)$ time and space with backtracking solution reconstruction.
* **NP-Hard Year-End Diversification Heuristic:**
  * Addressed the year-end reallocation phase—an NP-hard subset selection problem maximizing pairwise distance (diversity) within pooled residual funds—via a multi-start **greedy diversification heuristic** (`Ripescaggio` / `TrovaInsiemeMaxDiversita`).
* **In-Place Sorting & Asymptotic Complexity Analysis:**
  * Built a custom **HeapSort** module (`heapsort`) guaranteeing $\mathcal{O}(n \log n)$ worst-case time complexity and $\mathcal{O}(1)$ auxiliary space for ordering project IDs.

---

## Repository Structure

```text
├── finanziamenti.c      # Main entry point, CLI parser, event loop, and year-end reallocation
├── knapsack.h / .c      # Data structures and memory routines for the 0/1 Knapsack solver
├── heapsort.h / .c      # In-place max-heap construction and HeapSort implementation
├── listaelementi.h / .c # Circular doubly linked list ADT, preference scoring, and diversity metrics
├── item.h / .c          # Core record definitions for projects and funding entities
└── alloc.h / .c         # Safe dynamic memory allocation wrappers for 1D/2D integer arrays
