#include <stdio.h>
#include <string.h>

int main() {
    int k;
    scanf("%d", &k);
    
    for (int i = 0; i < k; i++) {
        char gender[10];
        float wbc, rbc, hb, hct, plt;
        scanf("%s %f %f %f %f %f", gender, &wbc, &rbc, &hb, &hct, &plt);
        
        int abnormal_count = 0;
        if (!(4.0 <= wbc && wbc <= 10.0)) {
            abnormal_count++;
        }
        if (!(3.5 <= rbc && rbc <= 5.5)) {
            abnormal_count++;
        }
        if (strcmp(gender, "male") == 0) {
            if (!(120 <= hb && hb <= 160)) {
                abnormal_count++;
            }
        } else {
            if (!(110 <= hb && hb <= 150)) {
                abnormal_count++;
            }
        }
        if (strcmp(gender, "male") == 0) {
            if (!(42 <= hct && hct <= 48)) {
                abnormal_count++;
            }
        } else {
            if (!(36 <= hct && hct <= 40)) {
                abnormal_count++;
            }
        }
        if (!(100 <= plt && plt <= 300)) {
            abnormal_count++;
        }
        if (abnormal_count == 0) {
            printf("normal\n");
        } else {
            printf("%d\n", abnormal_count);
        }
    }
    return 0;
}