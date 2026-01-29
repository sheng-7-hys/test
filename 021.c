#include <stdio.h>
// 判断三个点是否关于某个点中心对称
// 充要条件：存在一个点是另外两个点的中点
int main() {
    long long x1, y1, x2, y2, x3, y3;
    if (scanf("%lld %lld %lld %lld %lld %lld", &x1, &y1, &x2, &y2, &x3, &y3) != 6) {
        return 0;
    }

    int ok = 0;
    if (2 * x1 == x2 + x3 && 2 * y1 == y2 + y3) {
        ok = 1; // A 是 B、C 的中点
    } else if (2 * x2 == x1 + x3 && 2 * y2 == y1 + y3) {
        ok = 1; // B 是 A、C 的中点
    } else if (2 * x3 == x1 + x2 && 2 * y3 == y1 + y2) {
        ok = 1; // C 是 A、B 的中点
    }

    printf(ok ? "yes\n" : "no\n");
    return 0;
}