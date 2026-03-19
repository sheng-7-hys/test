// 更全面的文件操作示例 / More complete file I/O demo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 小工具：若失败则退出 / Small helper: exit on failure
static FILE *xfopen(const char *path, const char *mode) {
	FILE *fp = fopen(path, mode);
	if (!fp) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	return fp;
}

// 文本写入模式 w / Text write (overwrite)
static void write_text_overwrite(const char *path) {
	FILE *fp = xfopen(path, "w"); // 不存在则创建，存在则清空 / create or truncate
	fputs("Hello file!\n这是覆盖写入。\nThis overwrites the file.\n", fp);
	fclose(fp);
}

// 文本追加模式 a / Text append
static void append_text(const char *path) {
	FILE *fp = xfopen(path, "a"); // 不存在则创建，指针在末尾 / create if missing, pointer at end
	fputs("追加一行中文。\nAppending a new line.\n", fp);
	fclose(fp);
}

// 文本读模式 r / Text read
static void read_text(const char *path) {
	FILE *fp = xfopen(path, "r");
	char buf[256];
	printf("Reading %s (mode r):\n", path);
	while (fgets(buf, (int)sizeof(buf), fp)) {
		fputs(buf, stdout);
	}
	if (ferror(fp)) {
		perror("read_text");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	fclose(fp);
}

// 读写模式 r+ / Read-Write without truncation
static void read_write_inplace(const char *path) {
	FILE *fp = xfopen(path, "r+"); // 可读可写，文件必须已存在 / read-write, must exist
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	printf("%s size before r+: %ld bytes\n", path, size);
	fseek(fp, 0, SEEK_SET);
	fputs("[r+] 在开头插入/Insert at start\n", fp);
	fclose(fp);
}

// 读写+截断模式 w+ / Read-Write with truncate
static void rewrite_with_wplus(const char *path) {
	FILE *fp = xfopen(path, "w+"); // 清空后可读写 / truncate then read-write
	fputs("w+ 清空并重写\nrewritten with w+\n", fp);
	fseek(fp, 0, SEEK_SET);
	char buf[128];
	printf("After w+ rewrite, reading:\n");
	while (fgets(buf, (int)sizeof(buf), fp)) {
		fputs(buf, stdout);
	}
	fclose(fp);
}

// 读写追加模式 a+ / Read from start, write always at end
static void read_then_append_aplus(const char *path) {
	FILE *fp = xfopen(path, "a+"); // 可读可写，写入强制在末尾 / read-write, writes at end
	fseek(fp, 0, SEEK_SET);
	char buf[128];
	printf("Existing content before a+:\n");
	while (fgets(buf, (int)sizeof(buf), fp)) {
		fputs(buf, stdout);
	}
	fputs("[a+] 末尾追加\n", fp);
	fclose(fp);
}

// 二进制写入与读取 / Binary write & read
static void write_binary(const char *path) {
	int numbers[5] = {1, 2, 3, 4, 5};
	FILE *fp = xfopen(path, "wb"); // binary overwrite
	size_t written = fwrite(numbers, sizeof(int), 5, fp);
	if (written != 5) {
		perror("fwrite");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	fclose(fp);
}

static void read_binary(const char *path) {
	int numbers[5] = {0};
	FILE *fp = xfopen(path, "rb");
	size_t read = fread(numbers, sizeof(int), 5, fp);
	if (read != 5) {
		perror("fread");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	fclose(fp);
	printf("Binary ints: ");
	for (size_t i = 0; i < 5; ++i) {
		printf("%d ", numbers[i]);
	}
	printf("\n");
}

// 格式化写入与读取 / Formatted write & read
static void formatted_io(const char *path) {
	FILE *fp = xfopen(path, "w");
	// fprintf 支持格式化输出 / formatted output
	fprintf(fp, "name=%s age=%d score=%.1f\n", "Alice", 20, 95.5);
	fprintf(fp, "name=%s age=%d score=%.1f\n", "Bob", 22, 88.0);
	fclose(fp);

	fp = xfopen(path, "r");
	char name[32];
	int age = 0;
	double score = 0.0;
	printf("Formatted read from %s:\n", path);
	while (fscanf(fp, "name=%31s age=%d score=%lf", name, &age, &score) == 3) {
		printf("Parsed -> name:%s age:%d score:%.1f\n", name, age, score);
	}
	fclose(fp);
}

// 重命名与删除 / Rename & delete
static void rename_and_delete(const char *old_path, const char *new_path) {
	if (rename(old_path, new_path) != 0) {
		perror("rename");
		exit(EXIT_FAILURE);
	}
	printf("Renamed %s -> %s\n", old_path, new_path);

	if (remove(new_path) != 0) {
		perror("remove");
		exit(EXIT_FAILURE);
	}
	printf("Deleted %s\n", new_path);
}

// 主流程 / Main flow
int main(void) {
	const char *demo_txt = "demo.txt";
	const char *rw_txt = "rw.txt";
	const char *aplus_txt = "aplus.txt";
	const char *bin_file = "numbers.bin";
	const char *fmt_file = "formatted.txt";

	// 1) w 覆盖写入 / overwrite
	write_text_overwrite(demo_txt);
	read_text(demo_txt);

	// 2) a 追加写入 / append
	append_text(demo_txt);
	read_text(demo_txt);

	// 3) r+ 读写不清空 / read-write in place
	write_text_overwrite(rw_txt);
	read_write_inplace(rw_txt);
	read_text(rw_txt);

	// 4) w+ 读写并清空 / read-write with truncate
	rewrite_with_wplus(rw_txt);

	// 5) a+ 读写且写入在末尾 / read then append
	write_text_overwrite(aplus_txt);
	read_then_append_aplus(aplus_txt);
	read_text(aplus_txt);

	// 6) 二进制写读 / binary
	write_binary(bin_file);
	read_binary(bin_file);

	// 7) 格式化写读 / formatted
	formatted_io(fmt_file);

	// 8) 重命名并删除示例 / rename and delete demo
	write_text_overwrite("temp.txt");
	rename_and_delete("temp.txt", "temp_renamed.txt");

	printf("\nAll operations done.\n");
	return 0;
}
