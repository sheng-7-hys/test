#include <stdio.h>
#include <string.h>

#define MAX_LEN 60000

int main() {
    char text[MAX_LEN] = {0};
    int len = 0;
    int cursor = 0;
    int insert_mode = 1; 
    int c;                  
    while ((c = fgetc(stdin)) != EOF) {
        switch (c) {
            case '[':
                cursor = 0;
                break;
            case ']':
                cursor = len;
                break;
            case '{':
                if (cursor > 0) {
                    cursor--;
                }
                break;
            case '}':
                if (cursor < len) {
                    cursor++;
                }
                break;
            case '-':
                insert_mode = !insert_mode;
                break;
            case '=':
                if (cursor > 0) {
                    for (int i = cursor - 1; i < len - 1; i++) {
                        text[i] = text[i + 1];
                    }
                    len--; 
                    cursor--;
                }
                break; 
            default:
                if ((c >= 'a' && c <= 'z') || c == ' ') {
                    if (insert_mode) {
                        for (int i = len; i > cursor; i--) {
                            text[i] = text[i - 1];
                        }
                        text[cursor] = c;
                        len++;   
                        cursor++;
                    } else {
                        if (cursor < len) {
                            text[cursor] = c;
                            cursor++;
                        } else {
                            text[cursor] = c;
                            len++;
                            cursor++;
                        }
                    }
                }
                break;
        }
    }
    for (int i = 0; i < len; i++) {
        putchar(text[i]);
    }

    return 0;
}