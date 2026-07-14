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
  int len;        // token 长度
};

// 抽象语法树节点的种类
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

// 抽象语法树节点的类型
struct Node {
  NodeKind kind; // 节点类型
  Node *lhs;     // 左边
  Node *rhs;     // 右边
  int val;       // 仅在 kind 为 ND_NUM 时使用
};

extern Token *token;
extern char *user_input;

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
Token *tokenize(char *p);
Node *expr(void);
bool at_eof(void);
void gen(Node *node);

#endif
