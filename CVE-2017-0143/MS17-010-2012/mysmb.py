# impacket SMB extension for MS17-010 exploit.
# this file contains only valid SMB packet format operation.
from impacket import smb, smbconnection
from impacket.dcerpc.v5 import transport
from struct import pack
import os
import random


def getNTStatus(self):
	return (self['ErrorCode'] << 16) | (self['_reserved'] << 8) | self['ErrorClass']
setattr(smb.NewSMBPacket, "getNTStatus", getNTStatus)

############# SMB_COM_TRANSACTION_SECONDARY (0x26)
class SMBTransactionSecondary_Parameters(smb.SMBCommand_Parameters):
	structure = (
		('TotalParameterCount','<H=0'),
		('TotalDataCount','<H'),
		('ParameterCount','<H=0'),
		('ParameterOffset','<H=0'),
		('ParameterDisplacement','<H=0'),
		('DataCount','<H'),
		('DataOffset','<H'),
		('DataDisplacement','<H=0'),
)

# Note: impacket-0.9.15 struct has no ParameterDisplacement
############# SMB_COM_TRANSACTION2_SECONDARY (0x33)
class SMBTransaction2Secondary_Parameters(smb.SMBCommand_Parameters):
	structure = (
		('TotalParameterCount','<H=0'),
		('TotalDataCount','<H'),
		('ParameterCount','<H=0'),
		('ParameterOffset','<H=0'),
		('ParameterDisplacement','<H=0'),
		('DataCount','<H'),
		('DataOffset','<H'),
		('DataDisplacement','<H=0'),
		('FID','<H=0'),
)

############# SMB_COM_NT_TRANSACTION_SECONDARY (0xA1)
class SMBNTTransactionSecondary_Parameters(smb.SMBCommand_Parameters):
	structure = (
		('Reserved1','3s=""'),
		('TotalParameterCount','<L'),
		('TotalDataCount','<L'),
		('ParameterCount','<L'),
		('ParameterOffset','<L'),
		('ParameterDisplacement','<L=0'),
		('DataCount','<L'),
		('DataOffset','<L'),
		('DataDisplacement','<L=0'),
		('Reserved2','<B=0'),
	)


def _put_trans_data(transCmd, parameters, data, noPad=False):
	# have to init offset before calling len()
	transCmd['Parameters']['ParameterOffset'] = 0
	transCmd['Parameters']['DataOffset'] = 0
	
	# SMB header: 32 bytes
	# WordCount: 1 bytes
	# ByteCount: 2 bytes
	# Note: Setup length is included when len(param) is called
	offset = 32 + 1 + len(transCmd['Parameters']) + 2
	
	transData = ''
	if len(parameters):
		padLen = 0 if noPad else (4 - offset % 4 ) % 4
		transCmd['Parameters']['ParameterOffset'] = offset + padLen
		transData = ('\x00' * padLen) + parameters
		offset += padLen + len(parameters)
	
	if len(data):
		padLen = 0 if noPad else (4 - offset % 4 ) % 4
		transCmd['Parameters']['DataOffset'] = offset + padLen
		transData += ('\x00' * padLen) + data
	
	transCmd['Data'] = transData
	

origin_NewSMBPacket_addCommand = getattr(smb.NewSMBPacket, "addCommand")
login_MaxBufferSize = 61440
def NewSMBPacket_addCommand_hook_login(self, command):
	# restore NewSMBPacket.addCommand
	setattr(smb.NewSMBPacket, "addCommand", origin_NewSMBPacket_addCommand)
	
	if isinstance(command['Parameters'], smb.SMBSessionSetupAndX_Extended_Parameters):
		command['Parameters']['MaxBufferSize'] = login_MaxBufferSize
	elif isinstance(command['Parameters'], smb.SMBSessionSetupAndX_Parameters):
		command['Parameters']['MaxBuffer'] = login_MaxBufferSize
	
	# call original one
	origin_NewSMBPacket_addCommand(self, command)

def _setup_login_packet_hook(maxBufferSize):
	# setup hook for next NewSMBPacket.addCommand if maxBufferSize is not None
	if maxBufferSize is not None:
		global login_MaxBufferSize
		login_MaxBufferSize = maxBufferSize
		setattr(smb.NewSMBPacket, "addCommand", NewSMBPacket_addCommand_hook_login)


class MYSMB(smb.SMB):
	def __init__(self, remote_host, use_ntlmv2=True, timeout=8):
		self.__use_ntlmv2 = use_ntlmv2
		self._default_tid = 0
		self._pid = os.getpid() & 0xffff
		self._last_mid = random.randint(1000, 20000)
		if 0x4000 <= self._last_mid <= 0x4110:
			self._last_mid += 0x120
		self._pkt_flags2 = 0
		self._last_tid = 0  # last tid from connect_tree()
		self._last_fid = 0  # last fid from nt_create_andx()
		self._smbConn = None
		smb.SMB.__init__(self, remote_host, remote_host, timeout=timeout)

	def set_pid(self, pid):
		self._pid = pid

	def get_pid(self):
		return self._pid

	def set_last_mid(self, mid):
		self._last_mid = mid

	def next_mid(self):
		self._last_mid += random.randint(1, 20)
		if 0x4000 <= self._last_mid <= 0x4110:
			self._last_mid += 0x120
		return self._last_mid

	def get_smbconnection(self):
		if self._smbConn is None:
			self.smbConn = smbconnection.SMBConnection(self.get_remote_host(), self.get_remote_host(), existingConnection=self, manualNegotiate=True)
		return self.smbConn

	def get_dce_rpc(self, named_pipe):
		smbConn = self.get_smbconnection()
		rpctransport = transport.SMBTransport(self.get_remote_host(), self.get_remote_host(), filename='\\'+named_pipe, smb_connection=smbConn)
		return rpctransport.get_dce_rpc()

	# override SMB.neg_session() to allow forcing ntlm authentication
	def neg_session(self, extended_security=True, negPacket=None):
		smb.SMB.neg_session(self, extended_security=self.__use_ntlmv2, negPacket=negPacket)

	# to use any login method, SMB must not be used from multiple thread
	def login(self, user, password, domain='', lmhash='', nthash='', ntlm_fallback=True, maxBufferSize=None):
		_setup_login_packet_hook(maxBufferSize)
		smb.SMB.login(self, user, password, domain, lmhash, nthash, ntlm_fallback)

	def login_standard(self, user, password, domain='', lmhash='', nthash='', maxBufferSize=None):
		_setup_login_packet_hook(maxBufferSize)
		smb.SMB.login_standard(self, user, password, domain, lmhash, nthash)

	def login_extended(self, user, password, domain='', lmhash='', nthash='', use_ntlmv2=True, maxBufferSize=None):
		_setup_login_packet_hook(maxBufferSize)
		smb.SMB.login_extended(self, user, password, domain, lmhash, nthash, use_ntlmv2)

	def connect_tree(self, path, password=None, service=smb.SERVICE_ANY, smb_packet=None):
		self._last_tid = smb.SMB.tree_connect_andx(self, path, password, service, smb_packet)
		return self._last_tid

	def get_last_tid(self):
		return self._last_tid

	def nt_create_andx(self, tid, filename, smb_packet=None, cmd=None, shareAccessMode=smb.FILE_SHARE_READ|smb.FILE_SHARE_WRITE, disposition=smb.FILE_OPEN, accessMask=0x2019f):
		self._last_fid = smb.SMB.nt_create_andx(self, tid, filename, smb_packet, cmd, shareAccessMode, disposition, accessMask)
		return self._last_fid

	def get_last_fid(self):
		return self._last_fid

	def set_default_tid(self, tid):
		self._default_tid = tid

	def set_pkt_flags2(self, flags):
		self._pkt_flags2 = flags

	def send_echo(self, data):
		pkt = smb.NewSMBPacket()
		pkt['Tid'] = self._default_tid
		
		transCommand = smb.SMBCommand(smb.SMB.SMB_COM_ECHO)
		transCommand['Parameters'] = smb.SMBEcho_Parameters()
		transCommand['Data'] = smb.SMBEcho_Data()

		transCommand['Parameters']['EchoCount'] = 1
		transCommand['Data']['Data'] = data
		pkt.addCommand(transCommand)

		self.sendSMB(pkt)
		return self.recvSMB()

	def do_write_andx_raw_pipe(self, fid, data, mid=None, pid=None, tid=None):
		writeAndX = smb.SMBCommand(smb.SMB.SMB_COM_WRITE_ANDX)
		writeAndX['Parameters'] = smb.SMBWriteAndX_Parameters_Short()
		writeAndX['Parameters']['Fid'] = fid
		writeAndX['Parameters']['Offset'] = 0
		writeAndX['Parameters']['WriteMode'] = 4  # SMB_WMODE_WRITE_RAW_NAMED_PIPE
		writeAndX['Parameters']['Remaining'] = 12345  # can be any. raw named pipe does not use it
		writeAndX['Parameters']['DataLength'] = len(data)
		writeAndX['Parameters']['DataOffset'] = 32 + len(writeAndX['Parameters']) + 1 + 2 + 1 # WordCount(1), ByteCount(2), Padding(1)
		writeAndX['Data'] = '\x00' + data  # pad 1 byte
		
		self.send_raw(self.create_smb_packet(writeAndX, mid, pid, tid))
		return self.recvSMB()

	def create_smb_packet(self, smbReq, mid=None, pid=None, tid=None):
		if mid is None:
			mid = self.next_mid()
		
		pkt = smb.NewSMBPacket()
		pkt.addCommand(smbReq)
		pkt['Tid'] = self._default_tid if tid is None else tid
		pkt['Uid'] = self._uid
		pkt['Pid'] = self._pid if pid is None else pid
		pkt['Mid'] = mid
		flags1, flags2 = self.get_flags()
		pkt['Flags1'] = flags1
		pkt['Flags2'] = self._pkt_flags2 if self._pkt_flags2 != 0 else flags2
		
		if self._SignatureEnabled:
			pkt['Flags2'] |= smb.SMB.FLAGS2_SMB_SECURITY_SIGNATURE
			self.signSMB(pkt, self._SigningSessionKey, self._SigningChallengeResponse)
			
		req = str(pkt)
		return '\x00'*2 + pack('>H', len(req)) + req  # assume length is <65536

	def send_raw(self, data):
		self.get_socket().send(data)

	def create_trans_packet(self, setup, param='', data='', mid=None, maxSetupCount=None, totalParameterCount=None, totalDataCount=None, maxParameterCount=None, maxDataCount=None, pid=None, tid=None, noPad=False):
		if maxSetupCount is None:
			maxSetupCount = len(setup)
		if totalParameterCount is None:
			totalParameterCount = len(param)
		if totalDataCount is None:
			totalDataCount = len(data)
		if maxParameterCount is None:
			maxParameterCount = totalParameterCount
		if maxDataCount is None:
			maxDataCount = totalDataCount
		transCmd = smb.SMBCommand(smb.SMB.SMB_COM_TRANSACTION)
		transCmd['Parameters'] = smb.SMBTransaction_Parameters()
		transCmd['Parameters']['TotalParameterCount'] = totalParameterCount
		transCmd['Parameters']['TotalDataCount'] = totalDataCount
		transCmd['Parameters']['MaxParameterCount'] = maxParameterCount
		transCmd['Parameters']['MaxDataCount'] = maxDataCount
		transCmd['Parameters']['MaxSetupCount'] = maxSetupCount
		transCmd['Parameters']['Flags'] = 0
		transCmd['Parameters']['Timeout'] = 0xffffffff
		transCmd['Parameters']['ParameterCount'] = len(param)
		transCmd['Parameters']['DataCount'] = len(data)
		transCmd['Parameters']['Setup'] = setup
		_put_trans_data(transCmd, param, data, noPad)
		return self.create_smb_packet(transCmd, mid, pid, tid)

	def send_trans(self, setup, param='', data='', mid=None, maxSetupCount=None, totalParameterCount=None, totalDataCount=None, maxParameterCount=None, maxDataCount=None, pid=None, tid=None, noPad=False):
		self.send_raw(self.create_trans_packet(setup, param, data, mid, maxSetupCount, totalParameterCount, totalDataCount, maxParameterCount, maxDataCount, pid, tid, noPad))
		return self.recvSMB()

	def create_trans_secondary_packet(self, mid, param='', paramDisplacement=0, data='', dataDisplacement=0, pid=None, tid=None, noPad=False):
		transCmd = smb.SMBCommand(smb.SMB.SMB_COM_TRANSACTION_SECONDARY)
		transCmd['Parameters'] = SMBTransactionSecondary_Parameters()
		transCmd['Parameters']['TotalParameterCount'] = len(param)
		transCmd['Parameters']['TotalDataCount'] = len(data)
		transCmd['Parameters']['ParameterCount'] = len(param)
		transCmd['Parameters']['ParameterDisplacement'] = paramDisplacement
		transCmd['Parameters']['DataCount'] = len(data)
		transCmd['Parameters']['DataDisplacement'] = dataDisplacement
		
		_put_trans_data(transCmd, param, data, noPad)
		return self.create_smb_packet(transCmd, mid, pid, tid)

	def send_trans_secondary(self, mid, param='', paramDisplacement=0, data='', dataDisplacement=0, pid=None, tid=None, noPad=False):
		self.send_raw(self.create_trans_secondary_packet(mid, param, paramDisplacement, data, dataDisplacement, pid, tid, noPad))

	def create_trans2_packet(self, setup, param='', data='', mid=None, maxSetupCount=None, totalParameterCount=None, totalDataCount=None, maxParameterCount=None, maxDataCount=None, pid=None, tid=None, noPad=False):
		if maxSetupCount is None:
			maxSetupCount = len(setup)
		if totalParameterCount is None:
			totalParameterCount = len(param)
		if totalDataCount is None:
			totalDataCount = len(data)
		if maxParameterCount is None:
			maxParameterCount = totalParameterCount
		if maxDataCount is None:
			maxDataCount = totalDataCount
		transCmd = smb.SMBCommand(smb.SMB.SMB_COM_TRANSACTION2)
		transCmd['Parameters'] = smb.SMBTransaction2_Parameters()
		transCmd['Parameters']['TotalParameterCount'] = totalParameterCount
		transCmd['Parameters']['TotalDataCount'] = totalDataCount
		transCmd['Parameters']['MaxParameterCount'] = maxParameterCount
		transCmd['Parameters']['MaxDataCount'] = maxDataCount
		transCmd['Parameters']['MaxSetupCount'] = len(setup)
		transCmd['Parameters']['Flags'] = 0
		transCmd['Parameters']['Timeout'] = 0xffffffff
		transCmd['Parameters']['ParameterCount'] = len(param)
		transCmd['Parameters']['DataCount'] = len(data)
		transCmd['Parameters']['Setup'] = setup
		_put_trans_data(transCmd, param, data, noPad)
		return self.create_smb_packet(transCmd, mid, pid, tid)

	def send_trans2(self, setup, param='', data='', mid=None, maxSetupCount=None, totalParameterCount=None, totalDataCount=None, maxParameterCount=None, maxDataCount=None, pid=None, tid=None, noPad=False):
		self.send_raw(self.create_trans2_packet(setup, param, data, mid, maxSetupCount, totalParameterCount, totalDataCount, maxParameterCount, maxDataCount, pid, tid, noPad))
		return self.recvSMB()

	def create_trans2_secondary_packet(self, mid, param='', paramDisplacement=0, data='', dataDisplacement=0, pid=None, tid=None, noPad=False):
		transCmd = smb.SMBCommand(smb.SMB.SMB_COM_TRANSACTION2_SECONDARY)
		transCmd['Parameters'] = SMBTransaction2Secondary_Parameters()
		transCmd['Parameters']['TotalParameterCount'] = len(param)
		transCmd['Parameters']['TotalDataCount'] = len(data)
		transCmd['Parameters']['ParameterCount'] = len(param)
		transCmd['Parameters']['ParameterDisplacement'] = paramDisplacement
		transCmd['Parameters']['DataCount'] = len(data)
		transCmd['Parameters']['DataDisplacement'] = dataDisplacement
		
		_put_trans_data(transCmd, param, data, noPad)
		return self.create_smb_packet(transCmd, mid, pid, tid)

	def send_trans2_secondary(self, mid, param='', paramDisplacement=0, data='', dataDisplacement=0, pid=None, tid=None, noPad=False):
		self.send_raw(self.create_trans2_secondary_packet(mid, param, paramDisplacement, data, dataDisplacement, pid, tid, noPad))

	def create_nt_trans_packet(self, function, setup='', param='', data='', mid=None, maxSetupCount=None, totalParameterCount=None, totalDataCount=None, maxParameterCount=None, maxDataCount=None, pid=None, tid=None, noPad=False):
		if maxSetupCount is None:
			maxSetupCount = len(setup)
		if totalParameterCount is None:
			totalParameterCount = len(param)
		if totalDataCount is None:
			totalDataCount = len(data)
		if maxParameterCount is None:
			maxParameterCount = totalParameterCount
		if maxDataCount is None:
			maxDataCount = totalDataCount
		transCmd = smb.SMBCommand(smb.SMB.SMB_COM_NT_TRANSACT)
		transCmd['Parameters'] = smb.SMBNTTransaction_Parameters()
		transCmd['Parameters']['MaxSetupCount'] = maxSetupCount
		transCmd['Parameters']['TotalParameterCount'] = totalParameterCount
		transCmd['Parameters']['TotalDataCount'] = totalDataCount
		transCmd['Parameters']['MaxParameterCount'] = maxParameterCount
		transCmd['Parameters']['MaxDataCount'] = maxDataCount
		transCmd['Parameters']['ParameterCount'] = len(param)
		transCmd['Parameters']['DataCount'] = len(data)
		transCmd['Parameters']['Function'] = function
		transCmd['Parameters']['Setup'] = setup
		_put_trans_data(transCmd, param, data, noPad)
		return self.create_smb_packet(transCmd, mid, pid, tid)

	def send_nt_trans(self, function, setup='', param='', data='', mid=None, maxSetupCount=None, totalParameterCount=None, totalDataCount=None, maxParameterCount=None, maxDataCount=None, pid=None, tid=None, noPad=False):
		self.send_raw(self.create_nt_trans_packet(function, setup, param, data, mid, maxSetupCount, totalParameterCount, totalDataCount, maxParameterCount, maxDataCount, pid, tid, noPad))
		return self.recvSMB()

	def create_nt_trans_secondary_packet(self, mid, param='', paramDisplacement=0, data='', dataDisplacement=0, pid=None, tid=None, noPad=False):
		transCmd = smb.SMBCommand(smb.SMB.SMB_COM_NT_TRANSACT_SECONDARY)
		transCmd['Parameters'] = SMBNTTransactionSecondary_Parameters()
		transCmd['Parameters']['TotalParameterCount'] = len(param)
		transCmd['Parameters']['TotalDataCount'] = len(data)
		transCmd['Parameters']['ParameterCount'] = len(param)
		transCmd['Parameters']['ParameterDisplacement'] = paramDisplacement
		transCmd['Parameters']['DataCount'] = len(data)
		transCmd['Parameters']['DataDisplacement'] = dataDisplacement
		_put_trans_data(transCmd, param, data, noPad)
		return self.create_smb_packet(transCmd, mid, pid, tid)

	def send_nt_trans_secondary(self, mid, param='', paramDisplacement=0, data='', dataDisplacement=0, pid=None, tid=None, noPad=False):
		self.send_raw(self.create_nt_trans_secondary_packet(mid, param, paramDisplacement, data, dataDisplacement, pid, tid, noPad))

	def recv_transaction_data(self, mid, minLen):
		data = ''
		while len(data) < minLen:
			recvPkt = self.recvSMB()
			if recvPkt['Mid'] != mid:
				continue
			resp = smb.SMBCommand(recvPkt['Data'][0])
			data += resp['Data'][1:]  # skip padding
			#print(len(data))
		return data

