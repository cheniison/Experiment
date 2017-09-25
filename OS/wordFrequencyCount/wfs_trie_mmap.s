	.file	"wfs_trie_mmap.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%lu\t%s\n"
	.text
	.globl	print
	.type	print, @function
print:
.LFB81:
	.cfi_startproc
	testq	%rdi, %rdi
	je	.L8
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdx, %r13
	movq	%rsi, %r12
	movq	%rdi, %rbp
	cmpq	$0, 208(%rdi)
	je	.L3
	movb	$0, (%rsi,%rdx)
	movq	208(%rdi), %rdx
	movq	%rsi, %rcx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
.L3:
	movl	$97, %ebx
	leaq	1(%r13), %r14
.L4:
	movb	%bl, (%r12,%r13)
	movq	%r14, %rdx
	movq	%r12, %rsi
	movq	0(%rbp), %rdi
	call	print
	addl	$1, %ebx
	addq	$8, %rbp
	cmpb	$123, %bl
	jne	.L4
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_restore 13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_restore 14
	.cfi_def_cfa_offset 8
.L8:
	rep ret
	.cfi_endproc
.LFE81:
	.size	print, .-print
	.section	.rodata.str1.1
.LC1:
	.string	"essay.txt"
.LC3:
	.string	"%f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB82:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$216, %rsp
	.cfi_def_cfa_offset 240
	movq	%fs:40, %rax
	movq	%rax, 200(%rsp)
	xorl	%eax, %eax
	call	clock
	movq	%rax, %rbx
	movl	$0, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	open
	movl	%eax, %ebp
	movq	%rsp, %rdx
	movl	%eax, %esi
	movl	$1, %edi
	call	__fxstat
	movl	$1, %edx
	testl	%eax, %eax
	js	.L10
	movl	$0, %r9d
	movl	%ebp, %r8d
	movl	$2, %ecx
	movq	48(%rsp), %rsi
	movl	$0, %edi
	call	mmap
	movq	$word_tree, cur(%rip)
	movq	%rax, %rsi
	jmp	.L11
.L16:								# ch 不是 '\0'
	andl	$-33, %edx				# %edx = ch & 11011111
	subl	$65, %edx				# %edx -= 65
	cmpb	$25, %dl				# %dl - 25
	jbe	.L12						# <= 0
	movq	cur(%rip), %rdx
	cmpq	$word_tree, %rdx		
	je	.L11
	addq	$1, 208(%rdx)			# 8 * 26
	movq	$word_tree, cur(%rip)
	jmp	.L11
.L12:								# ch 是字母
	leal	32(%rcx), %edx			# ch + 32 -> edx
	cmpl	$90, %ecx				# ch - 90
	cmovle	%edx, %ecx				# 
	leal	-97(%rcx), %edx			# %edx -> ch - 'a'
	movslq	%ecx, %rcx				
	movq	cur(%rip), %rdi			# rdi -> cur
	leaq	(%rdi,%rcx,8), %rcx		# rcx -> cur + 8 * ch
	cmpq	$0, -776(%rcx)			# 776 = 8 * 97
	jne	.L15						# == NULL
	movq	witer(%rip), %rdi		# %rdi -> witer
	leaq	1(%rdi), %r8
	movq	%r8, witer(%rip)		# 更新witer
	imulq	$216, %rdi, %rdi		# rdi -> 208 + 8 * witer 
	addq	$words, %rdi			# rdi -> words + witer
	movq	%rdi, -776(%rcx)		# 分配
.L15:
	movslq	%edx, %rdx				# 
	movq	cur(%rip), %rcx			# rcx -> cur
	movq	(%rcx,%rdx,8), %rdx		# rdx -> cur + 8 * (ch - 'a')
	movq	%rdx, cur(%rip)			# 
.L11:
	addq	$1, %rsi			# rsi -> atmp
	movzbl	-1(%rsi), %edx		# edx -> ch
	movsbl	%dl, %ecx			# dl -> ch 低八位
	testl	%ecx, %ecx			# 是否为 '\0'
	jne	.L16
	movq	48(%rsp), %rsi
	movq	%rax, %rdi
	call	munmap
	call	clock
	movq	%rax, %rbp
	movl	$0, %edx
	leaq	144(%rsp), %rsi
	movl	$word_tree, %edi
	call	print
	subq	%rbx, %rbp
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rbp, %xmm0
	divsd	.LC2(%rip), %xmm0
	movl	$.LC3, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movl	$0, %edx
.L10:
	movl	%edx, %eax
	movq	200(%rsp), %rbx
	xorq	%fs:40, %rbx
	je	.L17
	call	__stack_chk_fail
.L17:
	addq	$216, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE82:
	.size	main, .-main
	.comm	cur,8,8
	.comm	word_tree,216,32
	.globl	witer
	.bss
	.align 8
	.type	witer, @object
	.size	witer, 8
witer:
	.zero	8
	.comm	words,21600000,32
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
