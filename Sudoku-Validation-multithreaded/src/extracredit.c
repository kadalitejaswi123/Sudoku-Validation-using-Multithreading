#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <windows.h>  

typedef struct {
    int thread_id;
    int thread_count;
    int start_idx;
    int N;
    int** sudoku;
    int type; 
    FILE* output_file;
} ThreadArgs;

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
volatile bool global_valid = true;  // Global flag for early termination

bool isSegmentValid(int* segment, int N) {
    bool* used = (bool*)calloc(N + 1, sizeof(bool));
    for (int i = 0; i < N; i++) {
        if (segment[i] < 1 || segment[i] > N || used[segment[i]]) {
            free(used);
            return false;
        }
        used[segment[i]] = true;
    }
    free(used);
    return true;
}

void* validateMixed(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int* segment = (int*)malloc(args->N * sizeof(int));

    for (int idx = args->start_idx; idx < args->N; idx += args->thread_count) {
        if (!global_valid) break;  // Early exit if another thread found an invalid segment

        bool local_valid = true;

        if (args->type == 0) { // Row validation
            for (int j = 0; j < args->N; j++) {
                segment[j] = args->sudoku[idx][j];
            }
            local_valid = isSegmentValid(segment, args->N);
        }
        else if (args->type == 1) { // Column validation
            for (int i = 0; i < args->N; i++) {
                segment[i] = args->sudoku[i][idx];
            }
            local_valid = isSegmentValid(segment, args->N);
        }
        else { // Grid validation
            int n = sqrt(args->N);
            int grid_row = (idx / n) * n;
            int grid_col = (idx % n) * n;
            int k = 0;

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    segment[k++] = args->sudoku[grid_row + i][grid_col + j];
                }
            }
            local_valid = isSegmentValid(segment, args->N);
        }

        pthread_mutex_lock(&file_mutex);
        fprintf(args->output_file, "Thread %d checks %s %d and is %s.\n",
                args->thread_id, (args->type == 0) ? "row" : (args->type == 1) ? "column" : "grid",
                idx + 1, local_valid ? "valid" : "invalid");
        pthread_mutex_unlock(&file_mutex);

        if (!local_valid) {
            global_valid = false;  // Trigger early termination
            break;
        }
    }

    free(segment);
    return NULL;
}

int main() {
    FILE *fp = fopen("inp.txt", "r");
    if (!fp) {
        printf("Error opening input file\n");
        return 1;
    }

    int K, N;
    fscanf(fp, "%d %d", &K, &N);

    int** sudoku = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        sudoku[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            fscanf(fp, "%d", &sudoku[i][j]);
        }
    }
    fclose(fp);

    FILE* out = fopen("output.txt", "w");
    if (!out) {
        printf("Error opening output file\n");
        return 1;
    }

    // Start high-precision timer
    LARGE_INTEGER fre, s, e;
    QueryPerformanceFrequency(&fre);
    QueryPerformanceCounter(&s);

    int threads_per_type = K / 3;
    int total_threads = threads_per_type * 3;
    pthread_t* threads = (pthread_t*)malloc(total_threads * sizeof(pthread_t));
    ThreadArgs* thread_args = (ThreadArgs*)malloc(total_threads * sizeof(ThreadArgs));

    for (int type = 0; type < 3; type++) {
        for (int i = 0; i < threads_per_type; i++) {
            int thread_idx = type * threads_per_type + i;
            thread_args[thread_idx].thread_id = thread_idx + 1;
            thread_args[thread_idx].thread_count = threads_per_type;
            thread_args[thread_idx].start_idx = i; 
            thread_args[thread_idx].N = N;
            thread_args[thread_idx].sudoku = sudoku;
            thread_args[thread_idx].type = type;
            thread_args[thread_idx].output_file = out;

            pthread_create(&threads[thread_idx], NULL, validateMixed, &thread_args[thread_idx]);
        }
    }

    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    QueryPerformanceCounter(&e);
    double time_taken = (double)(e.QuadPart - s.QuadPart) * 1e6 / fre.QuadPart;

    fprintf(out, "\nSudoku is %s\n", global_valid ? "valid" : "invalid");
    fprintf(out, "The total time taken is %.2f microseconds.\n", time_taken);

    fclose(out);
    pthread_mutex_destroy(&file_mutex);
    free(threads);
    free(thread_args);
    for (int i = 0; i < N; i++) {
        free(sudoku[i]);
    }
    free(sudoku);

    return 0;
}
