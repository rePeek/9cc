#include "9cc.h"

// 当前关注的 token
Token *token;

// 输入字符串
char *user_input;

// 用于报告错误的函数
// 接收与 printf 相同的参数
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 报告错误位置
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  exit(1);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("参数个数不正确");
    return 1;
  }

  user_input = argv[1];
  token = tokenize(user_input);

  Node *node = expr();

  if (!at_eof())
    error_at(token->str, "无法解析");

  // 输出汇编前半部分
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  gen(node);

  printf(" pop rax\n");
  printf(" ret\n");
  printf(".section .note.GNU-stack,\"\",@progbits\n");
  return 0;
}
