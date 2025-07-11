# Sudoku Validation using Multithreading 

A multithreaded Sudoku validator built using `pthreads` in C/C++, implemented as part of the Operating Systems course at IIT Hyderabad.

---

## Features

- ✅ Validates Sudoku boards (9x9, 16x16, 36x36)
- Parallelized using POSIX threads
- Multiple scheduling strategies:
  - Static Chunk Scheduling
  - Static Mixed Scheduling
  - Dynamic: TAS, CAS, and Bounded CAS

---

## Project Structure
sudoku-validation-multithreaded/
├── src/ # Source code for each scheduler
│ ├── sequential.c
│ ├── chunk.c
│ ├── mixed.c
│ └── extracredit.c
├── inputs/ # Sudoku input boards
│ └── inp.txt
├── report/ # Final PDF report
│ └── Assgn1Report.pdf
├── README.md # You're here


---

## Input Format

Your input file should be named `inp.txt` and contain:

<NUM_THREADS> <SUDOKU_SIZE>
<row1 elements>
<row2 elements>
...


✅ Example:

8 36
7 6 5 3 8 2 1 ... (continues for 36x36 grid)


---

## Compilation & Execution

🔹 Sequential Validator

gcc -o sequential src/sequential.c -lm
./sequential

🔹 Chunk Scheduler

gcc -o chunk src/chunk.c -pthread -lm
./chunk

🔹 Mixed Scheduler

gcc -o mixed src/mixed.c -pthread -lm
./mixed

🔹 Extra Credit: Dynamic Scheduling (TAS/CAS)

gcc -o extracredit src/extracredit.c -pthread -lm
./extracredit

📊 Sample Output

Thread 1 checks row 1: valid
Thread 2 checks column 1: valid
...
Sudoku is valid.
Time taken: 10520 microseconds

📈 Performance Analysis
Performance is benchmarked by measuring execution time across:

Different thread counts (e.g., 2, 4, 8, 16)

Different input sizes (9x9, 16x16, 36x36)

Different schedulers

Full results and observations are included in:
report/Assgn1Report.pdf

Course Information:

Course: Operating Systems

Program: B.Tech in Computational Engineering

Institute: Indian Institute of Technology Hyderabad

Student ID: CO23BTECH11008

This project is for academic demonstration only. Please do not reuse without permission.

