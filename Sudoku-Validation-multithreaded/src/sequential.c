#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

#define MAX_N 64  

// Function to check if a row is valid
bool check_row(int sudoku[MAX_N][MAX_N], int N, int row, FILE *output) {
    bool seen[MAX_N + 1] = {false};

    for (int i = 0; i < N; i++) {
        int num = sudoku[row][i];
        if (num < 1 || num > N || seen[num]) {
            fprintf(output, "checks row %d and is invalid.\n", row + 1);
            return false;
        }
        seen[num] = true;
    }
    fprintf(output, "checks row %d and is valid.\n", row + 1);
    return true;
}

// Function to check if a column is valid
bool check_column(int sudoku[MAX_N][MAX_N], int N, int col, FILE *output) {
    bool seen[MAX_N + 1] = {false};

    for (int i = 0; i < N; i++) {
        int num = sudoku[i][col];
        if (num < 1 || num > N || seen[num]) {
            fprintf(output, "checks column %d and is invalid.\n", col + 1);
            return false;
        }
        seen[num] = true;
    }
    fprintf(output, "checks column %d and is valid.\n", col + 1);
    return true;
}

// Function to check if a subgrid is valid
bool check_subgrid(int sudoku[MAX_N][MAX_N], int N, int r_start, int c_start, FILE *output) {
    bool seen[MAX_N + 1] = {false};
    int sub_size = (int)sqrt(N);

    for (int i = r_start; i < r_start + sub_size; i++) {
        for (int j = c_start; j < c_start + sub_size; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > N || seen[num]) {
                fprintf(output, "checks subgrid starting at (%d, %d) and is invalid.\n", r_start + 1, c_start + 1);
                return false;
            }
            seen[num] = true;
        }
    }
    fprintf(output, "checks subgrid starting at (%d, %d) and is valid.\n", r_start + 1, c_start + 1);
    return true;
}

// Function to validate the entire Sudoku
bool validate_sudoku(int sudoku[MAX_N][MAX_N], int N, FILE *output) {
    for (int i = 0; i < N; i++) {
        if (!check_row(sudoku, N, i, output)) return false;
        if (!check_column(sudoku, N, i, output)) return false;
    }

    int subgrid_size = (int)sqrt(N);
    for (int i = 0; i < N; i += subgrid_size) {
        for (int j = 0; j < N; j += subgrid_size) {
            if (!check_subgrid(sudoku, N, i, j, output)) return false;
        }
    }

    return true;
}

int main() {
    int K, N;
    FILE *input = fopen("inp.txt", "r");
    if (input == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    fscanf(input, "%d %d", &K, &N);

    if (sqrt(N) != (int)sqrt(N)) {
        printf("Error: N is not a perfect square.\n");
        fclose(input);
        return 1;
    }

    int sudoku[MAX_N][MAX_N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fscanf(input, "%d", &sudoku[i][j]) != 1) {
                printf("Error reading sudoku value at (%d, %d).\n", i, j);
                fclose(input);
                return 1;
            }
        }
    }
    fclose(input);

    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }


#ifdef _WIN32
    LARGE_INTEGER fre, s, e;
    QueryPerformanceFrequency(&fre);
    QueryPerformanceCounter(&s);
#else
    struct timespec s, e;
    clock_gettime(CLOCK_MONOTONIC, &s);
#endif

    fprintf(output, "Validating Sudoku...\n");

    bool is_valid = validate_sudoku(sudoku, N, output);

    fprintf(output, "Sudoku is %s.\n", is_valid ? "valid" : "invalid");

#ifdef _WIN32
    QueryPerformanceCounter(&e);
    double time_taken = (double)(e.QuadPart - s.QuadPart) * 1e6 / fre.QuadPart;
#else
    clock_gettime(CLOCK_MONOTONIC, &);
    double time_taken = (e.tv_sec - s.tv_sec) * 1e6 + (e.tv_nsec - s.tv_nsec) / 1e3;
#endif

    fprintf(output, "The total time taken is %.2f microseconds.\n", time_taken);

    fclose(output);
    return 0;
}
