#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int checkID(char id[19]) {
    int weight[] = {7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
    char checkCode[] = "10X98765432";
    for (int i = 0; i < 17; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return 0;
        }
    }
    int sum = 0;
    for (int i = 0; i < 17; i++) {
        sum += (id[i] - '0') * weight[i];
    }
    int z = sum % 11;
    char correctM = checkCode[z];
    char idM = (id[17] >= 'a' && id[17] <= 'z') ? (id[17] - 32) : id[17];
    return (idM == correctM) ? 1 : 0;
}

int main() {
    int N;
    scanf("%d", &N);
    char (*ids)[19] = (char (*)[19])malloc(N * sizeof(char[19]));
    if (ids == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < N; i++) {
        scanf("%18s", ids[i]);
        ids[i][18] = '\0';
    }

    int hasProblem = 0;
    for (int i = 0; i < N; i++) {
        if (checkID(ids[i]) == 0) {
            printf("%s\n", ids[i]);
            hasProblem = 1;
        }
    }

    // 所有号码均合法时输出
    if (!hasProblem) {
        printf("All passed\n");
    }

    free(ids);
    return 0;
}