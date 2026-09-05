	.file	"tmp_test.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp
	mov DWORD PTR -4[rbp], 10
	cmp	DWORD PTR -4[rbp], 5
	jle	.L2
	mov	DWORD PTR -4[rbp], 1
.L2:
	mov	eax, DWORD PTR -4[rbp]
	pop	rbp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
