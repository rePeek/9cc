#ifndef CHIBICC_H
#define CHIBICC_H

#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenizer.c
//
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
  TokenKind kind; // token 类型
  Token *next;    // 下一个输入 token
  int val;        // kind 为 TK_NUM 时的数值
  char *loc;      // token 位置
  int len;        // token 长度
};

Token *tokenize(char *p);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);

//
// parse.c
//
// Local variable
typedef struct Obj Obj;

struct Obj {
  Obj *next;
  char *name;
  int offset;
};

// 抽象语法树节点的种类
typedef enum {
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_NEG,       // unary -
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_RETURN,    // "return"
  ND_IF,        // "if"
  ND_FOR,       // "for"
  ND_BLOCK,     // { ... }
  ND_EXPR_STMT, // Expression statement
  ND_VAR,       // Variable
  ND_NUM,       // Integer
} NodeKind;

typedef struct Node Node;

// 抽象语法树节点的类型
// 抽象语法树节点的类型
struct Node {
  NodeKind kind; // 节点类型
  Node *next;    // Next node

  Node *lhs; // Left-hand side
  Node *rhs; // Right-hand side

  // "if" or "for" statement
  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  // Block
  Node *body;

  Obj *var; // Used if kind == ND_VAR
  int val;  // Used if kind == ND_NUM
};

// Function
typedef struct Function Function;
struct Function {
  Node *body;
  Obj *locals;
  int stack_size;
};

Function *parse(Token *tok);

//
// codegen.c
//

void codegen(Function *prog);

#endif
