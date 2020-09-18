 <h1 align="center" >Welcome to Kernelhub</h1>


 <p align="center">
    <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://img.shields.io/badge/Ascotbe-Kernelhub-green"></a>
    <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://img.shields.io/badge/LICENSE-GPL-ff69b4"></a>
	<a href="https://github.com/Ascotbe/Kernelhub/stargazers"><img alt="Release" src="https://img.shields.io/github/stars/ascotbe/Kernelhub.svg"></a>
	<a href="https://github.com/Ascotbe/Medusa"><img alt="Release" src="https://img.shields.io/badge/scope-2000—2020-success"></a>
 </p>




**请使用者遵守 [中华人民共和国网络安全法](http://www.cac.gov.cn/2016-11/07/c_1119867116.htm)，勿将项目用于非授权的测试，否则造成的严重后果与本项目无关。**



> 前言

目前还在收集当中，有几个CVE并没有找到可以利用的源码或者脚本

还有各位大佬的项目地址我也没写文档，后面总结好会在每个CVE下面标明出处的，现在望见谅，各位看官可以点个star然后过一段时间后再来看，最少要HVV结束后了（逃

> 未测试成功编号

下列编号都是在筛选后未能通过复现测试的CVE，后续会再次进行复测

| CVE-2019-0859 | CVE-2018-8440 | CVE-2018-1038 | CVE-2016-3371 | CVE-2016-0041 |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| CVE-2016-3309 | CVE-2015-0002 | CVE-2015-1725 | CVE-2015-0062 | CVE-2015-0057 |
| CVE-2013-5065 | CVE-2013-0008 | CVE-2010-4398 | CVE-2014-6321 | CVE-2009-0079 |
| CVE-2011-0045 | CVE-2010-2554 | CVE-2010-0270 | CVE-2005-1983 | CVE-2015-2387 |
| CVE-2014-6324 | CVE-2012-0002 | CVE-2010-0020 | -             | -             |



### 编号列表

| SecurityBulletin                 |                         Description                          |              OperatingSystem               |
| :------------------------------- | :----------------------------------------------------------: | :----------------------------------------: |
| [CVE-2020-1472](./CVE-2020-1472) |               Netlogon Elevation of Privilege                | Windows 2008/2012/2016/2019/1903/1909/2004 |
| [CVE-2020-0796](./CVE-2020-0796) |                 SMBv3 Remote Code Execution                  |             Windows 1903/1909              |
| [CVE-2020-0787](./CVE-2020-0787) |       Windows Background Intelligent Transfer Service        |     Windows 7/8/10/2008/2012/2016/2019     |
| [CVE-2019-1458](./CVE-2019-1458) |                Win32k Elevation of Privilege                 |       Windows 7/8/10/2008/2012/2016        |
| [CVE-2019-1388](./CVE-2019-1388) |      Windows Certificate Dialog Elevation of Privilege       |      Windows 7/8/2008/2012/2016/2019       |
| [CVE-2019-0859](./CVE-2019-0859) |                Win32k Elevation of Privilege                 |     Windows 7/8/10/2008/2012/2016/2019     |
| [CVE-2019-0803](./CVE-2019-0803) |                Win32k Elevation of Privilege                 |     Windows 7/8/10/2008/2012/2016/2019     |
| [CVE-2018-8639](./CVE-2018-8639) |                Win32k Elevation of Privilege                 |     Windows 7/8/10/2008/2012/2016/2019     |
| [CVE-2018-8453](./CVE-2018-8453) |                Win32k Elevation of Privilege                 |     Windows 7/8/10/2008/2012/2016/2019     |
| [CVE-2018-8440](./CVE-2018-8440) |             Windows ALPC Elevation of Privilege              |       Windows 7/8/10/2008/2012/2016        |
| [CVE-2018-8120](./CVE-2018-8120) |                Win32k Elevation of Privilege                 |               Windows 7/2008               |
| [CVE-2018-1038](./CVE-2018-1038) |            Windows Kernel Elevation of Privilege             |               Windows 7/2008               |
| [CVE-2018-0743](./CVE-2018-0743) |      Windows Subsystem for Linux Elevation of Privilege      |              Windows 10/2016               |
| [CVE-2018-0833](./CVE-2018-0833) |       SMBv3 Null Pointer Dereference Denial of Service       |               Windows 8/2012               |
| [CVE-2017-8464](./CVE-2017-8464) |                  LNK Remote Code Execution                   |       Windows 7/8/10/2008/2012/2016        |
| [CVE-2017-0213](./CVE-2017-0213) |              Windows COM Elevation of Privilege              |       Windows 7/8/10/2008/2012/2016        |
| [CVE-2017-0143](./CVE-2017-0143) |                 Windows Kernel Mode Drivers                  |    Windows 7/8/10/2008/2012/2016/Vista     |
| [CVE-2017-0101](./CVE-2017-0101) |        GDI Palette Objects Local Privilege Escalation        |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-7255](./CVE-2016-7255) |                 Windows Kernel Mode Drivers                  |    Windows 7/8/10/2008/2012/2016/Vista     |
| [CVE-2016-3371](./CVE-2016-3371) |            Windows Kernel Elevation of Privilege             |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-3309](./CVE-2016-3309) |                Win32k Elevation of Privilege                 |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-3225](./CVE-2016-3225) |          Windows SMB Server Elevation of Privilege           |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-0099](./CVE-2016-0099) |                    Secondary Logon Handle                    |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-0095](./CVE-2016-0095) |                Win32k Elevation of Privilege                 |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-0051](./CVE-2016-0051) |                WebDAV Elevation of Privilege                 |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2016-0041](./CVE-2016-0041) |       Win32k Memory Corruption Elevation of Privilege        |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2015-2546](./CVE-2015-2546) |       Win32k Memory Corruption Elevation of Privilege        |       Windows 7/8/10/2008/2012/Vista       |
| [CVE-2015-2387](./CVE-2015-2387) |                 ATMFD.DLL Memory Corruption                  |    Windows 7/8/2003/2008/2012/Vista/Rt     |
| [CVE-2015-2370](./CVE-2015-2370) |              Windows RPC Elevation of Privilege              |    Windows 7/8/10/2003/2008/2012/Vista     |
| [CVE-2015-1725](./CVE-2015-1725) |                Win32k Elevation of Privilege                 |    Windows 7/8/10/2003/2008/2012/Vista     |
| [CVE-2015-1701](./CVE-2015-1701) |                 Windows Kernel Mode Drivers                  |         Windows 7/2003/2008/Vista          |
| [CVE-2015-0062](./CVE-2015-0062) |        Windows Create Process Elevation of Privilege         |           Windows 7/8/2008/2012            |
| [CVE-2015-0057](./CVE-2015-0057) |                Win32k Elevation of Privilege                 |      Windows 7/8/2003/2008/2012/Vista      |
| [CVE-2015-0003](./CVE-2015-0003) |                Win32k Elevation of Privilege                 |      Windows 7/8/2003/2008/2012/Vista      |
| [CVE-2015-0002](./CVE-2015-0002) | Microsoft Application Compatibility Infrastructure Elevation of Privilege |         Windows 7/8/2003/2008/2012         |
| [CVE-2014-6324](./CVE-2014-6324) |               Kerberos Checksum Vulnerability                |      Windows 7/8/2003/2008/2012/Vista      |
| [CVE-2014-6321](./CVE-2014-6321) |           Microsoft Schannel Remote Code Execution           |      Windows 7/8/2003/2008/2012/Vista      |
| [CVE-2014-4113](./CVE-2014-4113) |              Win32k.sys Elevation of Privilege               |      Windows 7/8/2003/2008/2012/Vista      |
| [CVE-2014-4076](./CVE-2014-4076) |                TCP/IP Elevation of Privilege                 |                Windows 2003                |
| [CVE-2014-1767](./CVE-2014-1767) |       Ancillary Function Driver Elevation of Privilege       |      Windows 7/8/2003/2008/2012/Vista      |
| [CVE-2013-5065](./CVE-2013-5065) |                         NDProxy.sys                          |              Windows XP/2003               |
| [CVE-2013-1345](./CVE-2013-1345) |                        Kernel Driver                         |   Windows 7/8/2003/2008/2012/Vista/Rt/Xp   |
| [CVE-2013-1332](./CVE-2013-1332) |        DirectX Graphics Kernel Subsystem Double Fetch        |    Windows 7/8/2003/2008/2012/Vista/Rt     |
| [CVE-2013-0008](./CVE-2013-0008) |               Win32k Improper Message Handling               |       Windows 7/8/2008/2012/Vista/Rt       |
| [CVE-2012-0217](./CVE-2012-0217) |                         Service Bus                          |           Windows 7/2003/2008/Xp           |
| [CVE-2012-0002](./CVE-2012-0002) |                   Remote Desktop Protocol                    |        Windows 7/2003/2008/Vista/Xp        |
| [CVE-2011-2005](./CVE-2011-2005) |       Ancillary Function Driver Elevation of Privilege       |              Windows 2003/Xp               |
| [CVE-2011-1974](./CVE-2011-1974) |               NDISTAPI Elevation of Privilege                |              Windows 2003/Xp               |
| [CVE-2011-1249](./CVE-2011-1249) |       Ancillary Function Driver Elevation of Privilege       |        Windows 7/2003/2008/Vista/Xp        |
| [CVE-2011-0045](./CVE-2011-0045) |              Windows Kernel Integer Truncation               |                 Windows Xp                 |
| [CVE-2010-4398](./CVE-2010-4398) |       Driver Improper Interaction with Windows Kernel        |        Windows 7/2003/2008/Vista/Xp        |
| [CVE-2010-3338](./CVE-2010-3338) |                        Task Scheduler                        |            Windows 7/2008/Vista            |
| [CVE-2010-2554](./CVE-2010-2554) |                   Tracing Registry Key ACL                   |            Windows 7/2008/Vista            |
| [CVE-2010-1897](./CVE-2010-1897) |                    Win32k Window Creation                    |        Windows 7/2003/2008/Vista/Xp        |
| [CVE-2010-0270](./CVE-2010-0270) |                    SMB Client Transaction                    |               Windows 7/2008               |
| [CVE-2010-0233](./CVE-2010-0233) |                  Windows Kernel Double Free                  |      Windows 2000/2003/2008/Vista/Xp       |
| [CVE-2010-0020](./CVE-2010-0020) |                    SMB Pathname Overflow                     |     Windows 7/2000/2003/2008/Vista/Xp      |
| [CVE-2009-2532](./CVE-2009-2532) |                     SMBv2 Command Value                      |             Windows 2008/Vista             |
| [CVE-2009-0079](./CVE-2009-0079) |               Windows RPCSS Service Isolation                |              Windows 2003/Xp               |
| [CVE-2008-4250](./CVE-2008-4250) |                        Server Service                        |         Windows 2000/2003/Vista/Xp         |
| [CVE-2008-4037](./CVE-2008-4037) |                  SMB Credential Reflection                   |      Windows 2000/2003/2008/Vista/Xp       |
| [CVE-2008-3464](./CVE-2008-3464) |                     AFD Kernel Overwrite                     |              Windows 2003/Xp               |
| [CVE-2008-1084](./CVE-2008-1084) |                          Win32.sys                           |      Windows 2000/2003/2008/Vista/Xp       |
| [CVE-2006-3439](./CVE-2006-3439) |                    Remote Code Execution                     |            Windows 2000/2003/Xp            |
| [CVE-2005-1983](./CVE-2005-1983) |                         PnP Service                          |              Windows 2000/Xp               |
| [CVE-2003-0352](./CVE-2003-0352) |               Buffer Overrun In RPC Interface                |          Windows 2000/2003/Xp/Nt           |





### 所需环境

- 测试目标系统

  ```bash
  #Windows 7 SP1 X64 
  ed2k://|file|cn_windows_7_home_premium_with_sp1_x64_dvd_u_676691.iso|3420557312|1A3CF44F3F5E0BE9BBC1A938706A3471|/
  #Windows 7 SP1 X86
  ed2k://|file|cn_windows_7_home_premium_with_sp1_x86_dvd_u_676770.iso|2653276160|A8E8BD4421174DF34BD14D60750B3CDB|/
  #Windows Server 2008 R2 SP1 X64 
  ed2k://|file|cn_windows_server_2008_r2_standard_enterprise_datacenter_and_web_with_sp1_x64_dvd_617598.iso|3368839168|D282F613A80C2F45FF23B79212A3CF67|/
  #Windows Server 2003 R2 SP2 x86
  ed2k://|file|cn_win_srv_2003_r2_enterprise_with_sp2_vl_cd1_X13-46432.iso|637917184|284DC0E76945125035B9208B9199E465|/
  #Windows Server 2003 R2 SP2 x64
  ed2k://|file|cn_win_srv_2003_r2_enterprise_x64_with_sp2_vl_cd1_X13-47314.iso|647686144|107F10D2A7FF12FFF0602FF60602BB37|/
  #Windows Server 2008 SP2 x86
  ed2k://|file|cn_windows_server_standard_enterprise_and_datacenter_with_sp2_x86_dvd_x15-41045.iso|2190057472|E93B029C442F19024AA9EF8FB02AC90B|/
  #Windows Server 2000 SP4 x86
  ed2k://|file|ZRMPSEL_CN.iso|402690048|00D1BDA0F057EDB8DA0B29CF5E188788|/
  #Windows Server 2003 SP2 x86
  thunder://QUFodHRwOi8vcy5zYWZlNS5jb20vV2luZG93c1NlcnZlcjIwMDNTUDJFbnRlcnByaXNlRWRpdGlvbi5pc29aWg==
  #Windows 8.1 x86
  ed2k://|file|cn_windows_8_1_enterprise_x86_dvd_2972257.iso|3050842112|6B60ABF8282F943FE92327463920FB67|/
  #Windows 10 1709 x64
  ed2k://|file|cn_windows_10_multi-edition_vl_version_1709_updated_dec_2017_x64_dvd_100406208.iso|5007116288|317BDC520FA2DD6005CBA8293EA06DF6|/
  ```

- Linux编译环境

  ```bash
  sudo vim /etc/apt/sources.list
  #在sources.list末尾添加deb http://us.archive.ubuntu.com/ubuntu trusty main universe
  sudo apt-get update
  sudo apt-get install mingw32 mingw32-binutils mingw32-runtime
  sudo apt-get install gcc-mingw-w64-i686 g++-mingw-w64-i686 mingw-w64-tools
  ```

- Windows编译环境

  ```
  VS2019（内置V142、V141、V120、V110、V100、V141_xp、V120_xp、V110_xp、MFC）
  ```








### 参考项目&网站

- [windows-kernel-exploits](https://github.com/SecWiki/windows-kernel-exploits)
- [WindowsExploits](https://github.com/abatchy17/WindowsExploits)
- [Exploits](https://github.com/WindowsExploits/Exploits)
- [CVE](cve.mitre.org)
- [CVE Details](https://www.cvedetails.com/)

