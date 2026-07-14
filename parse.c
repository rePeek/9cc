#include "9cc.h"

// 如果下一个 token 是期望的符号，就读取一个 token
// 并返回 true；否则返回 false。
static bool consume(char *op) {
  if (token->kind != TK_RESERVED || (int)strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

// 如果下一个 token 是期望的符号，就读取一个 token。
// 否则报告错误。
static void expect(char *op) {
  if (token->kind != TK_RESERVED || (int)strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    error_at(token->str, "不是 '%s'", op);
  token = token->next;
}

// 如果下一个 token 是数值，就读取一个 token 并返回该数值。
// 否则报告错误。
static int expect_number(void) {
  if (token->kind != TK_NUM)
    error_at(token->str, "不是数字");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof(void) { return token->kind == TK_EOF; }

// 创建新 token 并连接到 cur
static Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

static bool startswith(char *p, char *q) {
  return strncmp(p, q, strlen(q)) == 0;
}

// tokenize 输入字符串 p 并返回结果
Token *tokenize(char *p) {
  Token head = {0};
  Token *cur = &head;

  while (*p) {
    // 跳过空白字符
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") ||
        startswith(p, ">=")) {
      cur = new_token(TK_RESERVED, cur, p);
      cur->len = 2;
      p += 2;
      continue;
    }

    if (strchr("+-*/()<>", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      cur->len = 1;
      continue;
    }

    if (isdigit(*p)) {
      char *q = p;
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      cur->len = p - q;
      continue;
    }

    error_at(p, "无法 tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

static Node *equality(void);
static Node *relational(void);
static Node *add(void);
static Node *mul(void);
static Node *unary(void);
static Node *primary(void);

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

static Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr(void) { return equality(); }

static Node *equality(void) {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node(ND_NE, node, relational());
    else
      return node;
  }
}

static Node *relational(void) {
  Node *node = add();

  for (;;) {
    if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume("<="))
      node = new_node(ND_LE, node, add());
    else if (consume(">")) {
      Node *rhs = add();
      node = new_node(ND_LT, rhs, node);
    } else if (consume(">=")) {
      Node *rhs = add();
      node = new_node(ND_LE, rhs, node);
    } else
      return node;
  }
}

static Node *add(void) {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

static Node *mul(void) {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

static Node *unary(void) {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

static Node *primary(void) {
  // 如果下一个 token 是 "(",则应为 "(" expr ")"
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  // 否则应为数值
  return new_node_num(expect_number());
}
