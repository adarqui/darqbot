	.file "mod_intel.S"
	.text
.global intel_true
.global intel_false
.global intel_self
.global intel_strlen
.global intel_strcmp
.global intel_bzero
.global intel_memset
.global intel_mul
.global intel_imul
.global intel_sub
.global intel_add
.global intel_div
.global intel_idiv
.global intel_mod
.global intel_imod
.global intel_neg
.global intel_eflags
.global intel_and
.global intel_or
.global intel_xor
.global intel_not

	.type intel_true, @function
	.type intel_false, @function
	.type intel_self, @function
	.type intel_strlen, @function
	.type intel_strcmp, @function
	.type intel_bzero, @function
	.type intel_memset @function
	.type intel_mul, @function
	.type intel_imul, @function
	.type intel_sub, @function
	.type intel_add, @function
	.type intel_div, @function
	.type intel_idiv, @function
	.type intel_mod, @function
	.type intel_imod, @function
	.type intel_neg, @function
	.type intel_eflags, @function
	.type intel_and, @function
	.type intel_or, @function
	.type intel_xor, @function
	.type intel_not, @function

intel_true:
# int_true(void)
	pushl %ebp
	movl %esp, %ebp
	movl $1, %eax
	popl %ebp
	ret

intel_false:
# int false(void)
	pushl %ebp
	movl %esp, %ebp
	movl $0, %eax
	popl %ebp
	ret

intel_self:
# void * self(void *)
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %eax
	popl %ebp
	ret

intel_strlen:
# size_t strlen(const char *)
	pushl %ebp
	movl %esp, %ebp

	pushl %ecx
	pushl %edx

	movl 8(%ebp), %edx
	movl $0, %ecx
	cmp $0, %edx
		jz out_strlen
	redo_strlen:
		movb (%edx), %al
		inc %edx
		cmpb $0, %al
		je out_strlen
		inc %ecx
		jmp redo_strlen
	out_strlen:
	movl %ecx, %eax

	popl %edx
	popl %ecx

	popl %ebp
	ret

intel_bzero:
# void bzero(void *, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %ecx
	pushl %edx

	movl 8(%ebp), %edx
	xorl %ecx,%ecx

	cmp $0, %edx
	je out_bzero

	redo_bzero:
		movb $0, (%edx)
		inc %edx
		decl 12(%ebp)
		movb (%edx), %cl
		cmpb $0, %cl
		jne redo_bzero
	out_bzero:
	xorl %eax, %eax

	popl %edx
	popl %ecx

	popl %ebp
	ret	


intel_memset:
# void * memset(void *, int, size_t)
	pushl %ebp
	movl %esp, %ebp

	pushl %ecx
	pushl %edx
	
	movl 8(%ebp), %edx

	cmp $0, %edx
	je out_memset

	xorl %ecx, %ecx

	redo_memset:
		movb 12(%ebp), %cl
		movb %cl, (%edx)	
		inc %edx
		decl 16(%ebp)
		cmp $0, 16(%ebp)
		jne redo_memset

	out_memset:
	movl 8(%ebp), %eax

	popl %edx
	popl %ecx

	popl %ebp
	ret


intel_mul:
# int mul(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ebx
	mul %ebx

	popl %ebx
	popl %ebp
	ret

intel_imul:
# int imul(int, int)
	pushl %ebp
	movl %esp, %ebp
	
	pushl %ebx
	movl 8(%ebp), %eax
	movl 12(%ebp), %ebx
	imul %ebx

	popl %ebx
	leave
	ret

intel_sub:
# int sub(int, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %edx

	movl 8(%ebp), %eax
	movl 12(%ebp), %edx
	subl %edx, %eax

	popl %edx

	leave
	ret	


intel_add:
# int add(int, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %edx

	movl 8(%ebp), %eax
	movl 12(%ebp), %edx
	addl %edx, %eax

	popl %edx

	leave
	ret


intel_strcmp:
# int strcmp(const char *, const char *)
# this is bad, temporary solution
	pushl %ebp
	movl %esp, %ebp

	pushl 12(%ebp)
	call intel_strlen
	addl $4, %esp

	movl %eax, %edx

	pushl 8(%ebp)
	call intel_strlen
	addl $4, %esp

	cmpl %eax, %edx
	jnz out_strcmp
	
	movl %eax, %ecx
	movl 8(%ebp), %edi
	movl 12(%ebp), %esi

	xorl %eax,%eax
	repe cmpsb
	cmovne %ecx, %eax

	out_strcmp:
	popl %ebp
	ret


intel_div:
# int intel_div(int, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %edx

	movl 8(%ebp), %eax
	xorl %edx, %edx
	divl 12(%ebp)

	popl %edx

	leave
	ret


intel_idiv:
# int intel_idiv(int, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %edx

	movl 8(%ebp), %eax
	xorl %edx, %edx
	idivl 12(%ebp)
	
	popl %edx

	leave
	ret


intel_mod:
# int intel_mod(int, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %edx

	movl 8(%ebp), %eax
	xorl %edx, %edx
	divl 12(%ebp)
	movl %edx, %eax
	
	popl %edx

	leave
	ret


intel_imod:
# int intel_imod(int, int)
	pushl %ebp
	movl %esp, %ebp

	pushl %edx
	
	movl 8(%ebp), %eax
	xorl %edx,%edx
	idivl 12(%ebp)
	movl %edx, %eax
	
	leave
	ret

intel_neg:
# long intel_neg(long)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	neg %eax

	popl %ebp
	ret	

intel_eflags:
# unsigned int intel_eflags(void)
	pushl %ebp
	movl %esp, %ebp

	pushfl
	movl 0(%ebp), %eax
	popfl

	leave
	ret



intel_and:
# int intel_and(int,int)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	andl 12(%ebp), %eax

	leave
	ret


intel_or:
# int intel_or(int,int)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	orl 12(%ebp), %eax

	leave
	ret


intel_xor:
# int intel_xor(int, int)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	xorl 12(%ebp), %eax

	leave
	ret


intel_not:
# int intel_not(int)
	pushl %ebp
	movl %esp, %ebp

	notl 8(%ebp)
	movl 8(%ebp) , %eax

	leave
	ret
