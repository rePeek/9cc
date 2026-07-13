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
    error_at(token->str, "不是 '%c'", op);
  token = token->next;
}

// 如果下一个 token 是数值，就读取一个 token 并返回该数值。
// 否则报告错误。
int expect_number() {
  if (token->kind != TK_NUM)
    error_at(token->str, "不是数字");
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

    if (strchr("+-*/()", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "无法 tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

// 抽象语法树节点的种类
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

// 抽象语法树节点的类型
struct Node {
  NodeKind kind; // 节点类型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  int val;       // 仅在 kind 为 ND_NUM 时使用
};

Node *expr();
Node *mul();
Node *unary();
Node *primary();

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node(ND_ADD, node, mul());
    else if (consume('-'))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_node(ND_MUL, node, unary());
    else if (consume('/'))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume('+'))
    return primary();
  if (consume('-'))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *primary() {
  // 如果下一个 token 是 "("，则应为 "(" expr ")"
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }

  // 否则应为数值
  return new_node_num(expect_number());
}

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf(" push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf(" pop rdi\n");
  printf(" pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf(" add rax, rdi\n");
    break;
  case ND_SUB:
    printf(" sub rax, rdi\n");
    break;
  case ND_MUL:
    printf(" imul rax, rdi\n");
    break;
  case ND_DIV:
    printf(" cqo\n");
    printf(" idiv rdi\n");
    break;
  case ND_NUM:
    break;
  }

  printf(" push rax\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("参数个数不正确");
    return 1;
  }

  user_input = argv[1];

  // 进行 tokenize
  token = tokenize(argv[1]);

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
