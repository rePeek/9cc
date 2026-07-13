# 附录 1：x86-64 指令集 cheat sheet

本章汇总了本书制作的编译器中使用的 x86-64 指令集功能。为了简洁表达，本章使用下面这些缩写记法。

- `src`, `dst`：两个大小相同的任意寄存器

- `r8`, `r16`, `r32`, `r64`：分别表示 8 bit、16 bit、32 bit、64 bit 寄存器

- imm: 即値

- `imm`：立即数

- `reg1:reg2`：把两个寄存器 `reg1`、`reg2` 分别作为高位和低位，用来表示 128 bit 这种无法放入一个寄存器的大数的记法

## 整数寄存器列表

下面的表汇总了 64 bit 整数寄存器及其 alias 名称。

| 64 | 32 | 16 | 8 |
| --- | --- | --- | --- |
| RAX | EAX | AX | AL |
| RDI | EDI | DI | DIL |
| RSI | ESI | SI | SIL |
| RDX | EDX | DX | DL |
| RCX | ECX | CX | CL |
| RBP | EBP | BP | BPL |
| RSP | ESP | SP | SPL |
| RBX | EBX | BX | BL |
| R8 | R8D | R8W | R8B |
| R9 | R9D | R9W | R9B |
| R10 | R10D | R10W | R10B |
| R11 | R11D | R11W | R11B |
| R12 | R12D | R12W | R12B |
| R13 | R13D | R13W | R13B |
| R14 | R14D | R14W | R14B |
| R15 | R15D | R15W | R15B |

ABI 中的用法如下。函数返回时不需要恢复原值的寄存器标记为 `✔`。

| --- | --- | --- |
| RDI | 第1引数 | ✔ |
| RSI | 第2引数 | ✔ |
| RDX | 第3引数 | ✔ |
| RCX | 第4引数 | ✔ |
| R8 | 第5引数 | ✔ |
| R9 | 第6引数 | ✔ |

进行函数调用时，需要在 `RSP` 是 16 的倍数，也就是 16 字节对齐的状态下调用 `call` 指令。不满足这个条件的函数调用不符合 ABI，某些函数可能会崩溃。

## 内存访问

| 指令 | 含义 |
| --- | --- |
| `mov dst, [r64]` | 从 `r64` 指向的地址 load 值到 `dst` |
| `mov [r64], src` | 把 `src` 的值 store 到 `r64` 指向的地址 |
| `push r64/imm` | `RSP` 减 8，并把 `r64/imm` store 到 `RSP` |
| `pop r64` | 从 `RSP` load 到 `r64`，并让 `RSP` 加 8 |

## 函数调用

| 指令 | 含义 |
| --- | --- |
| `call label` | 把 `RIP` push 到栈上，并跳转到 `label` |
| `call r64` | 把 `RIP` push 到栈上，并跳转到 `r64` 的地址 |
| `ret` | pop 栈并跳转到该地址 |
| `leave` | 等价于 `mov rsp, rbp` 后接 `pop rbp` |

## 条件分支

| 指令序列 | 含义 |
| --- | --- |
| `cmp reg1, reg2/imm`<br>`je label` | 如果 `reg1 == reg2/imm`，跳转到 `label` |
| `cmp reg1, reg2/imm`<br>`jne label` | 如果 `reg1 != reg2/imm`，跳转到 `label` |
| `cmp reg1, reg2/imm`<br>`jl label` | 如果 `reg1 < reg2/imm`，跳转到 `label`（有符号比较） |
| `cmp reg1, reg2/imm`<br>`jle label` | 如果 `reg1 <= reg2/imm`，跳转到 `label`（有符号比较） |

## 条件赋值

| 指令序列 | 含义 |
| --- | --- |
| `cmp reg1, reg2/imm`<br>`sete al`<br>`movzb eax, al` | `RAX = (reg1 == reg2/imm) ? 1 : 0` |
| `cmp reg1, reg2/imm`<br>`setne al`<br>`movzb eax, al` | `RAX = (reg1 != reg2/imm) ? 1 : 0` |
| `cmp reg1, reg2/imm`<br>`setl al`<br>`movzb eax, al` | `RAX = (reg1 < reg2/imm) ? 1 : 0`（有符号比较） |
| `cmp reg1, reg2/imm`<br>`setle al`<br>`movzb eax, al` | `RAX = (reg1 <= reg2/imm) ? 1 : 0`（有符号比较） |

## 整数和逻辑运算

| 指令 | 含义 |
| --- | --- |
| `add dst, src/imm` | `dst = dst + src/imm` |
| `sub dst, src/imm` | `dst = dst - src/imm` |
| `mul src` | `RDX:RAX = RAX * src` |
| `imul dst, src` | `dst = dst * src` |
| `div r32` | `EAX = EDX:EAX / r32`，`EDX = EDX:EAX % r32` |
| `div r64` | `RAX = RDX:RAX / r64`，`RDX = RDX:RAX % r64` |
| `idiv r32/r64` | `div` 的有符号版本 |
| `cqo` | 把 `RAX` 符号扩展为 128 bit，并 store 到 `RDX:RAX` |
| `and dst, src` | `dst = src & dst` |
| `or dst, src` | `dst = src | dst` |
| `xor dst, src` | `dst = src ^ dst` |
| `neg dst` | `dst = -dst` |
| `not dst` | `dst = ~dst` |
| `shl dst, imm/CL` | 将 `dst` 左移 `imm` 或 `CL` 寄存器的值指定的位数。用寄存器指定移位量时，只能使用 `CL` |
| `shr dst, imm/CL` | 将 `dst` 逻辑右移 `imm` 或 `CL` 指定的位数，移入的高位 bit 清零 |
| `sar dst, imm/CL` | 将 `dst` 算术右移 `imm` 或 `CL` 指定的位数，移入的高位 bit 与原 `dst` 的 sign bit 相同 |
| `lea dst, [src]` | 计算 `[src]` 的地址，但不进行内存访问，而是把地址计算结果本身 store 到 `dst` |
| `movsb dst, r8` | 对 `r8` 进行符号扩展后 store 到 `dst` |
| `movzb dst, r8` | 不对 `r8` 进行符号扩展，store 到 `dst` |
| `movsw dst, r16` | 对 `r16` 进行符号扩展后 store 到 `dst` |
| `movzw dst, r16` | 不对 `r16` 进行符号扩展，store 到 `dst` |
