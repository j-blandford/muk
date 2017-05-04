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
	;cli

    mov eax, [esp + 4*1]      ;eax = address to store this task's ESP
    mov ecx, [esp + 4*2]      ;ecx = address to get next task's ESP

	; ;xchg bx, bx

    ; ;Save general purpose registers for previous task
    ; ; Note: EAX, ECX, EDX and "return EIP" are saved by caller, so don't need to be saved again

    ; push ebx
    ; push esi
    ; push edi
    ; push ebp

    ; ;Save previous task's ESP
    ; mov [eax],esp

    ; ;Load next task's ESP
    ; mov esp,[ecx]

    ; ;Load general purpose registers for next task

    pop ebp
    pop edi
    pop esi
    pop ebx

	; the "return EIP" should be the only thing now on the stack that ret needs

	; xchg bx, bx

	sti
	
    ;Return to next task's EIP
    ret

global set_esp
set_esp:
	cli

	mov eax, [esp + 4*1]      ;eax = last esp
	mov ecx, [esp + 4*2]      ;ebx = new esp

	; load the new stack pointer
	; Save previous task's ESP
    mov [eax],esp

    ;Load next task's ESP
    mov esp,[ecx]

	push dword [ebx]	; push new EIP for the ret statement below

	sti

	xchg bx, bx

	;jmp back to the task's eip
	ret