# intel syntax, as opposed to mod_func_intel.S which is aTT syntax
.intel_syntax noprefix

	.file "mod_func_intel2.S"
	.text
.global intel2_true
.global intel2_false
.global intel2_self
.global intel2_strlen
.global intel2_strlen2
.global intel2_strcmp
.global intel2_strncmp
.global intel2_strchr
.global intel2_strrchr
.global intel2_strcpy
.global intel2_bzero
.global intel2_memset
.global intel2_mul
.global intel2_imul
.global intel2_sub
.global intel2_add
.global intel2_div
.global intel2_idiv
.global intel2_mod
.global intel2_imod
.global intel2_neg
.global intel2_eflags
.global intel2_and
.global intel2_or
.global intel2_xor
.global intel2_not
.global intel2_jmp
.global intel2_sar
.global intel2_shr
.global intel2_sal
.global intel2_shl
.global intel2_ror
.global intel2_rol
.global intel2_bswap
.global intel2_isdigit
.global intel2_isbinary
.global intel2_isalpha
.global intel2_isalnum
#.global intel2_isascii
.global intel2_ispunct
.global intel2_isupper
.global intel2_islower
.global intel2_toupper
.global intel2_tolower
	.type intel2_true, @function
	.type intel2_false, @function
	.type intel2_self, @function
	.type intel2_strlen, @function
	.type intel2_strlen2, @function
	.type intel2_strcmp, @function
	.type intel2_strncmp, @function
	.type intel2_strchr, @function
	.type intel2_strrchr, @function
	.type intel2_strcpy, @function
	.type intel2_bzero, @function
	.type intel2_memset @function
	.type intel2_mul, @function
	.type intel2_imul, @function
	.type intel2_sub, @function
	.type intel2_add, @function
	.type intel2_div, @function
	.type intel2_idiv, @function
	.type intel2_mod, @function
	.type intel2_imod, @function
	.type intel2_neg, @function
	.type intel2_eflags, @function
	.type intel2_and, @function
	.type intel2_or, @function
	.type intel2_xor, @function
	.type intel2_not, @function
	.type intel2_jmp, @function
	.type intel2_sar, @function
	.type intel2_shr, @function
	.type intel2_sal, @function
	.type intel2_shl, @function
	.type intel2_ror, @function
	.type intel2_rol, @function
	.type intel2_bswap, @function
	.type intel2_isdigit, @function
	.type intel2_isbinary, @function
	.type intel2_isalpha, @function
	.type intel2_isalnum, @function
	#.type intel2_isascii, @function
	.type intel2_ispunct, @function
	.type intel2_isupper,@function
	.type intel2_islower,@function
	.type intel2_toupper,@function
	.type intel2_tolower,@function


intel2_true:
# int_true(void)
	push %ebp
	mov  %ebp, %esp
	mov %eax, 1
	pop %ebp
	ret

intel2_false:
# int false(void)
	push %ebp
	mov %ebp, %esp
	mov %eax, 0
	pop %ebp
	ret

intel2_self:
# void * self(void *)
	push %ebp
	mov %ebp, %esp
	mov %eax, 8[%ebp]
	pop %ebp
	ret

intel2_strlen:
# size_t strlen(const char *)
	push %ebp
	mov %ebp, %esp

	push %ecx
	push %edx

	mov %edx, 8[%ebp]
	mov %ecx, 0
	cmp %edx, 0
		jz out_strlen
	redo_strlen:
		movb %al, [%edx]
		inc %edx
		cmpb %al, 0
		je out_strlen
		inc %ecx
		jmp redo_strlen
	out_strlen:
	mov %eax, %ecx

	pop %edx
	pop %ecx

	pop %ebp
	ret



intel2_strlen2:
# repne/scasb variation
        push %ebp
        mov %ebp, %esp

	push %ecx
        push %edi

        cld
        mov %edi, 8[%ebp]
	xor %eax, %eax
	xor %ecx, %ecx
	not %ecx
        repne scasb
	not %ecx
	mov %eax, %ecx

        pop %edi
	pop %ecx

        leave
        ret




intel2_strcmp:
# int intel2_strcmp(const char *, const char *)
        push %ebp
        mov %ebp, %esp

	push %esi
	push %edi
	push %ecx

        mov %edi, 8[%ebp]
        mov %esi, 12[%ebp]

        .Lintel2_strcmp_strlen:
                mov %eax, 8[%ebp]
                xor %ecx, %ecx
                jmp .Lintel2_strcmp_strlen_end
                .Lintel2_strcmp_strlen_begin:
                inc %eax
                inc %ecx
                .Lintel2_strcmp_strlen_end:
                cmpb [%eax], 0
                jne .Lintel2_strcmp_strlen_begin

        xor %eax, %eax

        cld
        repe cmpsb
        jne .Lintel2_strcmp_noteq
        jmp .Lintel2_strcmp_eq

        .Lintel2_strcmp_noteq:

        mov %eax, -1
        jg .Lintel2_strcmp_eq
        neg %eax

        .Lintel2_strcmp_eq:

	pop %ecx
	pop %edi
	pop %esi

        leave
        ret




intel2_strncmp:
# int intel2_strncmp(const char *, const char *, size_t)
        push %ebp
        mov %ebp, %esp

	push %esi
	push %edi
	push %ecx

        mov %edi, 8[%ebp]
        mov %esi, 12[%ebp]
	mov %ecx, 16[%ebp]

        xor %eax, %eax

        cld
        repe cmpsb
        jne .Lintel2_strncmp_noteq
        jmp .Lintel2_strncmp_eq

        .Lintel2_strncmp_noteq:

        mov %eax, -1
        jg .Lintel2_strncmp_eq
        neg %eax

        .Lintel2_strncmp_eq:

	pop %ecx
	pop %edi
	pop %esi

        leave
        ret




intel2_strchr:
# char * intel2_strchr(char *, int)
        push %ebp
        mov %ebp, %esp

        push %edi

        mov %eax, 12[%ebp]
        mov %edi, 8[%ebp]

        jmp .Lstrchr_loop_end
        .Lstrchr_loop_begin:
        inc %edi
        .Lstrchr_loop_end:
        cmpb [%edi], 0
        je .Lstrchr_zero
        cmpb [%edi], %al
        je .Lstrchr_done
        jmp .Lstrchr_loop_begin

        .Lstrchr_zero:
        xor %edi, %edi

        .Lstrchr_done:
        mov %eax, %edi

        pop %edi

        leave
        ret





intel2_strrchr:
# char * intel2_strrchr(char *, int)
        push %ebp
        mov %ebp, %esp

        push %ebx
	push %ecx
        push %edi

        mov %edi, 8[%ebp]

        push %edi
        call intel2_strlen2
        add %esp, 4

        cmp %eax, 0
        jle .Lintel2_strrchr_done

        add %edi, %eax
        mov %ecx, %eax
        mov %eax, 12[%ebp]
        std
        repne scasb
        je .Lintel2_strrchr_match
        xor %eax, %eax
        jmp .Lintel2_strrchr_done

        .Lintel2_strrchr_match:
	inc %edi
        mov %eax, %edi

        .Lintel2_strrchr_done:

        pop %edi
	pop %ecx
        pop %ebx
        leave
        ret



intel2_strcpy:
	push %ebp
	mov %ebp, %esp

	push %esi
	push %edi

	mov %edi, 8[%ebp]
	mov %esi, 12[%ebp]

	.Lintel2_strcpy_start:
	movb %al, [%esi]
	movb [%edi], %al
	cmp %al, 0
	je .Lintel2_strcpy_done
	inc %edi
	inc %esi
	jmp .Lintel2_strcpy_start

	.Lintel2_strcpy_done:
	mov %eax, 8[%ebp]

	pop %edi
	pop %esi
	leave
	ret


intel2_bzero:
# void bzero(void *, int)
	push %ebp
	mov %ebp, %esp

	push %ecx
	push %edx

	mov %edx, 8[%ebp]
	xor %ecx,%ecx

	cmp %edx, 0
	je out_bzero

	redo_bzero:
		movb [%edx], 0
		inc %edx
		decd 12[%ebp]
		movb %cl, [%edx]
		cmpb %cl, 0
		jne redo_bzero
	out_bzero:
	xor %eax, %eax

	pop %edx
	pop %ecx

	pop %ebp
	ret	


intel2_memset:
# void * memset(void *, int, size_t)
	push %ebp
	mov %ebp, %esp

	push %ecx
	push %edx
	
	mov %edx, 8[%ebp]

	cmp %edx, 0
	je out_memset

	xor %ecx, %ecx

	redo_memset:
		movb %cl, 12[%ebp]
		movb [%edx], %cl
		inc %edx
		decd 16[%ebp]
		cmpd 16[%ebp], 0
		jne redo_memset

	out_memset:
	mov %eax, 8[ebp]

	pop %edx
	pop %ecx

	pop %ebp
	ret


intel2_mul:
# int mul(int, int)
	push %ebp
	mov %ebp, %esp
	push %ebx

	mov %eax, 8[%ebp]
	mov %ebx, 12[%ebp]
	mul %ebx

	pop %ebx
	pop %ebp
	ret

intel2_imul:
# int imul(int, int)
	push %ebp
	mov %ebp, %esp
	
	push %ebx
	mov %eax, 8[%ebp]
	mov %ebx, 12[%ebp]
	imul %ebx

	pop %ebx
	leave
	ret

intel2_sub:
# int sub(int, int)
	push %ebp
	mov %ebp, %esp

	push %edx

	mov %eax, 8[%ebp]
	mov %edx, 12[%ebp]
	sub %eax, %edx

	pop %edx

	leave
	ret	


intel2_add:
# int add(int, int)
	push %ebp
	mov %ebp, %esp

	push %edx

	mov %eax, 8[%ebp]
	mov %edx, 12[%ebp]
	add %eax, %edx

	pop %edx

	leave
	ret




intel2_div:
# int intel2_div(int, int)
	push %ebp
	mov %ebp, %esp

	push %edx

	mov %eax, 8[%ebp]
	xor %edx, %edx
	divd 12[%ebp]

	pop %edx

	leave
	ret


intel2_idiv:
# int intel2_idiv(int, int)
	push %ebp
	mov %ebp, %esp

	push %edx

	mov %eax, 8[%ebp]
	xor %edx, %edx
	idivd 12[%ebp]
	
	pop %edx

	leave
	ret


intel2_mod:
# int intel2_mod(int, int)
	push %ebp
	mov %ebp, %esp

	push %edx

	mov %eax, 8[%ebp]
	xor %edx, %edx
	divd 12[%ebp]
	mov %edx, %edx
	
	pop %edx

	leave
	ret


intel2_imod:
# int intel2_imod(int, int)
	push %ebp
	mov %ebp, %esp

	push %edx
	
	mov %eax, 8[%ebp]
	xor %edx,%edx
	idivd 12[%ebp]
	mov %eax, %edx
	
	leave
	ret

intel2_neg:
# long intel2_neg(long)
	push %ebp
	mov %esp, %ebp

	mov %eax, 8[%ebp]
	neg %eax

	pop %ebp
	ret	

intel2_eflags:
# unsigned int intel2_eflags(void)
	push %ebp
	mov %ebp, %esp

	pushf
	mov %eax, 0[%ebp]
	popf

	leave
	ret



intel2_and:
# int intel2_and(int,int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	and %eax, 12[%ebp]

	leave
	ret


intel2_or:
# int intel2_or(int,int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	or %eax, 12[%ebp]

	leave
	ret


intel2_xor:
# int intel2_xor(int, int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	xor %eax, 12[%ebp]

	leave
	ret


intel2_not:
# int intel2_not(int)
	push %ebp
	mov %ebp, %esp

	notd 8[%ebp]
	mov %eax, 8[%ebp]

	leave
	ret



intel2_jmp:
# int intel2_jmp(void *)
	push %ebp
	mov %ebp, %esp

	jmp 8[%ebp]

	leave
	ret


intel2_sar:
# int intel2_sar(int, int)
	push %ebp
	mov %ebp, %esp
	push %ecx

	mov %eax, 8[%ebp]
	mov %ecx, 12[%ebp]
	sar %eax, %cl

	pop %ecx
	leave
	ret


intel2_shr:
# int intel2_shr(int, int)
	push %ebp
	mov %ebp, %esp
	push %ecx

	mov %eax, 8[%ebp]
	mov %ecx, 12[%ebp]

	shr %eax, %cl

	pop %ecx
	leave
	ret


intel2_sal:
# int intel2_sal(int, int)
	push %ebp
	mov %ebp, %esp
	push %ecx

	mov %eax, 8[%ebp]
	mov %ecx, 12[%ebp]
	sal %eax, %cl

	pop %ecx
	leave
	ret


intel2_shl:
# int intel2_shl(int, int)
	push %ebp
	mov %ebp, %esp
	push %ecx

	mov %eax, 8[%ebp]
	mov %ecx, 12[%ebp]
	shl %eax, %cl

	pop %ecx
	leave
	ret

intel2_ror:
# int intel2_ror(int, int)
	push %ebp
	mov %ebp, %esp
	push %ecx

	mov %eax, 8[%ebp]
	mov %ecx, 12[%ebp]
	ror %eax, %cl

	pop %ecx
	leave
	ret


intel2_rol:
# int intel2_rol(int, int)
	push %ebp
	mov %ebp, %esp
	push %ecx

	mov %eax, 8[%ebp]
	mov %ecx, 12[%ebp]
	rol %eax, %cl

	pop %ecx
	leave
	ret

intel2_bswap:
# int intel2_bswap(int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	bswap %eax

	leave
	ret


intel2_isdigit:
# int intel2_isdigit(int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	cmp %eax, 48
	jl .Lisdigit_mismatch

	cmp %eax, 57
	jg .Lisdigit_mismatch

	mov %eax, 1
	jmp .Lisdigit_success

	.Lisdigit_mismatch:
	xor %eax, %eax

	.Lisdigit_success:

	leave
	ret

intel2_isbinary:
# int intel2_isbinary(int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	cmp %eax, 48
	je .Lintel2_isdigit_success

	cmp %eax, 49
	je .Lintel2_isdigit_success

	.Lintel2_isdigit_fail:
	xor %eax, %eax
	jmp .Lintel2_isdigit_done
	
	.Lintel2_isdigit_success:
	mov %eax, 1

	.Lintel2_isdigit_done:

	leave
	ret

intel2_isalpha:
# int intel2_isalpha(int)
	push %ebp
	mov %ebp, %esp

	mov %eax, 8[%ebp]
	cmp %eax, 65
	jl .Lintel2_isalpha_fail

	cmp %eax, 90
	jle .Lintel2_isalpha_success

	cmp %eax, 97
	jl .Lintel2_isalpha_fail

	cmp %eax, 122
	jle .Lintel2_isalpha_success

	jmp .Lintel2_isalpha_fail

	.Lintel2_isalpha_success:
	mov %eax, 1
	jmp .Lintel2_isalpha_done

	.Lintel2_isalpha_fail:
	xor %eax, %eax

	.Lintel2_isalpha_done:
	leave
	ret


intel2_isalnum:
# int intel2_isalnum(int)
	push %ebp
	mov %ebp, %esp

	push 8[%ebp]
	call intel2_isalpha
	add %esp, 4

	cmp %eax, 1
	jge .Lintel2_isalnum_done

	push 8[%ebp]
	call intel2_isdigit
	add %esp, 4

	.Lintel2_isalnum_done:
	leave
	ret



intel2_isupper:
# int intel2_isupper(int)
        push %ebp
        mov %ebp, %esp
        push %ebx

        xor %eax,%eax
        mov %ebx, 8[%ebp]
        cmp %ebx, 65
        jae .L2_intel2_isupper
        mov %eax, 0
        jmp .L3_intel2_isupper

.L2_intel2_isupper:
        cmp %ebx, 90
        setbe %bl
        mov %eax, %ebx

.L3_intel2_isupper:
        pop %ebx
        leave
        ret





intel2_islower:
# int intel2_islower(int)
        push %ebp
        mov %ebp, %esp
        push %ebx

        xor %eax,%eax
        mov %ebx, 8[%ebp]
        cmp %ebx, 97
        jae .L2_intel2_islower
        mov %eax, 0
        jmp .L3_intel2_islower

.L2_intel2_islower:
        cmp %ebx, 122
        setbe %bl
        mov %eax, %ebx

.L3_intel2_islower:
        pop %ebx
        leave
        ret


intel2_toupper:
# int intel2_toupper(int)
	push %ebp
	mov %ebp, %esp

	push 8[%ebp]
	call intel2_islower
	add %esp, 4

	cmp %eax, 0
	mov %eax, 8[%ebp]
	jle out_toupper

	sub %eax, 32
	
	out_toupper:
	leave
	ret


intel2_tolower:
# int intel2_tolower(int)
        push %ebp
        mov %ebp, %esp

        push 8[%ebp]
        call intel2_isupper
        add %esp, 4

        cmp %eax, 0
        mov %eax, 8[%ebp]
        jle out_tolower

        add %eax, 32

        out_tolower:
        leave
        ret

