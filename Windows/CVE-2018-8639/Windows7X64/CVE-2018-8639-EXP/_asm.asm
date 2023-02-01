_TEXT SEGMENT

NtGdiSetLinkedUFIs PROC
	mov		R10,RCX
	mov		EAX,1266h
	syscall
	ret
NtGdiSetLinkedUFIs ENDP

NtUserUnregisterClass PROC
	mov		R10,RCX
	mov		EAX,10BBh
	syscall
	ret
NtUserUnregisterClass ENDP

END
