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
