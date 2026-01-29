#include <stdio.h>
#include <stdlib.h>

double calculateGPA(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 0.0;
    }

    double sum_sc = 0.0;
    int sum_s = 0;

    for (int i = 0; i < n; i++) {
        int credits;
        char grade[16];

        if (scanf("%d %15s", &credits, grade) != 2) {
            continue;
        }

        if (grade[0] == 'P' || grade[0] == 'N') {
            continue;
        }

        char *endptr = NULL;
        long score = strtol(grade, &endptr, 10);
        if (*endptr != '\0' || credits <= 0) {
            continue;
        }

        sum_sc += credits * score;
        sum_s += credits;
    }

    if (sum_s == 0) {
        return 0.0;
    }

    return sum_sc / sum_s;
}

int main(void) {
    double gpaA = calculateGPA();
    double gpaB = calculateGPA();
    double diff = gpaA - gpaB;
    if(diff<=1.0*10e-6 && diff>=-1.0*10e-6){
        diff=0.00;
    }
    printf("%.2f\n", diff);
    return 0;
}