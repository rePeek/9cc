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

  program();

  // 输出汇编前半部分
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // prologue
  int stack_size = max_stacksize();
  if (stack_size % 16 != 0)
    stack_size += 16 - stack_size % 16;
  printf(" push rbp\n");
  printf(" mov rbp, rsp\n");
  printf(" sub rsp, %d\n", stack_size);

  // 从第一个表达式开始依次生成代码
  for (int i = 0; code[i]; i++) {
    gen(code[i]);

    // 表达式求值结果会在栈上留下一个值
    // 因此预先 pop，避免栈溢出
    printf(" pop rax\n");
  }

  // epilogue
  // 最后一个表达式的结果留在 RAX 中，因此它就是返回值
  printf(" mov rsp, rbp\n");
  printf(" pop rbp\n");
  printf(" ret\n");
  printf(".section .note.GNU-stack,\"\",@progbits\n");
  return 0;
}
