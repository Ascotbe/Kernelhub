from impacket import version
from impacket.examples import logger
from impacket.examples.utils import parse_credentials
from impacket.dcerpc.v5 import samr, epm, transport
from impacket.spnego import SPNEGO_NegTokenInit, TypesMech

import ldap3
import argparse
import logging
import sys
import string
import random
import ssl
import os
from binascii import unhexlify


class AddComputerSAMR:

    def __init__(self, username, password, domain, cmdLineOptions,computer_name=None,computer_pass=None):
        self.options = cmdLineOptions
        self.__username = username
        self.__password = password
        self.__domain = domain
        self.__lmhash = ''
        self.__nthash = ''
        self.__hashes = cmdLineOptions.hashes
        self.__aesKey = cmdLineOptions.aesKey
        self.__doKerberos = cmdLineOptions.k
        self.__target = cmdLineOptions.dc_host
        self.__kdcHost = cmdLineOptions.dc_host
        self.__computerName = computer_name
        self.__computerPassword = computer_pass
        self.__method = 'SAMR'
        self.__port = cmdLineOptions.port
        self.__domainNetbios = cmdLineOptions.domain_netbios
        self.__noAdd = False
        self.__delete = False
        self.__targetIp = cmdLineOptions.dc_ip

        if self.__targetIp is not None:
            self.__kdcHost = self.__targetIp


        if self.__doKerberos and cmdLineOptions.dc_host is None:
            raise ValueError("Kerberos auth requires DNS name of the target DC. Use -dc-host.")


        if cmdLineOptions.hashes is not None:
            self.__lmhash, self.__nthash = cmdLineOptions.hashes.split(':')

        # if self.__computerName is None:
        #     if self.__noAdd:
        #         raise ValueError("You have to provide a computer name when using -no-add.")
        #     elif self.__delete:
        #         raise ValueError("You have to provide a computer name when using -delete.")
        # else:
        #     if self.__computerName[-1] != '$':
        #         self.__computerName += '$'

        if self.__computerPassword is None:
            self.__computerPassword = ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(32))

        if self.__target is None:
            self.__target = self.__domain

        if self.__port is None:
            if self.__method == 'SAMR':
                self.__port = 445
            elif self.__method == 'LDAPS':
                self.__port = 636

        if self.__domainNetbios is None:
            self.__domainNetbios = self.__domain



    def run_samr(self):
        if self.__targetIp is not None:
            stringBinding = epm.hept_map(self.__targetIp, samr.MSRPC_UUID_SAMR, protocol = 'ncacn_np')
        else:
            stringBinding = epm.hept_map(self.__target, samr.MSRPC_UUID_SAMR, protocol = 'ncacn_np')
        rpctransport = transport.DCERPCTransportFactory(stringBinding)
        rpctransport.set_dport(self.__port)

        if self.__targetIp is not None:
            rpctransport.setRemoteHost(self.__targetIp)
            rpctransport.setRemoteName(self.__target)

        if hasattr(rpctransport, 'set_credentials'):
            # This method exists only for selected protocol sequences.
            rpctransport.set_credentials(self.__username, self.__password, self.__domain, self.__lmhash,
                                         self.__nthash, self.__aesKey)

        rpctransport.set_kerberos(self.__doKerberos, self.__kdcHost)
        self.doSAMRAdd(rpctransport)

    def generateComputerName(self):
        return 'DESKTOP-' + (''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(8)) + '$')

    def doSAMRAdd(self, rpctransport):
        dce = rpctransport.get_dce_rpc()
        servHandle = None
        domainHandle = None
        userHandle = None
        try:
            dce.connect()
            dce.bind(samr.MSRPC_UUID_SAMR)

            samrConnectResponse = samr.hSamrConnect5(dce, '\\\\%s\x00' % self.__target,
                samr.SAM_SERVER_ENUMERATE_DOMAINS | samr.SAM_SERVER_LOOKUP_DOMAIN )
            servHandle = samrConnectResponse['ServerHandle']

            samrEnumResponse = samr.hSamrEnumerateDomainsInSamServer(dce, servHandle)
            domains = samrEnumResponse['Buffer']['Buffer']
            domainsWithoutBuiltin = list(filter(lambda x : x['Name'].lower() != 'builtin', domains))

            if len(domainsWithoutBuiltin) > 1:
                domain = list(filter(lambda x : x['Name'].lower() == self.__domainNetbios, domains))
                if len(domain) != 1:
                    logging.critical("This server provides multiple domains and '%s' isn't one of them.", self.__domainNetbios)
                    logging.critical("Available domain(s):")
                    for domain in domains:
                        logging.error(" * %s" % domain['Name'])
                    logging.critical("Consider using -domain-netbios argument to specify which one you meant.")
                    raise Exception()
                else:
                    selectedDomain = domain[0]['Name']
            else:
                selectedDomain = domainsWithoutBuiltin[0]['Name']

            samrLookupDomainResponse = samr.hSamrLookupDomainInSamServer(dce, servHandle, selectedDomain)
            domainSID = samrLookupDomainResponse['DomainId']

            if logging.getLogger().level == logging.DEBUG:
                logging.info("Opening domain %s..." % selectedDomain)
            samrOpenDomainResponse = samr.hSamrOpenDomain(dce, servHandle, samr.DOMAIN_LOOKUP | samr.DOMAIN_CREATE_USER , domainSID)
            domainHandle = samrOpenDomainResponse['DomainHandle']


            if self.__noAdd or self.__delete:
                try:
                    checkForUser = samr.hSamrLookupNamesInDomain(dce, domainHandle, [self.__computerName])
                except samr.DCERPCSessionError as e:
                    if e.error_code == 0xc0000073:
                        raise Exception("Account %s not found in domain %s!" % (self.__computerName, selectedDomain))
                    else:
                        raise

                userRID = checkForUser['RelativeIds']['Element'][0]
                if self.__delete:
                    access = samr.DELETE
                    message = "delete"
                else:
                    access = samr.USER_FORCE_PASSWORD_CHANGE
                    message = "set password for"
                try:
                    openUser = samr.hSamrOpenUser(dce, domainHandle, access, userRID)
                    userHandle = openUser['UserHandle']
                except samr.DCERPCSessionError as e:
                    if e.error_code == 0xc0000022:
                        raise Exception("User %s doesn't have right to %s %s!" % (self.__username, message, self.__computerName))
                    else:
                        raise
            else:
                if self.__computerName is not None:
                    try:
                        checkForUser = samr.hSamrLookupNamesInDomain(dce, domainHandle, [self.__computerName])
                        raise Exception("Account %s already exists!" % self.__computerName)
                    except samr.DCERPCSessionError as e:
                        if e.error_code != 0xc0000073:
                            raise
                else:
                    foundUnused = False
                    while not foundUnused:
                        self.__computerName = self.generateComputerName()
                        try:
                            checkForUser = samr.hSamrLookupNamesInDomain(dce, domainHandle, [self.__computerName])
                        except samr.DCERPCSessionError as e:
                            if e.error_code == 0xc0000073:
                                foundUnused = True
                            else:
                                raise

                try:
                    createUser = samr.hSamrCreateUser2InDomain(dce, domainHandle, self.__computerName, samr.USER_WORKSTATION_TRUST_ACCOUNT, samr.USER_FORCE_PASSWORD_CHANGE,)
                except samr.DCERPCSessionError as e:
                    if e.error_code == 0xc0000022:
                        raise Exception("User %s doesn't have right to create a machine account!" % self.__username)
                    elif e.error_code == 0xc00002e7:
                        raise Exception("User %s machine quota exceeded!" % self.__username)
                    else:
                        raise

                userHandle = createUser['UserHandle']

            if self.__delete:
                samr.hSamrDeleteUser(dce, userHandle)
                logging.info("Successfully deleted %s." % self.__computerName)
                userHandle = None
            else:
                samr.hSamrSetPasswordInternal4New(dce, userHandle, self.__computerPassword)
                if self.__noAdd:
                    logging.info("Successfully set password of %s to %s." % (self.__computerName, self.__computerPassword))
                else:
                    checkForUser = samr.hSamrLookupNamesInDomain(dce, domainHandle, [self.__computerName])
                    userRID = checkForUser['RelativeIds']['Element'][0]
                    openUser = samr.hSamrOpenUser(dce, domainHandle, samr.MAXIMUM_ALLOWED, userRID)
                    userHandle = openUser['UserHandle']
                    req = samr.SAMPR_USER_INFO_BUFFER()
                    req['tag'] = samr.USER_INFORMATION_CLASS.UserControlInformation
                    req['Control']['UserAccountControl'] = samr.USER_WORKSTATION_TRUST_ACCOUNT
                    samr.hSamrSetInformationUser2(dce, userHandle, req)
                    logging.info("Successfully added machine account %s with password %s." % (self.__computerName, self.__computerPassword))

        except Exception as e:
            if logging.getLogger().level == logging.DEBUG:
                import traceback
                traceback.print_exc()

            logging.critical(str(e))
        finally:
            if userHandle is not None:
                samr.hSamrCloseHandle(dce, userHandle)
            if domainHandle is not None:
                samr.hSamrCloseHandle(dce, domainHandle)
            if servHandle is not None:
                samr.hSamrCloseHandle(dce, servHandle)
            dce.disconnect()

    def run(self,delete=False):
        self.__delete = delete
        self.run_samr()



