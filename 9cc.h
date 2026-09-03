#ifndef NINECC_H
#define NINECC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token 的种类
typedef enum {
  TK_PUNCT,   // 运算符
  TK_IDENT,   // 标识符
  TK_NUM,     // 整数 token
  TK_EOF,     // 表示输入结束的 token
  TK_KEYWORD, // 关键字
} TokenKind;

typedef struct Token Token;

// token 类型
struct Token {
  Token *next;    // 下一个输入 token
  char *str;      // token 字符串
  TokenKind kind; // token 类型
  int val;        // kind 为 TK_NUM 时的数值
  int len;        // token 长度
};

// 抽象语法树节点的种类
typedef enum {
  ND_ADD,    // +
  ND_SUB,    // -
  ND_MUL,    // *
  ND_DIV,    // /
  ND_ASSIGN, // =
  ND_LVAR,   // 局部变量
  ND_EQ,     // ==
  ND_NE,     // !=
  ND_LT,     // <
  ND_LE,     // <=
  ND_NUM,    // 整数
  ND_RETURN, // 返回值
} NodeKind;

typedef struct Node Node;

// 抽象语法树节点的类型
struct Node {
  NodeKind kind; // 节点类型
  Node *lhs;     // 左边
  Node *rhs;     // 右边
  int val;       // 仅在 kind 为 ND_NUM 时使用
  int offset;    // 仅在kind为 ND_LVAR 时使用
};

typedef struct LVar LVar;

struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

extern LVar *locals;
extern Token *token;
extern char *user_input;
extern Node *code[];

Token *tokenize(char *p);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool at_eof(void);
void program(void);
Node *stmt(void);
Node *expr(void);
void gen(Node *node);
int max_stacksize(void);

#endif
