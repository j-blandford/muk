; global save_registers
; save_registers:
; 	; first, we push everything to the stack
; 	pushad			; push all registers (double)
; 	pushfd			; push all flags (double)

; 	call get_eip	
; 	push eax		; push eip to stack

; 	mov cr3, eax 	
; 	push eax		; push CR3 to stack (currently not used in the scheduler...)

; 	; now we have pushed all of the vars to the stack, we can now save them!
; 	mov eax, [esp + 48]   ; the pointer to the thread registers

; 	; eax now holds the pointer to the first "register" uint32 (edi)
; 	pop ebx
; 	mov ebx, [eax+56] ; cr3
; 	pop ebx
; 	mov ebx, [eax+44] ; eip
; 	pop ebx
; 	mov ebx, [eax+52] ; eflags

; 	pop ebx
; 	mov ebx, [eax+4] ; edi
; 	pop ebx
; 	mov ebx, [eax+8] ; esi
; 	pop ebx
; 	mov ebx, [eax+12] ; ebp
; 	pop ebx
; 	mov ebx, [eax+16] ; esp

; 	pop ebx
; 	mov ebx, [eax+20] ; ebx
; 	pop ebx
; 	mov ebx, [eax+24] ; edx
; 	pop ebx
; 	mov ebx, [eax+28] ; ecx
; 	pop ebx
; 	mov ebx, [eax+32] ; eax

; 	; fix me: 'eax' and 'ebx' get trashed

; 	ret

global switch_registers
switch_registers:

	mov eax, [esp + 4]  ; the pointer to the new thread registers

	xchg bx, bx

	mov edi, [eax+16]
	mov esi, [eax+20]
	mov ebp, [eax+24]
	mov esp, [eax+28]

	mov ebx, [eax+32]
	mov edx, [eax+36]
	mov ecx, [eax+40]

	xchg bx, bx

	; ; ; ; mov [eax+56], cr3
	; ; push eax
	; ; mov eax, [eax+52] ; eflags
	; ; push eax
	; ; popfd
	; ; pop eax

	;mov eax, [eax+28]  ; we need to store this somewhere else for the eip (below)
	jmp [eax+56] ; jump to next thread's eip

	ret


get_eip: mov eax, [esp]
         ret

global set_stack_ptr
set_stack_ptr:
	xchg bx, bx
	mov eax, [esp + 4]
	mov ebx, esp
	mov esp, eax

	xchg bx, bx

	mov ecx, 16
loop_stack:
	mov edx, [ebx + 4*ecx]
	push edx
	dec ecx
loop_bottom:
	cmp ecx, 1
	jne loop_stack

	ret