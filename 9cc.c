#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token 的种类
typedef enum {
    TK_RESERVED, // 保留字
    TK_NUM,      // 整数 token
    TK_EOF,      // 表示输入结束的 token
} TokenKind;

typedef struct Token Token;

// token 类型
struct Token {
    Token *next;    // 下一个输入 token
    char *str;      // token 字符串
    TokenKind kind; // token 类型
    int val;        // kind 为 TK_NUM 时的数值
};

// 当前关注的 token
Token *token;

// 用于报告错误的函数
// 接收与 printf 相同的参数
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// 如果下一个 token 是期望的符号，就读取一个 token
// 并返回 true；否则返回 false。
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// 如果下一个 token 是期望的符号，就读取一个 token。
// 否则报告错误。
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error("不是 '%c'", op);
    token = token->next;
}

// 如果下一个 token 是数值，就读取一个 token 并返回该数值。
// 否则报告错误。
int expect_number() {
    if (token->kind != TK_NUM)
        error("不是数字");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() { return token->kind == TK_EOF; }

// 创建新 token 并连接到 cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// tokenize 输入字符串 p 并返回结果
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // 跳过空白字符
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("无法 tokenize");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        error("参数个数不正确");
        return 1;
    }

    // 进行 tokenize
    token = tokenize(argv[1]);

    // 输出汇编前半部分
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // 表达式开头必须是数字，因此检查它
    // 并输出最初的 mov 指令
    printf(" mov rax, %d\n", expect_number());

    // 一边消费 `+ <数>` 或 `- <数>` 这样的 token 序列
    // 一边输出汇编
    while (!at_eof()) {
        if (consume('+')) {
            printf(" add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf(" sub rax, %d\n", expect_number());
    }

    printf(" ret\n");
    printf(".section .note.GNU-stack,\"\",@progbits\n");
    return 0;
}
