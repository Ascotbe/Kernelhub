public _DoSyscall
.code
_DoSyscall proc

	mov		r10, rcx
	mov		rax, [rsp + 40]

	add		rsp, 16
	syscall
	sub		rsp, 16

	ret

_DoSyscall endp
end