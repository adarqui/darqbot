# .intel_syntax noprefix

	.file "mod_func_intel.S"
	.text
.global intel_true
.global intel_false
.global intel_self
.global intel_strlen
.global intel_strlen2
.global intel_strcmp
.global intel_strncmp
.global intel_strchr
.global intel_strrchr
.global intel_strcpy
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
.global intel_jmp
.global intel_sar
.global intel_shr
.global intel_sal
.global intel_shl
.global intel_ror
.global intel_rol
.global intel_bswap
.global intel_isdigit
.global intel_isbinary
.global intel_isalpha
.global intel_isalnum
.global intel_isascii
.global intel_ispunct
.global intel_isupper
.global intel_islower
.global intel_toupper
.global intel_tolower
	.type intel_true, @function
	.type intel_false, @function
	.type intel_self, @function
	.type intel_strlen, @function
	.type intel_strlen2, @function
	.type intel_strcmp, @function
	.type intel_strncmp, @function
	.type intel_strchr, @function
	.type intel_strrchr, @function
	.type intel_strpcy, @function
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
	.type intel_jmp, @function
	.type intel_sar, @function
	.type intel_shr, @function
	.type intel_sal, @function
	.type intel_shl, @function
	.type intel_ror, @function
	.type intel_rol, @function
	.type intel_bswap, @function
	.type intel_isdigit, @function
	.type intel_isbinary, @function
	.type intel_isalpha, @function
	.type intel_isalnum, @function
	.type intel_isascii, @function
	.type intel_ispunct, @function
	.type intel_isupper,@function
	.type intel_islower,@function
	.type intel_toupper,@function
	.type intel_tolower,@function


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



intel_strlen2:
# repne/scasb variation
        pushl %ebp
        movl %esp, %ebp

	pushl %ecx
        pushl %edi

        cld
        movl 8(%ebp), %edi
	xorl %eax, %eax
	xorl %ecx, %ecx
	not %ecx
        repne scasb
	not %ecx
	movl %ecx, %eax

        popl %edi
	popl %ecx

        leave
        ret




intel_strcmp:
# int intel_strcmp(const char *, const char *)
        pushl %ebp
        movl %esp, %ebp

	pushl %esi
	pushl %edi
	pushl %ecx

        movl 8(%ebp), %edi
        movl 12(%ebp), %esi

        .Lintel_strcmp_strlen:
                movl 8(%ebp), %eax
                xorl %ecx, %ecx
                jmp .Lintel_strcmp_strlen_end
                .Lintel_strcmp_strlen_begin:
                inc %eax
                inc %ecx
                .Lintel_strcmp_strlen_end:
                cmpb $0, (%eax)
                jne .Lintel_strcmp_strlen_begin

        xorl %eax, %eax

        cld
        repe cmpsb
        jne .Lintel_strcmp_noteq
        jmp .Lintel_strcmp_eq

        .Lintel_strcmp_noteq:

        movl $-1, %eax
        jg .Lintel_strcmp_eq
        neg %eax

        .Lintel_strcmp_eq:

	popl %ecx
	popl %edi
	popl %esi

        leave
        ret




intel_strncmp:
# int intel_strncmp(const char *, const char *, size_t)
        pushl %ebp
        movl %esp, %ebp

	pushl %esi
	pushl %edi
	pushl %ecx

        movl 8(%ebp), %edi
        movl 12(%ebp), %esi
	movl 16(%ebp), %ecx

        xorl %eax, %eax

        cld
        repe cmpsb
        jne .Lintel_strncmp_noteq
        jmp .Lintel_strncmp_eq

        .Lintel_strncmp_noteq:

        movl $-1, %eax
        jg .Lintel_strncmp_eq
        neg %eax

        .Lintel_strncmp_eq:

	popl %ecx
	popl %edi
	popl %esi

        leave
        ret




intel_strchr:
# char * intel_strchr(char *, int)
        pushl %ebp
        movl %esp, %ebp

        pushl %edi

        movl 12(%ebp), %eax
        movl 8(%ebp), %edi

        jmp .Lstrchr_loop_end
        .Lstrchr_loop_begin:
        inc %edi
        .Lstrchr_loop_end:
        cmpb $0, (%edi)
        je .Lstrchr_zero
        cmpb %al, (%edi)
        je .Lstrchr_done
        jmp .Lstrchr_loop_begin

        .Lstrchr_zero:
        xorl %edi, %edi

        .Lstrchr_done:
        movl %edi, %eax

        popl %edi

        leave
        ret





intel_strrchr:
# char * intel_strrchr(char *, int)
        pushl %ebp
        movl %esp, %ebp

        pushl %ebx
	pushl %ecx
        pushl %edi

        movl 8(%ebp), %edi

        pushl %edi
        call intel_strlen2
        addl $4, %esp

        cmpl $0, %eax
        jle .Lintel_strrchr_done

        addl %eax, %edi
        movl %eax, %ecx
        movl 12(%ebp), %eax
        std
        repne scasb
        je .Lintel_strrchr_match
        xorl %eax, %eax
        jmp .Lintel_strrchr_done

        .Lintel_strrchr_match:
	incl %edi
        movl %edi, %eax

        .Lintel_strrchr_done:

        popl %edi
	popl %ecx
        popl %ebx
        leave
        ret



intel_strcpy:
        pushl %ebp
	movl %esp, %ebp

	pushl %esi
	pushl %edi

	movl 8(%ebp), %edi
	movl 12(%ebp), %esi

	.Lintel_strcpy_start:
	movb (%esi), %al
	movb %al, (%edi)
	cmpb $0, %al
	je .Lintel_strcpy_done
	incl %esi
	incl %edi
	jmp .Lintel_strcpy_start

	.Lintel_strcpy_done:
	movl 8(%ebp), %eax

	popl %edi
	popl %esi
	leave
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



intel_jmp:
# int intel_jmp(void *)
	pushl %ebp
	movl %esp, %ebp

	jmp *8(%ebp)

	leave
	ret


intel_sar:
# int intel_sar(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx
	sarl %cl, %eax

	popl %ecx
	leave
	ret


intel_shr:
# int intel_shr(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx

	shrl $cl, %eax

	popl %ecx
	leave
	ret


intel_sal:
# int intel_sal(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx
	sall %cl, %eax

	popl %ecx
	leave
	ret


intel_shl:
# int intel_shl(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx
	shll %cl, %eax

	popl %ecx
	leave
	ret

intel_ror:
# int intel_ror(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx
	rorl %cl, %eax

	popl %ecx
	leave
	ret


intel_rol:
# int intel_rol(int, int)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx

	movl 8(%ebp), %eax
	movl 12(%ebp), %ecx
	roll %cl, %eax

	popl %ecx
	leave
	ret

intel_bswap:
# int intel_bswap(int)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	bswap %eax

	leave
	ret


intel_isdigit:
# int intel_isdigit(int)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	cmpl $48, %eax
	jl .Lisdigit_mismatch

	cmpl $57, %eax
	jg .Lisdigit_mismatch

	movl $1, %eax
	jmp .Lisdigit_success

	.Lisdigit_mismatch:
	xorl %eax, %eax

	.Lisdigit_success:

	leave
	ret


intel_isbinary:
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	cmpl $48, %eax
	je .Lintel_isbinary_success

	cmpl $49, %eax
	je .Lintel_isbinary_success

	jmp .Lintel_isbinary_fail

	.Lintel_isbinary_success:
	movl $1, %eax
	jmp .Lintel_isbinary_done

	.Lintel_isbinary_fail:
	xorl %eax, %eax

	.Lintel_isbinary_done:
	leave
	ret


intel_isalpha:
# int intel_isalpha(int)
	pushl %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	cmpl $65, %eax
	jl .Lintel_isalpha_fail

	cmpl $90, %eax
	jle .Lintel_isalpha_success

	cmpl $97, %eax
	jl .Lintel_isalpha_fail

	cmpl $122, %eax
	jle .Lintel_isalpha_success

	.Lintel_isalpha_fail:
	xorl %eax, %eax
	jmp .Lintel_isalpha_done

	.Lintel_isalpha_success:
	movl $1, %eax

	.Lintel_isalpha_done:
	leave
	ret



intel_isalnum:
# int intel_isalnum(int)
	pushl %ebp
	movl %esp, %ebp

	pushl 8(%ebp)
	call intel_isalpha
	addl $4, %esp

	cmpl $1, %eax
	jge .Lintel_isalnum_done

	pushl 8(%ebp)
	call intel_isdigit
	addl $4, %esp

	.Lintel_isalnum_done:
	leave
	ret



intel_isupper:
# int intel_isupper(int)
        pushl %ebp
        movl %esp, %ebp
        pushl %ebx

        xorl %eax,%eax
        movl 8(%ebp), %ebx
        cmpl $65, %ebx
        jae .L2_intel_isupper
        movl $0, %eax
        jmp .L3_intel_isupper

.L2_intel_isupper:
        cmpl $90, %ebx
        setbe %bl
        movl %ebx, %eax

.L3_intel_isupper:
        popl %ebx
        leave
        ret





intel_islower:
# int intel_islower(int)
        pushl %ebp
        movl %esp, %ebp
        pushl %ebx

        xorl %eax,%eax
        movl 8(%ebp), %ebx
        cmpl $97, %ebx
        jae .L2_intel_islower
        movl $0, %eax
        jmp .L3_intel_islower

.L2_intel_islower:
        cmpl $122, %ebx
        setbe %bl
        movl %ebx, %eax

.L3_intel_islower:
        popl %ebx
        leave
        ret


intel_toupper:
# int intel_toupper(int)
	pushl %ebp
	movl %esp, %ebp

	pushl 8(%ebp)
	call intel_islower
	add $4, %esp

	cmp $0, %eax
	movl 8(%ebp), %eax
	jle out_toupper

	subl $32, %eax
	
	out_toupper:
	leave
	ret


intel_tolower:
# int intel_tolower(int)
        pushl %ebp
        movl %esp, %ebp

        pushl 8(%ebp)
        call intel_isupper
        add $4, %esp

        cmp $0, %eax
        movl 8(%ebp), %eax
        jle out_tolower

        addl $32, %eax

        out_tolower:
        leave
        ret

