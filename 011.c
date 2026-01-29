#include <stdio.h>

int main() {
    int n, min, num;
    scanf("%d", &n);
    scanf("%d", &min);
    for (int i = 1; i < n; i++) {
        scanf("%d", &num);
        if (num < min) {
            min = num;
        }
    }
    printf("min = %d\n", min);
    return 0;
}