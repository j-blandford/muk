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
	cli

	mov eax, [esp + 4*1]      ;eax = address to migrate to new ESP
	mov ebx, [esp + 4*2]      ;ebx = address to new EIP
	mov ecx, esp			  ;ecx = address of old ESP

	; load the new stack pointer
	mov esp, [eax]

	push dword [ebx]	; push new EIP for the ret statement below

	sti

	;jmp back to the task's eip
	ret

global switch_to_task
switch_to_task:
	cli

    mov eax, [esp + 4*1]      ;eax = address to store this task's ESP
    mov ecx, [esp + 4*2]      ;ecx = address to get next task's ESP

	;xchg bx, bx

    ;Save general purpose registers for previous task
    ; Note: EAX, ECX, EDX and "return EIP" are saved by caller, so don't need to be saved again

    push ebx
    push esi
    push edi
    push ebp

    ;Save previous task's ESP
    mov [eax],esp

    ;Load next task's ESP
    mov esp,[ecx]

    ;Load general purpose registers for next task

    pop ebp
    pop edi
    pop esi
    pop ebx

	sti

	xchg bx, bx

    ;Return to next task's EIP
    ret