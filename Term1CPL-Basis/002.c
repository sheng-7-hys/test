// Selection sort demonstration: sorts an integer array in ascending order
#include <stdio.h>

static void selection_sort(int *arr, int n) {
	for (int i = 0; i < n - 1; ++i) {
		int min_idx = i;
		for (int j = i + 1; j < n; ++j) {
			if (arr[j] < arr[min_idx]) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			int tmp = arr[i];
			arr[i] = arr[min_idx];
			arr[min_idx] = tmp;
		}
	}
}

int main(void) {
	int data[] = {29, 10, 14, 37, 13, 5, 42};
	int n = (int)(sizeof(data) / sizeof(data[0]));

	printf("Before: ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", data[i]);
	}
	printf("\n");

	selection_sort(data, n);

	printf("After : ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", data[i]);
	}
	printf("\n");

	return 0;
}
