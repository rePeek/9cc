# Repository Guidelines

## 项目结构与模块组织

这是一个基于 `doc/` 中文档逐步实现的小型 C 编译器项目。核心实现位于
`9cc.c`。构建与测试入口分别是 `Makefile` 和 `test.sh`。`doc/` 保存翻译
章节和参考笔记，是理解当前功能、术语和实现顺序的主要上下文来源。讨论语法、
代码生成或行为预期时，应优先参考相关章节。`tmp`、`tmp.s`、`tmp.err`、`9cc`
等是本地生成产物，不应提交。Nix 开发环境由 `flake.nix` 和 `flake.lock`
定义。

## 构建、测试与开发命令

所有项目命令都应在 Nix 开发环境中运行：

```sh
nix develop
make
make test
```

`make` 会从 `9cc.c` 构建 `9cc` 可执行文件。`make test` 会构建编译器，
生成测试表达式的汇编，静态链接生成的汇编，并检查程序退出码。一次性执行
命令时使用：

```sh
nix develop -c make test
```

使用 `make clean` 清理生成的二进制和临时文件。

## 代码风格与命名约定

项目使用 C11，并通过 `.clang-format` 采用 LLVM 风格。格式化 C 代码：

```sh
nix develop -c clang-format -i 9cc.c
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

## Agent 专用说明

不要覆盖用户已有修改。构建、测试和格式化命令都放在 `nix develop` 中执行，
或使用 `nix develop -c ...`。回答实现问题或规划修改前，优先检查 `doc/`
中对应章节，确保沟通上下文和代码演进与文档保持一致。
