# static link 与 dynamic link

本书到目前为止只使用了 static link 这个功能。static link 是一种直接明了的执行模型，因此聚焦这个模型进行说明，可以比较容易地解释汇编代码、可执行文件的内存映像等内容。但实际上，在创建一般可执行文件时，static link 并没有那么广泛使用。实际广泛使用的是 dynamic link，而不是 static link。

本章会说明 static link 和 dynamic link。

默认情况下，编译器和 linker 会试图输出进行 dynamic link 的可执行文件。读者到目前为止应该也见过：如果忘记给 `cc` 加上 `-static` 选项，就会出现下面这样的错误（如果没见过，可以从 `Makefile` 中去掉 `-static` 选项后执行 `make`）。

```console
$ cc -o tmp tmp.s
/usr/bin/ld: /tmp/ccaRuuub.o: relocation R_X86_64_32S against `.data' can not be
/usr/bin/ld: final link failed: Nonrepresentable section on output
```

linker 默认会尝试进行 dynamic link，但要 dynamic link，编译器必须输出能够 dynamic link 的汇编代码。9cc 目前还不会输出这种代码，所以忘记加 `-static` 时，就会显示上面的错误。阅读本章后，应该就能理解这个错误的含义，并知道为了解决这个错误必须做什么。

## static link

static link 后的可执行文件是自包含的可执行文件，运行时不需要其他文件。例如 `printf` 等函数不是用户写的函数，而是 libc 标准库中的函数；但通过 static link 创建可执行文件时，`printf` 的代码会从 libc 复制到可执行文件中。执行 static link 后的程序时不需要 libc，因为 libc 中必要的代码和数据已经复制到了可执行文件里。

这里实际看一下，下面这个简单程序 `hello.c` 会如何变成 static link 后的可执行文件。

```c
#include <stdio.h>

int main() {
  printf("Hello world!\n");
}
```

要编译、链接这个 `hello.c` 文件，并转换成名为 `hello` 的可执行文件，需要输入下面的命令。

```console
$ cc -c hello.c
$ cc -o hello hello.o
```

在上面的命令中，第 1 行编译 `hello.c` 并创建 object file `hello.o`，第 2 行把它链接成可执行文件。也可以把这两条命令合并写成 `cc -o hello hello.c`，但即使这样启动编译器，内部也会执行与上面两条命令等价的处理。

`hello.c` include 了 `stdio.h`，但如本书到目前为止所见，header file 中并不包含函数本体的代码。因此，在创建 `hello.o` 文件时，编译器知道 `stdio.h` 中声明的 `printf` 函数存在以及它的类型，但对 `printf` 的实际代码一无所知。因此，`printf` 的代码不可能包含在 `hello.o` 文件中。实际上，`hello.o` 中只包含 `main` 的定义。把 `hello.o` 和包含 `printf` 的 object file 组合起来，完成可执行文件，是 linker 的职责。

第 2 行通过 `cc` 启动 linker 时，传给 linker 的不仅是命令行中传入的 `hello.o` 文件，还包括系统标准库路径 `/usr/lib/x86_64-linux-gnu/libc.a`。`printf` 函数包含在这个 `libc.a` 中。`.a` 和 `.tar`、`.zip` 一样，是 archive file。稍微看一下里面的内容。

```console
$ ar t /usr/lib/x86_64-linux-gnu/libc.a
...
printf_size.o
fprintf.o
printf.o
snprintf.o
sprintf.o
...
```

musl 的 `printf.c`

archive file 中包含，

static link 后的可执行文件执行模型很简单。运行时内存中只存在这个可执行文件，所以可执行文件的各个 segment 可以加载到内存中的任意位置。尝试加载到链接时确定的默认地址时，不会发生加载失败，因为可执行文件被加载之前，内存中没有放置任何东西。因此，在 static link 中，所有全局变量和函数的地址都可以在链接时决定。

static link 有以下优点。

- 简单，加载速度快。

- 没有依赖文件，只要复制可执行文件就能运行。

- 即使存在行为略有差异的不同版本库，static link 复制库代码和数据时已经固定下来，所以同一个可执行文件在任何环境中都会以相同方式运行。

static link 有以下缺点。

- 可执行文件使用的库函数和代码会被复制，因此会稍微浪费磁盘和内存。

- 如果库中的 bug 被修复，要把该修改反映到已有可执行文件中，需要重新链接。

另一方面，dynamic link 后的可执行文件在运行时需要其他文件，例如 Unix 上的 `.so` 或 Windows 上的 `.dll`。`.so` 或 `.dll` 中包含 `printf` 等函数代码，以及 `errno` 这样的全局变量。`.so`、`.dll` 等文件称为动态库，或简称库，也称为 DSO（dynamic shared object）。
