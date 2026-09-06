#include "chibicc.h"

int main(int argc, char **argv) {
  if (argc != 2)
    error("参数个数不正确");

  Token *token = tokenize(argv[1]);

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
