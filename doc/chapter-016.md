# 参考资料

- Compiler Explorer：方便的在线编译器。

- N1570 [PDF]：C11 语言规范的最终草案（内容与正式标准书相同）。

- N1570：HTML 版 C11 语言规范。

- X4J11/86-196 [PDF]：对 C 预处理器宏展开算法的说明。

- Rationale for American National Standard for Information Systems - Programming Language - C：说明标准化委员会在制定 C89 语言规范时考虑了哪些事项的文档。

- 8cc：作者编写的 C 编译器。

- 9cc：作者编写的 C 编译器。

- 试着从零开始开发 C 编译器（日记）。

- An Incremental Approach to Compiler Construction [PDF]：启发了增量式编译器开发思路的论文。

- Crafting Interpreters：Robert Nystrom 的在线书籍。

- A Retargetable C Compiler: Design and Implementation. David R. Hanson and Christopher W. Fraser：讲解简单 C 编译器 lcc 实现的书。

## 脚注

1. http://www.drdobbs.com/cpp/increasing-compiler-speed-by-over-75/240158941

   > DMD does memory allocation in a bit of a sneaky way. Since compilers are short-lived programs, and speed is of the essence, DMD just mallocs away, and never frees.

   DMD 的内存分配方式有一点取巧。由于编译器是短生命周期程序，而且速度至关重要，DMD 会一路调用 `malloc`，并且从不释放。

2. Linkers and Loaders, ISBN 978-1558604964, John R. Levine (1999) 1.2章

   > Programmers were using libraries of subprograms even before they used assemblers. By 1947, John Mauchly, who led the ENIAC project, wrote about loading programs along with subprograms selected from a catalog of programs stored on tapes, and of the need to relocate the subprograms' code to reflect the addresses at which they were loaded. Perhaps surprisingly, these two basic linker functions, relocation and library search, appear to predate even assemblers, as Mauchly expected both the program and subprograms to be written in machine language. The relocating loader allowed the authors and users of the subprograms to write each subprogram as though it would start at location zero, and to defer the actual address binding until the subprograms were linked with a particular main program.

   程序员甚至在使用汇编器之前，就已经在使用子程序库了。到 1947 年，领导 ENIAC 项目的 John Mauchly 已经写到：可以把程序与从磁带程序目录中选出的子程序一起加载，并且需要重定位子程序代码，以反映它们被加载到的地址。也许令人意外的是，重定位和库搜索这两个基本链接器功能似乎早于汇编器出现，因为 Mauchly 预期程序和子程序都用机器语言编写。重定位 loader 使子程序的作者和使用者可以像子程序从地址 0 开始一样编写它，并把实际地址绑定推迟到子程序与特定主程序链接时再完成。

3. https://parisc.wiki.kernel.org/images-parisc/b/b2/Rad_11_0_32.pdf The 32-bit PA-RISC run-time architecture document, v. 1.0 for HP-UX 11.0, 2.2.3章

   > When a process is initiated by the operating system, a virtual address range is allocated for that process to be used for the call stack, and the stack pointer (GR 30) is initialized to point to the low end of this range. As procedures are called, the stack pointer is incremented to allow the called procedure frame to exist at the address below the stack pointer. When procedures are exited, the stack pointer is decremented by the same amount.

   当操作系统启动一个进程时，会为该进程分配一个虚拟地址范围作为调用栈使用，并将栈指针（GR 30）初始化为指向这个范围的低端。随着过程调用发生，栈指针会递增，使被调用过程的栈帧存在于栈指针以下的地址处。过程退出时，栈指针会按相同的量递减。

4. https://www.acsac.org/2002/papers/classic-multics.pdf

   > Thirty Years Later: Lessons from the Multics Security Evaluation, "Third, stacks on the Multics processors grew in the positive direction, rather than the negative direction. This meant that if you actually accomplished a buffer overflow, you would be overwriting unused stack frames, rather than your own return pointer, making exploitation much more difficult.

   Thirty Years Later: Lessons from the Multics Security Evaluation 中写道：“第三，Multics 处理器上的栈向正方向增长，而不是向负方向增长。这意味着即使你真的造成了缓冲区溢出，也会覆盖未使用的栈帧，而不是自己的返回指针，从而让利用变得困难得多。”

5. https://en.wikipedia.org/wiki/ASCII#cite_note-Mackenzie_1980-1

   > There was some debate at the time whether there should be more control characters rather than the lowercase alphabet.

   当时曾经有过一些争论：是否应该设置更多控制字符，而不是加入小写字母表。

6. http://catb.org/~esr/jargon/html/L/language-lawyer.html

   > language lawyer: n. A person, usually an experienced or senior software engineer, who is intimately familiar with many or most of the numerous restrictions and features (both useful and esoteric) applicable to one or more computer programming languages. A language lawyer is distinguished by the ability to show you the five sentences scattered through a 200-plus-page manual that together imply the answer to your question “if only you had thought to look there”. Compare wizard, legal, legalese.

   language lawyer：名词。通常指经验丰富或资深的软件工程师，熟悉一种或多种计算机编程语言中许多限制和特性，包括有用的和晦涩的部分。language lawyer 的特点是，能指出一本 200 多页手册里分散的五句话，而这些话合在一起才推出你的问题答案，“只要你想得到去那里找”。可比较 wizard、legal、legalese。

7. C Programming Language, 2nd Edition (ISBN 978-0131103627), 5.12, p.122

   > C is sometimes castigated for the syntax of its declarations, particularly ones that involve pointers to functions.

   C 有时会因为其声明语法而受到批评，尤其是涉及函数指针的声明。

8. Go's Declaration Syntax by Rob Pike

   Go's Declaration Syntax，作者 Rob Pike。
