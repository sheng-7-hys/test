// Demo: dynamic allocation with malloc, calloc, realloc, free
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_ints(const int *arr, size_t n, const char *label) {
	printf("%s (%zu): ", label, n);
	for (size_t i = 0; i < n; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// malloc: raw bytes, contents uninitialized
static void demo_malloc(void) {
	size_t n = 5;
	int *arr = (int *)malloc(n * sizeof(int));
	if (!arr) {
		perror("malloc failed");
		return;
	}

	// Always initialize after malloc
	for (size_t i = 0; i < n; ++i) {
		arr[i] = (int)(i + 1);
	}
	print_ints(arr, n, "malloc");

	free(arr); // pair every successful malloc with free
}

// calloc: zero-initialized block; helps avoid using garbage values
static void demo_calloc(void) {
	size_t n = 4;
	int *arr = (int *)calloc(n, sizeof(int));
	if (!arr) {
		perror("calloc failed");
		return;
	}

	print_ints(arr, n, "calloc (zeroed)");

	// Populate after calloc
	for (size_t i = 0; i < n; ++i) {
		arr[i] = (int)(10 * (int)(i + 1));
	}
	print_ints(arr, n, "calloc (filled)");

	free(arr);
}

// realloc: resize an existing allocation; on failure original block stays valid
static void demo_realloc(void) {
	size_t n = 3;
	int *arr = (int *)malloc(n * sizeof(int));
	if (!arr) {
		perror("malloc failed");
		return;
	}
	for (size_t i = 0; i < n; ++i) {
		arr[i] = (int)(i + 1);
	}
	print_ints(arr, n, "realloc start");

	size_t new_n = 6;
	int *tmp = (int *)realloc(arr, new_n * sizeof(int));
	if (!tmp) {
		// On failure, arr is still valid; free it to avoid leak
		perror("realloc failed");
		free(arr);
		return;
	}
	arr = tmp;

	// Initialize the newly added slots
	for (size_t i = n; i < new_n; ++i) {
		arr[i] = (int)((i + 1) * 10);
	}
	print_ints(arr, new_n, "realloc grown");

	// Example shrink
	size_t shrink_n = 2;
	tmp = (int *)realloc(arr, shrink_n * sizeof(int));
	if (!tmp) {
		perror("realloc (shrink) failed");
		free(arr);
		return;
	}
	arr = tmp;
	print_ints(arr, shrink_n, "realloc shrunk");

	free(arr);
}

int main(void) {
	demo_malloc();
	demo_calloc();
	demo_realloc();
	return 0;
}
