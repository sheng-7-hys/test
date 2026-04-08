#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_FORMULA_LEN 256
#define MAX_TOKENS 512
#define MAX_VARS 8
#define MAX_PREMISES 16

typedef struct {
    char type;   // 'V' 表示变量, 'O' 表示运算符
    char value;  // 变量名或运算符字符
} Token;

typedef struct {
    char source[MAX_FORMULA_LEN];
    Token rpn[MAX_TOKENS];
    int rpn_len;
    int used_vars[26];
} ParsedFormula;

// 去掉字符串首尾空白
static void trim(char *s) {
    int n = (int)strlen(s);
    int left = 0;
    while (left < n && isspace((unsigned char)s[left])) {
        left++;
    }
    int right = n - 1;
    while (right >= left && isspace((unsigned char)s[right])) {
        right--;
    }
    int k = 0;
    for (int i = left; i <= right; i++) {
        s[k++] = s[i];
    }
    s[k] = '\0';
}

static int precedence(char op) {
    if (op == '!') return 4;
    if (op == '&') return 3;
    if (op == '|') return 2;
    if (op == '>') return 1; // 蕴含
    return 0;
}

static int is_right_assoc(char op) {
    return (op == '!' || op == '>');
}

// 将中缀表达式转换为后缀表达式(RPN)，并顺带做合法性检查
static int parse_formula(const char *input, ParsedFormula *out, char *err, int err_size) {
    memset(out, 0, sizeof(*out));
    strncpy(out->source, input, MAX_FORMULA_LEN - 1);

    char op_stack[MAX_TOKENS];
    int op_top = 0;
    int out_len = 0;
    int expect_operand = 1; // 1: 当前应出现“操作数”或“!”或“(”

    int n = (int)strlen(input);
    for (int i = 0; i < n; i++) {
        char c = input[i];
        if (isspace((unsigned char)c)) {
            continue;
        }

        // 支持 "->" 作为蕴含
        if (c == '-' && i + 1 < n && input[i + 1] == '>') {
            c = '>';
            i++;
        }

        if (c >= 'A' && c <= 'Z') {
            if (!expect_operand) {
                snprintf(err, err_size, "变量前缺少运算符");
                return 0;
            }
            out->used_vars[c - 'A'] = 1;
            out->rpn[out_len].type = 'V';
            out->rpn[out_len].value = c;
            out_len++;
            expect_operand = 0;
        } else if (c == '(') {
            if (!expect_operand) {
                snprintf(err, err_size, "'(' 前缺少运算符");
                return 0;
            }
            op_stack[op_top++] = c;
        } else if (c == ')') {
            if (expect_operand) {
                snprintf(err, err_size, "')' 前缺少有效子表达式");
                return 0;
            }
            int matched = 0;
            while (op_top > 0) {
                char t = op_stack[--op_top];
                if (t == '(') {
                    matched = 1;
                    break;
                }
                out->rpn[out_len].type = 'O';
                out->rpn[out_len].value = t;
                out_len++;
            }
            if (!matched) {
                snprintf(err, err_size, "括号不匹配");
                return 0;
            }
            expect_operand = 0;
        } else if (c == '!') {
            if (!expect_operand) {
                snprintf(err, err_size, "'!' 位置非法");
                return 0;
            }
            while (op_top > 0) {
                char t = op_stack[op_top - 1];
                if (t == '(') break;
                if (precedence(t) > precedence(c) ||
                    (precedence(t) == precedence(c) && !is_right_assoc(c))) {
                    op_top--;
                    out->rpn[out_len].type = 'O';
                    out->rpn[out_len].value = t;
                    out_len++;
                } else {
                    break;
                }
            }
            op_stack[op_top++] = c;
        } else if (c == '&' || c == '|' || c == '>') {
            if (expect_operand) {
                snprintf(err, err_size, "二元运算符 '%c' 左侧缺少操作数", c);
                return 0;
            }
            while (op_top > 0) {
                char t = op_stack[op_top - 1];
                if (t == '(') break;
                if (precedence(t) > precedence(c) ||
                    (precedence(t) == precedence(c) && !is_right_assoc(c))) {
                    op_top--;
                    out->rpn[out_len].type = 'O';
                    out->rpn[out_len].value = t;
                    out_len++;
                } else {
                    break;
                }
            }
            op_stack[op_top++] = c;
            expect_operand = 1;
        } else {
            snprintf(err, err_size, "出现非法字符: %c", c);
            return 0;
        }

        if (out_len >= MAX_TOKENS - 1 || op_top >= MAX_TOKENS - 1) {
            snprintf(err, err_size, "表达式过长");
            return 0;
        }
    }

    if (expect_operand) {
        snprintf(err, err_size, "表达式不能以运算符结尾");
        return 0;
    }

    while (op_top > 0) {
        char t = op_stack[--op_top];
        if (t == '(') {
            snprintf(err, err_size, "括号不匹配");
            return 0;
        }
        out->rpn[out_len].type = 'O';
        out->rpn[out_len].value = t;
        out_len++;
    }

    out->rpn_len = out_len;
    return 1;
}

// 从 used_vars 数组中按字母序提取变量列表
static int collect_vars(const int used_vars[26], char vars[MAX_VARS]) {
    int n = 0;
    for (int i = 0; i < 26; i++) {
        if (used_vars[i]) {
            if (n >= MAX_VARS) {
                return -1;
            }
            vars[n++] = (char)('A' + i);
        }
    }
    return n;
}

// 根据 union 变量表与 mask 得到每个变量真值
static void build_values(const char vars[MAX_VARS], int var_count, int mask, int values[26]) {
    memset(values, 0, sizeof(int) * 26);
    for (int i = 0; i < var_count; i++) {
        int bit = (mask >> (var_count - 1 - i)) & 1;
        values[vars[i] - 'A'] = bit;
    }
}

// 计算后缀表达式真值
static int eval_formula(const ParsedFormula *f, const int values[26], int *ok) {
    int st[MAX_TOKENS];
    int top = 0;
    *ok = 1;
    for (int i = 0; i < f->rpn_len; i++) {
        Token tk = f->rpn[i];
        if (tk.type == 'V') {
            st[top++] = values[tk.value - 'A'];
        } else {
            char op = tk.value;
            if (op == '!') {
                if (top < 1) {
                    *ok = 0;
                    return 0;
                }
                st[top - 1] = !st[top - 1];
            } else {
                if (top < 2) {
                    *ok = 0;
                    return 0;
                }
                int b = st[--top];
                int a = st[--top];
                int r = 0;
                if (op == '&') r = (a && b);
                else if (op == '|') r = (a || b);
                else if (op == '>') r = (!a || b); // 蕴含
                else {
                    *ok = 0;
                    return 0;
                }
                st[top++] = r;
            }
        }
    }
    if (top != 1) {
        *ok = 0;
        return 0;
    }
    return st[0];
}

static int build_union_vars(const ParsedFormula *a, const ParsedFormula *b, char vars[MAX_VARS]) {
    int used[26] = {0};
    for (int i = 0; i < 26; i++) {
        used[i] = a->used_vars[i] || b->used_vars[i];
    }
    return collect_vars(used, vars);
}

// 打印真值表并记录每行结果
static void print_truth_table(const ParsedFormula *f, const char vars[MAX_VARS], int nvars, int rows_result[]) {
    int rows = 1 << nvars;
    printf("\n[1] 真值表:\n");
    for (int i = 0; i < nvars; i++) {
        printf("%c ", vars[i]);
    }
    printf("| F\n");
    for (int i = 0; i < nvars; i++) {
        printf("--");
    }
    printf("+---\n");

    for (int mask = 0; mask < rows; mask++) {
        int values[26];
        int ok = 0;
        build_values(vars, nvars, mask, values);
        for (int i = 0; i < nvars; i++) {
            printf("%d ", values[vars[i] - 'A']);
        }
        int val = eval_formula(f, values, &ok);
        if (!ok) val = 0;
        rows_result[mask] = val;
        printf("| %d\n", val);
    }
}

// 根据真值表输出主析取范式
static void print_pdnf(const char vars[MAX_VARS], int nvars, const int rows_result[]) {
    int rows = 1 << nvars;
    int first = 1;
    printf("\n[3] 主析取范式(PDNF):\n");
    for (int mask = 0; mask < rows; mask++) {
        if (!rows_result[mask]) continue;
        if (!first) printf(" | ");
        first = 0;
        printf("(");
        for (int i = 0; i < nvars; i++) {
            int bit = (mask >> (nvars - 1 - i)) & 1;
            if (!bit) printf("!");
            printf("%c", vars[i]);
            if (i != nvars - 1) printf("&");
        }
        printf(")");
    }
    if (first) {
        printf("0");
    }
    printf("\n");
}

// 根据真值表输出主合取范式
static void print_pcnf(const char vars[MAX_VARS], int nvars, const int rows_result[]) {
    int rows = 1 << nvars;
    int first = 1;
    printf("[3] 主合取范式(PCNF):\n");
    for (int mask = 0; mask < rows; mask++) {
        if (rows_result[mask]) continue;
        if (!first) printf(" & ");
        first = 0;
        printf("(");
        for (int i = 0; i < nvars; i++) {
            int bit = (mask >> (nvars - 1 - i)) & 1;
            if (bit) printf("!");
            printf("%c", vars[i]);
            if (i != nvars - 1) printf("|");
        }
        printf(")");
    }
    if (first) {
        printf("1");
    }
    printf("\n");
}

static void print_formula_type(int nvars, const int rows_result[]) {
    int rows = 1 << nvars;
    int all_true = 1, all_false = 1;
    for (int i = 0; i < rows; i++) {
        if (rows_result[i]) all_false = 0;
        else all_true = 0;
    }

    printf("\n[2] 命题类别:\n");
    if (all_true) {
        printf("该命题是：永真式\n");
    } else if (all_false) {
        printf("该命题是：永假式\n");
    } else {
        printf("该命题是：可满足式（既非永真也非永假）\n");
    }
}

static void judge_relation(const ParsedFormula *f1, const ParsedFormula *f2) {
    char vars[MAX_VARS];
    int nvars = build_union_vars(f1, f2, vars);
    if (nvars < 0) {
        printf("\n[4] 两命题关系: 变量过多(最多支持 %d 个不同变量)\n", MAX_VARS);
        return;
    }

    int rows = 1 << nvars;
    int equiv = 1;
    int opposite = 1;
    int a_imp_b = 1;
    int b_imp_a = 1;

    for (int mask = 0; mask < rows; mask++) {
        int values[26], ok1 = 0, ok2 = 0;
        build_values(vars, nvars, mask, values);
        int a = eval_formula(f1, values, &ok1);
        int b = eval_formula(f2, values, &ok2);
        if (!ok1 || !ok2) {
            printf("\n[4] 两命题关系: 求值失败\n");
            return;
        }
        if (a != b) equiv = 0;
        if (a == b) opposite = 0;
        if (a && !b) a_imp_b = 0;
        if (b && !a) b_imp_a = 0;
    }

    printf("\n[4] 两个命题公式的逻辑关系:\n");
    if (equiv) {
        printf("两命题逻辑等价。\n");
    } else if (opposite) {
        printf("两命题互为矛盾（恒有一真一假）。\n");
    } else if (a_imp_b && !b_imp_a) {
        printf("前者蕴含后者（但不等价）。\n");
    } else if (!a_imp_b && b_imp_a) {
        printf("后者蕴含前者（但不等价）。\n");
    } else if (a_imp_b && b_imp_a) {
        printf("两命题逻辑等价。\n");
    } else {
        printf("两命题既不等价，也不存在单向蕴含关系。\n");
    }
}

static int split_premises(char *left, char parts[MAX_PREMISES][MAX_FORMULA_LEN]) {
    int count = 0;
    char *p = left;
    char *start = p;
    while (1) {
        if (*p == ',' || *p == ';' || *p == '\0') {
            int len = (int)(p - start);
            if (len > 0) {
                if (count >= MAX_PREMISES) return -1;
                if (len >= MAX_FORMULA_LEN) len = MAX_FORMULA_LEN - 1;
                strncpy(parts[count], start, len);
                parts[count][len] = '\0';
                trim(parts[count]);
                if (strlen(parts[count]) > 0) {
                    count++;
                }
            }
            if (*p == '\0') break;
            start = p + 1;
        }
        p++;
    }
    return count;
}

static void judge_inference(const char *input) {
    char line[MAX_FORMULA_LEN];
    strncpy(line, input, MAX_FORMULA_LEN - 1);
    line[MAX_FORMULA_LEN - 1] = '\0';
    trim(line);

    char *arrow = strstr(line, "=>");
    if (arrow == NULL) {
        printf("\n[5] 推理判断: 输入格式错误，应为 \"前提1,前提2=>结论\"\n");
        return;
    }

    char left[MAX_FORMULA_LEN], right[MAX_FORMULA_LEN];
    int left_len = (int)(arrow - line);
    if (left_len >= MAX_FORMULA_LEN) left_len = MAX_FORMULA_LEN - 1;
    strncpy(left, line, left_len);
    left[left_len] = '\0';
    strncpy(right, arrow + 2, MAX_FORMULA_LEN - 1);
    right[MAX_FORMULA_LEN - 1] = '\0';
    trim(left);
    trim(right);

    if (strlen(right) == 0) {
        printf("\n[5] 推理判断: 缺少结论\n");
        return;
    }

    char premise_texts[MAX_PREMISES][MAX_FORMULA_LEN];
    int premise_count = split_premises(left, premise_texts);
    if (premise_count < 0) {
        printf("\n[5] 推理判断: 前提过多，最多 %d 条\n", MAX_PREMISES);
        return;
    }

    ParsedFormula premises[MAX_PREMISES];
    ParsedFormula conclusion;
    char err[128];

    for (int i = 0; i < premise_count; i++) {
        if (!parse_formula(premise_texts[i], &premises[i], err, sizeof(err))) {
            printf("\n[5] 推理判断: 前提 %d 非法：%s\n", i + 1, err);
            return;
        }
    }
    if (!parse_formula(right, &conclusion, err, sizeof(err))) {
        printf("\n[5] 推理判断: 结论非法：%s\n", err);
        return;
    }

    int used[26] = {0};
    for (int i = 0; i < premise_count; i++) {
        for (int j = 0; j < 26; j++) {
            if (premises[i].used_vars[j]) used[j] = 1;
        }
    }
    for (int j = 0; j < 26; j++) {
        if (conclusion.used_vars[j]) used[j] = 1;
    }

    char vars[MAX_VARS];
    int nvars = collect_vars(used, vars);
    if (nvars < 0) {
        printf("\n[5] 推理判断: 变量过多(最多支持 %d 个不同变量)\n", MAX_VARS);
        return;
    }

    int rows = 1 << nvars;
    int valid = 1;
    for (int mask = 0; mask < rows; mask++) {
        int values[26];
        int ok = 0;
        build_values(vars, nvars, mask, values);

        int all_premises_true = 1;
        for (int i = 0; i < premise_count; i++) {
            int pv = eval_formula(&premises[i], values, &ok);
            if (!ok) {
                printf("\n[5] 推理判断: 前提求值失败\n");
                return;
            }
            if (!pv) {
                all_premises_true = 0;
                break;
            }
        }

        if (all_premises_true) {
            int cv = eval_formula(&conclusion, values, &ok);
            if (!ok) {
                printf("\n[5] 推理判断: 结论求值失败\n");
                return;
            }
            if (!cv) {
                valid = 0;
                break;
            }
        }
    }

    printf("\n[5] 命题推理正确性判断:\n");
    if (valid) {
        printf("该推理是正确的（前提集合逻辑蕴含结论）。\n");
    } else {
        printf("该推理不正确（存在反例：前提全真但结论为假）。\n");
    }
}

// 按实验要求给出覆盖多种情况的测试样例
static void print_sample_cases(void) {
    printf("\n=== 建议测试用例（可直接复制运行）===\n");
    printf("1) 公式合法性与四种联结词：!(A&B)|((A>B)&B)\n");
    printf("2) 两公式关系：\n");
    printf("   F1: A>B\n");
    printf("   F2: !A|B\n");
    printf("3) 推理判断（正确）：A, A>B => B\n");
    printf("4) 推理判断（错误）：A|B => A\n");
    printf("=================================\n");
}

int main(void) {
    char line1[MAX_FORMULA_LEN];
    char line2[MAX_FORMULA_LEN];
    char infer_line[MAX_FORMULA_LEN];
    char err[128];
    ParsedFormula f1, f2;

    printf("命题逻辑实验程序（支持运算符: !  &  |  > 或 -> ）\n");
    printf("变量请使用大写字母 A~Z，最多 %d 个不同变量。\n\n", MAX_VARS);

    printf("请输入命题公式 F1: ");
    if (!fgets(line1, sizeof(line1), stdin)) return 0;
    trim(line1);
    if (!parse_formula(line1, &f1, err, sizeof(err))) {
        printf("F1 非法：%s\n", err);
        return 0;
    }

    char vars[MAX_VARS];
    int nvars = collect_vars(f1.used_vars, vars);
    if (nvars < 0) {
        printf("F1 变量过多(最多 %d 个)\n", MAX_VARS);
        return 0;
    }

    int rows_result[1 << MAX_VARS] = {0};
    print_truth_table(&f1, vars, nvars, rows_result);
    print_formula_type(nvars, rows_result);
    print_pdnf(vars, nvars, rows_result);
    print_pcnf(vars, nvars, rows_result);

    printf("\n请输入命题公式 F2（用于关系判断）: ");
    if (!fgets(line2, sizeof(line2), stdin)) return 0;
    trim(line2);
    if (!parse_formula(line2, &f2, err, sizeof(err))) {
        printf("F2 非法：%s\n", err);
        return 0;
    }
    judge_relation(&f1, &f2);

    printf("\n请输入推理式（格式: 前提1,前提2=>结论 ）: ");
    if (!fgets(infer_line, sizeof(infer_line), stdin)) return 0;
    trim(infer_line);
    judge_inference(infer_line);

    print_sample_cases();
    return 0;
}
