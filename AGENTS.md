# Repository Guidelines

## 项目结构与模块组织

这是一个基于 chibicc commit 历史逐步实现的小型 C 编译器项目。Nix 开发环境由 `flake.nix` 和
`flake.lock` 定义。

### 参考优先级

1. **主参考：chibicc 仓库的 commit 历史**

   源码仓库：https://github.com/rui314/chibicc

   实现功能时，应先查看对应 commit 的具体实现（tokenize.c、parse.c、
   codegen.c），以此为准。

2. **辅助参考：`doc/` 中的书籍翻译章节**

   `doc/` 保存《低レイヤを知りたい人のためのCコンパイラ作成入門》的中文
   翻译章节和笔记。用于理解概念、术语、设计动机和编译原理背景。当 chibicc
   commit 的实现与书中描述有差异时（如 chibicc 用 `TK_KEYWORD` 统一关键字，
   书中用逐个 token 类型；chibicc 用 `gen_expr`/`gen_stmt` 分离，书中用单一
   `gen`），以 chibicc 为准。

## Agent 专用说明

不要覆盖用户已有修改。构建、测试和格式化命令都放在 `nix develop` 中执行，
或使用 `nix develop -c ...`。回答实现问题或规划修改前，先查看 chibicc
对应 commit 的具体实现作为主要依据，再参考 `doc/` 中的章节理解背景和动机。

## 构建、测试与开发命令

`make` 会从 `*.c` 构建 `9cc` 可执行文件。`make test` 会构建编译器，
生成测试表达式的汇编，静态链接生成的汇编，并检查程序退出码。一次性执行
命令时使用：

```sh
nix develop -c make test
```

使用 `make clean` 清理生成的二进制和临时文件。

## 代码风格与命名约定

项目使用 C11，并通过 `.clang-format` 采用 LLVM 风格。格式化 C 代码：

```sh
nix develop -c clang-format -i *.c *.h
```

缩进以 clang-format 输出为准，当前为两空格。命名保持短小并贴合现有代码，
例如 `Token`、`Node`、`tokenize`、`expr`、`mul`、`primary`。除非能明显
简化 parser 或代码生成逻辑，否则优先使用直接、简单的 C 写法。

## 测试指南

测试使用 shell 脚本 `test.sh`。成功用例使用：

```sh
assert expected "input"
```

错误输出用例使用 `assert_error`。修改 parser 或代码生成时，应覆盖优先级、
结合性、括号和诊断信息。提交前始终运行：

```sh
nix develop -c make test
```

## 提交与 Pull Request 规范

近期提交使用简短的祈使句摘要，例如 `Add recursive descent parser` 和
`Improve parse error reporting`。提交应保持聚焦，行为变更要包含测试。
Pull Request 应说明编译器功能或 bug 修复内容，列出验证命令，并注明哪些
生成文件是刻意排除的。
