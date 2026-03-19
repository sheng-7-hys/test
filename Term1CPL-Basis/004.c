#include <stdio.h>
// 引入字符串长度计算头文件
#include <string.h>

int main() {
    // 定义存储字符串地址的指针数组
    char* name[3] = {"Alice", "Bob", "Charlie"};
    // 字符占有的bit数（主流系统char=1字节=8bit）
    const int BITS_PER_CHAR = 8;

    // 循环遍历指针数组
    for (int i = 0; i < 3; i++) {
        // 1. 计算字符串相关数值
        // 有效字符数（不含结束符'\0'）
        int char_count = strlen(name[i]);
        // 有效字符的bit数 = 字符数 × 每字符bit数
        int bits_count = char_count * BITS_PER_CHAR;
        // 内存总占用bit数（含结束符'\0'）
        int total_bits_count = (char_count + 1) * BITS_PER_CHAR;

        // 2. 打印原有信息 + 新增bit数信息
        printf("字符串内容：%s\n", name[i]);
        printf("字符串起始地址：%p\n", name[i]);       // 字符串在内存中的地址
        printf("指针数组元素地址：%p\n", &name[i]);   // 数组中指针变量的地址
        printf("有效字符数：%d → 有效bit数：%d bit\n", char_count, bits_count);
        printf("含结束符总字节数：%d → 总bit数：%d bit\n", char_count + 1, total_bits_count);
        printf("----------------------------------------\n");
    }

    return 0;
}