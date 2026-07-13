# 附录 2：使用 Git 进行版本管理

Git 最初是为了 Linux kernel 的版本管理而开发的。Linux kernel 是由数千名开发者组成的巨大项目，为了满足这种复杂工作流，Git 拥有丰富功能。这些功能很方便，但在只有自己一个 committer 的个人开发中，并不需要熟练掌握 Git 的全部功能。要精通 Git 需要学习很多东西，但本书中需要记住的事项很少。下面为第一次使用 Git 开发的人准备了 cheat sheet。

- `git add 文件名`

  把新创建的文件加入 repository。

- git commit -A

- `git commit -A`

  把对 working tree 做出的所有修改汇总 commit（会启动 editor，需要输入 commit message）。

- git reset --hard

- `git reset --hard`

  取消自上一次 commit 以来对 working tree 做出的所有修改。

- git log -p

- `git log -p`

  查看过去的 commit。

- git push

- `git push`

  把 repository push 到 GitHub 等 upstream。

## 使用 Git 的工作流

为了第一次使用版本管理系统的人，这里稍微说明一下 Git 和版本管理系统的概念。

Git 是管理“包含文件修改历史的数据库”的工具。这个数据库称为 repository。从 GitHub 等地方 clone repository 时，repository 会被下载下来，然后 Git 会从 repository 中把默认最新状态的 directory tree 展开到当前目录下。

从 repository 展开的 directory tree 称为“working tree”。大家会在 working tree 中用 editor 编辑 source file 或进行编译，但 working tree 本身并不是 repository 的一部分。working tree 可以说像是从 zip 文件展开出的文件；无论对它做多少修改，原本的 repository 都会保持原样。

自己对 working tree 做出的修改，会在合适的节点汇总成称为“commit”的单位，并写回 repository。这样数据库会被更新，之后又可以继续制作其他修改。使用 Git 时，开发会以“修改文件，然后 commit”的循环推进。

## commit 时的注意点

commit message 可以用日语，但请认真书写。例如写成“添加 `*` 和 `/` 作为运算符，但尚不处理运算符优先级”这样的一行消息即可。

commit 单位请尽量拆小。例如修改代码时，可能会想顺手做一点 refactoring；这种情况下，请把 refactoring 作为另一个 commit 提交。把两个以上不同功能合并到一个 commit 中并不理想。

不需要使用 Git 的高级功能。例如应该不需要使用 branch。

添加功能的代码，请务必和测试该功能的代码一起 commit。另外，commit 时请先运行测试，确认既有功能没有损坏，新功能也能正常工作后再 commit。换句话说，目标是：无论 checkout repository 的哪个时间点，编译和测试都能通过。不过，如果不小心 commit 了测试不通过的内容，也不需要改写 Git commit log 来修正；只要在下一个 commit 中修复即可。

## Git 的内部结构

阅读 Git 文档时会看到很多 tricky 的功能，但如果在脑中建立“Git 原理上如何保存数据”的模型，就更容易理解这些功能。因此这里说明 Git 的内部结构。

Git 是一种作为 user program 实现的文件系统。Git 数据库的结构与文件系统非常相似。不过，普通文件系统使用文件名访问文件，而 Git 不同，它使用文件的 hash 值作为名字。

这种由文件内容决定名字的机制称为 content-addressable 机制。在 content-addressable 文件系统中，如果名字相同，内容也相同。另外，内容不同的文件不能拥有相同名字，也就是相同 hash 值。这是通过使用密码学安全的 hash 函数来保证的。在这种文件系统中，不需要给文件另起名字；只要名字确定，文件内容也唯一确定。

commit 在 Git 内部也是文件。这个文件中除了 commit message，还写有属于该 commit 的文件 hash 值，以及前一个 commit 的 hash 值。

要从 Git 的文件系统中取出文件，就必须知道自己想要的文件的 hash 值。

如果不知道 commit file 的 hash 值就无法取得 commit file，这听起来像鸡和蛋的问题。但实际上，repository 中除了文件系统之外，还包含 commit file 的 hash 值及其对应名字的目录，因此可以用它找到 commit。例如 repository 中会包含这样的信息：名为 `"master"` 的历史（默认展开到 working tree 的历史）对应的 commit hash 值是 `da39a3ee5e...`。使用这些信息，Git 就可以把属于 `master` 的文件展开到 working tree。

“进行 commit”在内部是这样的：把发生修改的文件加入 Git 内部文件系统；同样加入一个包含这些文件 hash 值以及前一个 commit hash 值的 commit file；最后用该 commit file 的 hash 值更新目录。

例如，如果想让 `master` 的最后一个 commit 不存在（虽然最好不要这么做），只要查看 `master` 指向的 commit file，得到前一个 commit 的 hash 值，再用该 hash 值覆盖 `master` 即可。“branch”也不过是这样的东西：存在两个以上 commit，它们都把某个 commit 作为前一个 commit，而这两个 commit 都被登记在目录中。

这种 content-addressable 的版本管理系统还有安全上的优点。某个 commit 的名字，也就是 commit file 的 hash 值，包含属于该 commit 的所有文件的 hash 值，以及前一个 commit file 的 hash 值。前一个 commit file 又包含再前一个 commit file 的 hash，因此最终来说，到达最新 commit 的所有 commit 的 hash 值，都会被包含在最新 commit 的 hash 值计算中。因此，在不改变 hash 值的情况下偷偷篡改 commit 内容或历史，在原理上是不可能的。这是很有趣的性质。

学习 Git 功能时，请始终把这种 content-addressable 文件系统放在脑中。这样很多事情应该都会更容易理解。
