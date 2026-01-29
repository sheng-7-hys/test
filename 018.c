#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char buf[1024];                     /* fixed-size input buffer */
    if (!fgets(buf, sizeof buf, stdin)) /* read a line safely */
        return 1;
    buf[strcspn(buf, "\n")] = '\0';     /* strip trailing newline */

    for (size_t i = 0; buf[i] != '\0'; i++) {
        if (buf[i] >= 'a' && buf[i] <= 'w') buf[i] += 3;
        else if (buf[i] >= 'x' && buf[i] <= 'z') buf[i] -= 23;
        else if (buf[i] >= 'A' && buf[i] <= 'W') buf[i] += 3;
        else if (buf[i] >= 'X' && buf[i] <= 'Z') buf[i] -= 23;
    }

    printf("%s\n", buf);
    return 0;
}