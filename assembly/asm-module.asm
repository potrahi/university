	bits 64

	section .data
	extern g_val0, g_val1, g_pole_long1
	extern g_pole_long2, g_pozdrav
	extern g_pole_bajty, g_4bajty
	extern g_long, g_lsb, g_msb
	extern g_heslo, g_vypis
	extern g_long_n, g_long_login, g_pocet
	extern g_dnes, pocet
	extern g_cislo, g_cislo_binarne
	extern g_int_pole, count_4x0, count_4x1
	extern l_cisla_bitu

	section .text

	global zdrcni
zdrcni:
	; zdrcni(char *nesmysl)
	;				rdi
	enter 0,0

	mov rax, qword 0	; i = 0
	mov rcx, qword 1	; k = 1

.while_first_start:
	mov dl, byte [ rdi + rax ]
	cmp dl, byte 0
	je .while_first_end


	.while_second_start:
		mov sil, byte [ rdi + rcx ]
		cmp dl, sil
		jne .while_second_end
		inc rcx
		jmp .while_second_start
	.while_second_end:

	inc rax
	mov byte [ rdi + rax ], sil

	jmp .while_first_start
.while_first_end:
	leave
	ret

	global int2bit
int2bit:
	; int2bit(char *string, long *num)
	;                rdi         rsi
	enter 0,0

	mov r11, qword 0 ; count = 0
	mov r10, qword 0 ; cislo = 0
	mov r9, qword 0 ; znak = 0
	mov r8, qword 0	; i = 0
.while_start:
	cmp byte [ rdi + r8 ], byte 0
	je .while_end

	cmp byte [ rdi + r8 ], byte '-'
	jne ._if
	mov r9, qword 1	; znak = 1
	inc r11
	cmp r11, qword 1
	jg .error
	jmp .increment
._if:

	cmp byte [ rdi + r8 ], byte '+'
	jne ._else_if
	mov r9, qword 0	; znak = 0
	inc r11
	cmp r11, qword 1
	jg .error
	jmp .increment
._else_if:

	cmp byte [ rdi + r8 ], byte '0'
	jb .error
	cmp byte [ rdi + r8 ], byte '9'
	ja .error

	mov rax, qword 10
	imul r10
	mov r10, rax
	movzx rcx, byte [ rdi + r8 ]
	sub rcx, qword 48
	add r10, rcx

.increment:
	inc r8
	jmp .while_start
.while_end:

	cmp r9, qword 1
	jne .next
	mov rax, r10
	sal rax, 1
	sub r10, rax
.next:
	jmp .return
.error:
	add r8, qword 1
	mov rax, r8
	jmp .end
.return:
	mov [ rsi ], r10
	mov rax, 0
.end:

	leave
	ret

	global kolik_jednicek
kolik_jednicek:
	; kolik_jednicek(int *pole, int *polejednicek, int delka)
	;                      rdi            rsi		    rdx
	enter 0,0

	movsx rdx, edx

	mov ecx, dword 0		; i = 0
.start_first_for:
	cmp ecx, edx
	jge .end_firs_for		; i < delka

	mov r8d, dword 1			; elone_mask = 1
	mov eax, dword 0		; count = 0
	movsx r10, dword [ rdi + rcx * 4 ]

		mov r9d, dword 0
	.start_second_for:
		cmp r9d, dword 32
		je .end_secosn_for

		test r10d, r8d
		jz .zero

		inc eax

	.zero:

		shl r8d, 1



		inc r9d
		jmp .start_second_for
	.end_secosn_for:

	mov dword [ rsi + rcx * 4 ], eax

	inc rcx
	jmp .start_first_for
.end_firs_for:
	leave
	ret

	global najdi_minmax
najdi_minmax:
	; najdi_minmax(int *pole, int delka, int volba)
	;                   rdi        rsi		  rdx
	enter 0,0

	movsx rcx, dword [ rdi + 0 * 4 ]		; minmax = pole[0]
	movsx rdx, edx
	movsx rsi, esi			; delka - 1

	mov r8, qword 0

	cmp rdx, 0				; if(volba < 0)
	jle .ifelse

.first_for_start_1:
	cmp r8, rsi					; i < delka
	jge .first_for_end_1

	mov r9, rsi
	sub r9, qword 1
	.second_for_start_1:
		cmp r9, r8				; delka - 1 != i
		jz .second_for_end_1

		movsx r10, dword [ rdi + r8 * 4 ]	; pole[i]
		movsx r11, dword [ rdi + r9 * 4 ]	; pole[k]

		cmp r10, r11		; pole[i] < pole[k]
		jg .mensi

		mov ecx, r11d
		mov dword [ rdi + r9 * 4 ], r10d
		mov dword [ rdi + r8 * 4 ], ecx

	.mensi:

		dec r9
		jmp .second_for_start_1
	.second_for_end_1:
	inc r8
	jmp .first_for_start_1
.first_for_end_1:

.ifelse:

.first_for_start_2:
	cmp r8, rsi					; i < delka
	jge .first_for_end_2

	mov r9, rsi
	sub r9, qword 1

	.secont_for_start_2:
		cmp r9, r8				; delka - 1 != i
		jz .second_for_end_2

		movsx r10, dword [ rdi + r8 * 4 ]	; pole[i]
		movsx r11, dword [ rdi + r9 * 4 ]	; pole[k]

		cmp r10, r11		; pole[i] > pole[k]
		jl .vetsi

		mov ecx, r11d		; minmax = pole[k]
		mov dword [ rdi + r9 * 4 ], r10d		; pole[k] = pole[i]
		mov dword [ rdi + r8 * 4 ], ecx		; pole[i] = minmax

	.vetsi:

		dec r9
		jmp .secont_for_start_2
	.second_for_end_2:
	inc r8
	jmp .first_for_start_2
.first_for_end_2:


	leave
	ret

	global nasobky_prvocisel_z
nasobky_prvocisel_z:
	; nasobky_prvocisel( int *t_pole, int t_start, int t_koef )
	;                     		rdi         rsi			rdx
	enter 0,0

	mov rcx, qword 0

	mov r8d, edx

.while_start:

	mov dword [ rdi + rcx * 4 ], esi
	mov eax, r8d
	mul dword [ rdi + rcx * 4 ]

	jc .while_end
	jo .while_end

	mov dword [ rdi + rcx * 4 ], eax
	mov esi, eax

	inc ecx
	jmp .while_start
.while_end:

	mov eax, ecx

	leave
	ret

	global nasobky_prvocisel_s
nasobky_prvocisel_s:
	; nasobky_prvocisel( int *t_pole, int t_start, int t_koef )
	;                     		rdi         rsi			rdx
	enter 0,0

	movsx rsi, esi
	movsx rdx, edx

	mov r8d, dword 0
	movsx r8, r8d
	mov r8, rdx

	mov rcx, qword 0

	movsx rax, eax


.while_start:

	mov qword [ rdi + rcx * 4 ], rsi
	mov rax, r8
	imul dword [ rdi + rcx * 4 ]

	jc .while_end
	jo .while_end

	mov qword [ rdi + rcx * 4 ], rax
	mov rsi, rax

	inc rcx
	jmp .while_start
.while_end:

	mov rax, rcx

	leave
	ret

	global povrch_trubky
povrch_trubky:
	; povrch_trubky( int t_R, int t_delka, int *t_vysledek )
	;                    rdi        rsi			  rdx
	enter 0,0

	mov ecx, dword 3141
	mov r8d, dword 1000

	mov r10, rdx

	cdq
	mov eax, edi
	imul esi			; t_R * t_delka
	imul ecx			; * 3141
	sal eax, 1		; * 2
	mov r9d, eax	; l_povrch = 3141 * 2 * t_R * t_delka
	idiv r8d			; l_povrch / 1000
	mov [ r10 + 0 * 4 ], eax
	mov eax, r9d
	idiv r8d
	imul r8d
	mov r11d, r9d
	sub r11d, eax
	mov [ r10 + 1 * 4 ], r11d

	leave
	ret

	global hledej_rozptyl
hledej_rozptyl:
	; hledej_rozptyl(int *t_pole, int t_N )
	;                      rdi        rsi
	enter 0,0

	push r12

	movsx rsi, esi		; t_N


	mov edx, dword 0
	movsx rdx, edx		; l_prumer_pole = 0

	mov ecx, dword 0
	movsx rcx, ecx		; l_odchylka = 0

	mov r8d, dword 0
	movsx r8, r8d		; l_rozptyl = 0

	mov r9d, dword 0
	movsx r9, r9d		; max_rozptyl = 0

	mov r10d, dword 0
	movsx r10, r10d		; cislo = 0

	mov r12, dword 0		; i = 0
.firs_for_start:
	cmp r12, rsi		; i < t_N
	jae .first_for_end							; ??????

	movsx r11, dword [ rdi + r12 * 4 ]

	add rdx, r11

	inc r12
	jmp .firs_for_start
.first_for_end:
	mov rax, rdx
	cqo
	idiv rsi

	mov r12, dword 0	; k = 0
.second_for_start:
	cmp r12, rsi		; k < t_N
	jae .second_for_end							; ???????

	movsx rcx, dword [ rdi + r12 * 4 ]	; l_odchylka = t_pole[i]
	cmp rcx, rdx						; l_odchylka -= l_prumer_pole
	mov r8, rcx 						; l_rozptyl = l_odchylka
	mov rax, r8
	imul rcx							; l_rozptyl = l_rozptyl * l_odchylka

	cmp rax, r9							; l_rozptyl > max_rozptyl
	jbe .less

	mov r9, rax							; max_rozptyl = l_rozptyl
	movsx r10, dword [ rdi + r12 * 4 ]	; cislo = t_pole[i]

.less:

	inc r12
	jmp .second_for_start
.second_for_end:

	mov rax, r10

	pop r12

	leave
	ret

	global prvocisla
prvocisla:
	; prvocisla( long *t_pole, int t_N, long *t_prvocisla )
	;                   rdi        rsi			  rdx
	enter 0,0

	push r12

	mov rcx, qword 0 ; index = 0
	movsx rsi, esi
	xor r9, r9
	mov r8, qword 0  ; i = 0
.start_firs_for:
	cmp r8, rsi
	jge .end_firs_for

	mov r9, qword [ rdi + r8 * 8 ]
	sar r9, 1				   ; m = t_pole[i]/2
	mov r10, 0				   ; flag = 0
	mov r12, qword [ rdi + r8 * 8]

		xor r11, r11
		mov r11, qword 2					; k = 2
	.star_second_for:
		cmp r11, r9
		jg .end_second_for
		xor rax, rax

		mov rax, r12	; t_pole[i]
		cqo
		idiv r11		; /k
		imul r11		; t_pole[i]*k

		cmp r12, rax
		jnz .modulo_not_0
		mov r10, qword 1
		jmp .end_second_for

	.modulo_not_0:
		inc r11
		jmp .star_second_for
	.end_second_for:

	cmp r10, qword 0
	jne .flag_not_0
	inc rcx
.flag_not_0:
	inc r8
	jmp .start_firs_for
.end_firs_for:
	xor rax,rax
	mov rax, rcx

	pop r12

	leave
	ret

	global overeni
overeni:
	; overeni( char *retezec )
	;				  rdi
	enter 0,0

	mov eax, dword 0 ; result = 0
	mov edx, dword 0 ; points_count = 0
	mov rsi, dword 0 ; i = 0
.start_while:
	movzx r10, byte [ rdi + rsi ]
	cmp r10, byte 0
	je .end_while

	cmp [ rdi + 0 ], byte '+'
	je .end_overeni

	cmp [ rdi + rsi ], byte '.'
	jne .not_equal
	inc edx
	cmp edx, dword 1
	jbe .less
	jmp .end_overeni

.less:
.not_equal:

	inc rsi
	jmp .start_while
.end_while:

	cmp rsi, qword 1
	jne .next
	cmp edx, dword 1
	jbe .end_overeni
.next:
	mov eax, dword 1
.end_overeni:

	leave
	ret

	global prenes_pismena
prenes_pismena:
	; prenes_pismena( char *t_pismena, char *t_text, char zastupny_znak )
	;                          rdi            rsi				rdx
	enter 0,0

	xor rax, rax
	movzx rdx, dl

	mov r10, 0
.back:
	cmp [ rsi + r10 ], byte 0
	je .found_0
	; (l_text[i] >= 'a' && l_text[i] <= 'z') || (l_text[i] >= 'A' && l_text[i] <= 'Z')
	movzx r11, byte [ rsi + r10 ]
	cmp r11, byte 'a'
	jb .next
	cmp r11, byte 'z'
	ja .next
	mov [ rdi + r10 ], r11
	inc eax
	jmp .next_next
.next:
	cmp r11, byte 'A'
	jb .next_next_next
	cmp r11, byte 'Z'
	ja .next_next_next
	mov [ rdi + r10 ], r11
	inc eax
	jmp .next_next
.next_next_next:
	mov [ rdi + r10 ], rdx
.next_next:
	inc r10
	jmp .back
.found_0:

	leave
	ret

	global kde_je_minmax
kde_je_minmax:
	enter 0,0

	; kde_je_minmax( int *t_cela_cisla, int t_N, int *t_pozice_nejv_nejm )
	;                            rdi        rsi				rdx

	movsx rdi, edi						; array[]
	movsx rsi, esi						; size
	movsx rdx, edx						; position[2]
	movsx r10, dword [ rdi + 0 * 4 ]	; tmp
	mov rax, qword 0							; (int i = 0;
.back:
	cmp rax, rsi						; i < size;
	jnl .end_for

	movsx r11, dword [ rdi + rax * 4 ]
	cmp r10, r11						; tmp > r11
	jge .greate
	mov [ rdx + 0 * 4 ], rax
	jmp .next
.greate:
	mov [ rdx + 1 * 4 ], rax
.next:

	inc rax								; i++)
	jmp .back
.end_for:

	leave
	ret

	global bitova_maska
bitova_maska:
	enter 0,0

	; long bitova_maska( char *t_cisla_bitů, int t_LEN )
	;                            rdi              rsi

	xor rax, rax
	movsx rsi, esi

	mov r10, 0
.back:
	cmp r10, rsi
	jnl .end_for

	mov cl, byte [ rdi + r10 ]
	mov r11, 1
	shl r11, cl
	or rax, r11

	inc r10
	jmp .back
.end_for:

	leave
	ret

	global count
count:
	enter 0,0

	xor rax, rax

	mov eax, 0
.back_1_for:
	cmp eax, dword 256
	jnl .end_1_for

	xor rdi, rdi
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11

	mov r8d, 1 	; mask
	mov r9d, 0			; tmp1
	mov r10d, 0			; tmp0
	mov edi, dword 0
	movsx r11, dword [ g_int_pole + eax * 4 ]

	.back_2_for:
		cmp edi, dword 4
		jnl .end_2_for

		test r11d, r8d
		jz .null
		inc r9d
		jmp .next

	.null:
		inc r10d

	.next:
		cmp r9d, dword 4
		jnz .not_count1
		inc dword [ count_4x1 ]

	.not_count1:
		cmp r10d, dword 4
		jnz .not_count0
		inc dword [ count_4x0 ]

	.not_count0:
		shl r8d, 1
		inc edi
		jmp .back_2_for

	.end_2_for:

	cmp r11, 0					; zeptat se
	jz .not_this
	cmp r9d, dword 4
	jg .not_this
	cmp r8d, dword 4
	jl .not_this
	not r11
	or r11, qword 1

.not_this:

	inc eax
	jmp .back_1_for
.end_1_for:

	leave
	ret

	global do_dvojkove
do_dvojkove:
	enter 0,0

	mov r8d, dword [ g_cislo ]	; cislo
	mov r9d, dword 1			; maska
	mov r10d, dword 31			; delka_array
	mov r11d, dword 0		; for (i = 0;
.back:
	cmp r11d, 32			; i < 32;
	jnl .end_for

	test r8d, r9d							; if ( cislo & maska )
	jz .its_0
	mov [ g_cislo_binarne + r10d ], byte '1' 		; g_cislo_binarne[ delka_array ] = 1;
	jmp .next
.its_0:										; else
	mov [ g_cislo_binarne + r10d ], byte '0' 		; g_cislo_binarne[ delka_array ] = 0;
.next:
	shl r9d, 1								; mask = mask << 1;
	dec r10d								; delka_array--;
	inc r11d				; i++)
	jmp .back
.end_for:

	leave
	ret

	global kolik_cislic
kolik_cislic:
	enter 0,0

	mov dil, [ pocet ]
	mov rax, 0
.back:
	cmp [ g_dnes + rax ], byte 0
	je .found_0

	cmp [ g_dnes + rax ], byte '0'
	jb .not_this
	cmp [ g_dnes + rax ], byte '9'
	ja .not_this

	inc dil

.not_this:
	inc rax
	jmp .back
.found_0:
	mov [ pocet ], dil

	leave
	ret

	global spocitej
spocitej:
	enter 0,0
	mov rax, [ g_long_n ]
	mov rdi, 1			; maska

	mov rsi, 0
.back:
	cmp rsi, 64
	jnl .end_for

	test rax, rdi
	jz .no
	inc byte [ g_pocet ]
.no:
	shl rdi, 1
	inc rsi
	jmp .back
.end_for:

	leave
	ret

	global preved
preved:
	enter 0,0

	mov r8, [ g_heslo ]
	mov [ g_vypis ], r8

	leave
	ret

	global rozloz
rozloz:
	enter 0,0

	mov rdi, [ g_long ]
	mov [ g_lsb ], rdi

	leave
	ret

	global sloz
sloz:
	enter 0,0

	mov dil, [ g_pole_bajty + 0 ]
	mov [ g_4bajty + 3 ], dil
	mov dil, [ g_pole_bajty + 1 ]
	mov [ g_4bajty + 2 ], dil
	mov dil, [ g_pole_bajty + 2 ]
	mov [ g_4bajty + 1 ], dil
	mov dil, [ g_pole_bajty + 3 ]
	mov [ g_4bajty ], dil

	leave
	ret

	global posun
posun:
	enter 0,0

	mov dil, byte [ g_pozdrav ]
	mov sil, byte [ g_pozdrav + 1 ]
	mov [ g_pozdrav ], sil
	mov sil, byte [ g_pozdrav + 2 ]
	mov [ g_pozdrav + 1 ], sil
	mov sil, byte [ g_pozdrav + 3 ]
	mov [ g_pozdrav + 2 ], sil
	mov sil, byte [ g_pozdrav + 4 ]
	mov [ g_pozdrav + 3 ], sil
	mov sil, byte [ g_pozdrav + 5 ]
	mov [ g_pozdrav + 4 ], sil
	mov [ g_pozdrav + 5 ], dil

	leave
	ret

	global presun_bz
presun_bz:
	enter 0,0

	mov r8, [ g_pole_long1 + 0 * 8 ]
	mov [g_pole_long2 + 0 * 8 ], r8
	mov r8, [ g_pole_long1 + 1 * 8 ]
	mov [g_pole_long2 + 1 * 8 ], r8
	mov r8, [ g_pole_long1 + 2 * 8 ]
	mov [g_pole_long2 + 2 * 8 ], r8
	mov r8, [ g_pole_long1 + 3 * 8 ]
	mov [g_pole_long2 + 3 * 8 ], r8
	mov r8, [ g_pole_long1 + 4 * 8 ]
	mov [g_pole_long2 + 4 * 8 ], r8
	mov r8, [ g_pole_long1 + 5 * 8 ]
	mov [g_pole_long2 + 5 * 8 ], r8

	leave
	ret

	global presun
presun:
	enter 0,0

	movsx r8, dword [ g_val0 ]
	movsx r9, dword [ g_val1 ]
	mov [ g_pole_long1 + 0 * 8 ], r8
	mov [ g_pole_long1 + 5 * 8 ], r9

	leave
	ret
