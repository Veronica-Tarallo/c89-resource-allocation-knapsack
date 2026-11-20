# Test Cases & Reference Outputs

This directory contains the complete suite of sample input event logs (`Esempio00.txt`-`Esempio06.txt`) and their corresponding expected reference outputs (`soluzione00.txt`-`soluzione06.txt`), originally provided as verification benchmarks for the *Algorithms and Data Structures* laboratory (*Università degli Studi di Milano*).

## Files

- `Esempio00.txt` / `soluzione00.txt` - Introductory scenario covering entity registration, project proposals, withdrawals, cancellations, budget-constrained selections, and year-end reallocation.
- `Esempio01.txt` / `soluzione01.txt` - Edge-case scenario testing selection under insufficient funds, empty available project lists, and active year-end committee reallocation.
- `Esempio02.txt` - `Esempio06.txt` / `soluzione02.txt` - `soluzione06.txt` - Large-scale stress tests involving 10 entities, 60 project proposals, multiple sequential selections, mid-year withdrawals and cancellations, and year-end diversification heuristics.

## Usage

From the root directory of the repository, compile the solver and run any test case:

    gcc -std=c89 -Wall -Wextra -pedantic -O2 *.c -o resource_allocator
    ./resource_allocator examples/Esempio00.txt
