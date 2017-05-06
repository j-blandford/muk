get_eip: mov eax, [esp]
         ret

global set_context
set_context:
	mov ecx, [esp + 4]      ;ebx = new esp

    ;Load next task's ESP
    mov esp, [ecx]

	pop edi
    pop esi
    pop ebx
    pop ebp

    sti

	;jmp back to the task's eip
	ret

global switch_context
switch_context:
	mov eax, [esp + 4*1]      ;eax = last esp
	mov ecx, [esp + 4*2]      ;ecx = new esp

	push ebp
    push ebx
    push esi
    push edi

	; Save previous task's ESP
    mov [eax], esp
    ;Load next task's ESP
    mov esp, [ecx]

	pop edi
    pop esi
    pop ebx
    pop ebp

	sti

	;jmp back to the task's eip
	ret