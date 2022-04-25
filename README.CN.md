 <h1 align="center" >Welcome to Kernelhub</h1>
 <p align="center">
    <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://img.shields.io/badge/Ascotbe-Kernelhub-green"></a>
     <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://visitor-badge.glitch.me/badge?page_id=https://github.com/Ascotbe/Kernelhub/README.md"></a>
    <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://img.shields.io/badge/LICENSE-GPL-ff69b4"></a>
	<a href="https://github.com/Ascotbe/Kernelhub/stargazers"><img alt="Release" src="https://img.shields.io/github/stars/ascotbe/Kernelhub.svg"></a>
	<a href="https://github.com/Ascotbe/Medusa"><img alt="Release" src="https://img.shields.io/badge/scope-2000—2021-success"></a>
     <a href="https://github.com/Ascotbe/Medusa"><img alt="Release" src="https://img.shields.io/github/repo-size/Ascotbe/Kernelhub"></a>
 </p>



> 前言

- 制作该项目的初衷是为了，学习、分析、研究最新的内核漏洞，以避免花费大量时间去查看系统以及相关内容
- 本项目是一个提权相关的集合，除测试失败或者未测试的EXP都有详细说明以及演示GIF图
- 如有本项目存在遗漏的CVE漏洞，欢迎提Issues并带上利用代码
- 项目代码禁止在真实环境进行测试！代码的可靠性自行验证，您造成的过失项目作者概不负责

> 在线预览

```
http://kernelhub.ascotbe.com
```

**中文文档 | [EnglishDocumentation](./README.md)**

> 编号列表

| SecurityBulletin                               |                         Description                          |                   OperatingSystem                   |
| :--------------------------------------------- | :----------------------------------------------------------: | :-------------------------------------------------: |
| [CVE-2022-21882](./CVE-2022-21882)             |                        Windows Win32k                        |           Windows 10/11/2019/2022/Server            |
| [CVE-2021-34486](./CVE-2021-34486)             |               Windows Event Tracing Elevation                |               Windows 10/2019/Server                |
| [CVE-2021-42287](./CVE-2021-42287)             |                          Windows AD                          |       Windows 2008/2012/2016/2019/2022/Server       |
| [CVE-2021-43224](./TestFailure/CVE-2021-43224) |                   Windows Common Log File                    |  Windows 7/8/10/11/2008/2012/2016/2019/2022/Server  |
| [CVE-2021-42278](./CVE-2021-42278)             |                          Windows AD                          |       Windows 2008/2012/2016/2019/2022/Server       |
| [CVE-2021-40449](./CVE-2021-40449)             |                        Windows Win32k                        | Windows 7/8.1/10/11/2008/2012/2016/2019/2022/Server |
| [CVE-2021-40444](./CVE-2021-40444)             |                        Windows IE RCE                        |  Windows 7/8.1/10/2008/2012/2016/2019/2022/Server   |
| [CVE-2021-36934](./CVE-2021-36934)             |                      Windows Elevation                       |                     Windows 10                      |
| [CVE-2021-34527](./TestFailure/CVE-2021-34527) |         Windows Print Spooler Remote Code Execution          |   Windows 7/8/10/2008/2012/2016/2019/2022/Server    |
| [CVE-2021-33739](./CVE-2021-33739)             |             Microsoft DWM Core Library Elevation             |                  Windows 10/Server                  |
| [CVE-2021-31166](./TestFailure/CVE-2021-31166) |                     HTTP Protocol Stack                      |                  Windows 10/Server                  |
| [CVE-2021-26868](./CVE-2021-33739)             |             Windows Graphics Component Elevation             |        Windows 8.1/10/2012/2016/2019/Server         |
| [CVE-2021-21551](./TestFailure/CVE-2021-21551) |                             None                             |                        None                         |
| [CVE-2021-1732](./CVE-2021-1732)               |                        Windows Win32k                        |               Windows 10/2019/Server                |
| [CVE-2021-1709](./TestFailure/CVE-2021-1709)   |                        Windows Win32k                        |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2020-17087](./TestFailure/CVE-2020-17087) |                Windows Kernel Local Elevation                |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2020-17057](./TestFailure/CVE-2020-17057) |                        Windows Win32k                        |             Windows 10/2016/2019/Server             |
| [CVE-2020-16938](./CVE-2020-16938)             |            Windows Kernel Information Disclosure             |                   Windows Server                    |
| [CVE-2020-16898](./CVE-2020-16898)             |             Windows TCP/IP Remote Code Execution             |               Windows 10/2019/Server                |
| [CVE-2020-1362](./TestFailure/CVE-2020-1362)   |         Windows WalletService Elevation of Privilege         |             Windows 10/2016/2019/Server             |
| [CVE-2020-1350](./TestFailure/CVE-2020-1350)   |                      Windows DNS Server                      |         Windows 2008/2012/2016/2019/Server          |
| [CVE-2020-1337](./CVE-2020-1337)               |               Windows Print Spooler Elevation                |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2020-1313](./CVE-2020-1313)               |        Windows Update Orchestrator Service Elevation         |                  Windows 10/Server                  |
| [CVE-2020-1066](./CVE-2020-1066)               |                   .NET Framework Elevation                   |                   Windows 7/2008                    |
| [CVE-2020-1054](./CVE-2020-1054)               |                       Win32k Elevation                       |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2020-1472](./CVE-2020-1472)               |                      Netlogon Elevation                      |         Windows 2008/2012/2016/2019/Server          |
| [CVE-2020-0668](./CVE-2020-0668)               |                   Windows Kernel Elevation                   |     Windows 7/8.1/10/2008/2012/2016/2019/Serve      |
| [CVE-2020-1015](./CVE-2020-1015)               |                      Windows Elevation                       |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2020-0814](./TestFailure/CVE-2020-0814)   |                 Windows Installer Elevation                  |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2020-0796](./CVE-2020-0796)               |                 SMBv3 Remote Code Execution                  |                   Windows Server                    |
| [CVE-2020-0787](./CVE-2020-0787)               |       Windows Background Intelligent Transfer Service        |         Windows 7/8/10/2008/2012/2016/2019          |
| [CVE-2020-0624](./TestFailure/CVE-2020-0624)   |                       Win32k Elevation                       |                  Windows 10/Server                  |
| [CVE-2019-0808](.CVE-2019-0808)                |                       Win32k Elevation                       |                   Windows 7/2008                    |
| [CVE-2020-0683](./CVE-2020-0683)               |                 Windows Installer Elevation                  |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2019-0623](.CVE-2019-0808)                |                       Win32k Elevation                       |        Windows 7/8.1/10/2008/2012/2016/Serve        |
| [CVE-2019-1458](./CVE-2019-1458)               |                       Win32k Elevation                       |            Windows 7/8/10/2008/2012/2016            |
| [CVE-2019-1422](./TestFailure/CVE-2019-1422)   |                      Windows Elevation                       |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2019-1388](./CVE-2019-1388)               |             Windows Certificate Dialog Elevation             |           Windows 7/8/2008/2012/2016/2019           |
| [CVE-2019-1322](./TestFailure/CVE-2019-1322)   |                      Windows Elevation                       |               Windows 10/2019/Server                |
| [CVE-2019-1253](./TestFailure/CVE-2019-1253)   |                      Windows Elevation                       |               Windows 10/2019/Server                |
| [CVE-2019-1215](./TestFailure/CVE-2019-1215)   |                      Windows Elevation                       |     Windows 7/8.1/10/2008/2012/2016/2019/Server     |
| [CVE-2019-1040](./TestFailure/CVE-2019-1040)   |                    Windows NTLM Tampering                    |      Windows 7/8/10/2008/2012/2016/2019/Serve       |
| [CVE-2019-0986](./TestFailure/CVE-2019-0986)   |            Windows User Profile Service Elevation            |      Windows 7/8/10/2008/2012/2016/2019/Serve       |
| [CVE-2019-0863](./TestFailure/CVE-2019-0863)   |              Windows Error Reporting Elevation               |      Windows 7/8/10/2008/2012/2016/2019/Serve       |
| [CVE-2019-0859](./TestFailure/)                |                       Win32k Elevation                       |         Windows 7/8/10/2008/2012/2016/2019          |
| [CVE-2019-0803](./CVE-2019-0803)               |                       Win32k Elevation                       |         Windows 7/8/10/2008/2012/2016/2019          |
| [CVE-2019-0708](./TestFailure/CVE-2019-0708)   |                   Remote Desktop Services                    |                   Windows 7/2008                    |
| [CVE-2018-8639](./CVE-2018-8639)               |                       Win32k Elevation                       |         Windows 7/8/10/2008/2012/2016/2019          |
| [CVE-2018-8453](./CVE-2018-8453)               |                       Win32k Elevation                       |         Windows 7/8/10/2008/2012/2016/2019          |
| [CVE-2018-8440](./TestFailure/CVE-2018-8440)   |                    Windows ALPC Elevation                    |            Windows 7/8/10/2008/2012/2016            |
| [CVE-2018-8120](./CVE-2018-8120)               |                       Win32k Elevation                       |                   Windows 7/2008                    |
| [CVE-2018-1038](./TestFailure/CVE-2018-1038)   |                   Windows Kernel Elevation                   |                   Windows 7/2008                    |
| [CVE-2018-0833](./CVE-2018-0833)               |       SMBv3 Null Pointer Dereference Denial of Service       |                   Windows 8/2012                    |
| [CVE-2018-0886](./TestFailure/CVE-2018-0886)   |                CredSSP Remote Code Execution                 |      Windows 7/8/10/2008/2012/2016/2019/Server      |
| [CVE-2018-0824](./TestFailure/CVE-2018-0824)   |            COM for Windows Remote Code Execution             |        Windows 7/8/10/2008/2012/2016/Server         |
| [CVE-2017-11783](./TestFailure/CVE-2017-11783) |                      Windows Elevation                       |               Windows 8/10/2012/2016                |
| [CVE-2017-8465](./TestFailure/CVE-2017-8465)   |                       Win32k Elevation                       |              Windows 7/8/10/2012/2016               |
| [CVE-2017-8464](./CVE-2017-8464)               |                  LNK Remote Code Execution                   |            Windows 7/8/10/2008/2012/2016            |
| [CVE-2017-0263](./TestFailure/CVE-2017-0263)   |                       Win32k Elevation                       |            Windows 7/8/10/2008/2012/2016            |
| [CVE-2017-0213](./CVE-2017-0213)               |                    Windows COM Elevation                     |            Windows 7/8/10/2008/2012/2016            |
| [CVE-2017-0143](./CVE-2017-0143)               |                 Windows Kernel Mode Drivers                  |         Windows 7/8/10/2008/2012/2016/Vista         |
| [CVE-2017-0101](./CVE-2017-0101)               |        GDI Palette Objects Local Privilege Escalation        |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2017-0100](./TestFailure/CVE-2017-0100)   |                  Windows HelpPane Elevation                  |            Windows 7/8/10/2008/2012/2016            |
| [CVE-2017-0005](./TestFailure/CVE-2017-0005)   |                    Windows GDI Elevation                     |       Windows 7/8/10/2008/2012/2016/Vista/XP        |
| [CVE-2016-7255](./CVE-2016-7255)               |                 Windows Kernel Mode Drivers                  |         Windows 7/8/10/2008/2012/2016/Vista         |
| [CVE-2016-3371](./CVE-2016-3371)               |                   Windows Kernel Elevation                   |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2016-3309](./TestFailure/CVE-2016-3309)   |                       Win32k Elevation                       |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2016-3225](./CVE-2016-3225)               |                 Windows SMB Server Elevation                 |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2016-0099](./CVE-2016-0099)               |                    Secondary Logon Handle                    |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2016-0095](./CVE-2016-0095)               |                       Win32k Elevation                       |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2016-0051](./CVE-2016-0051)               |                       WebDAV Elevation                       |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2016-0041](./CVE-2016-0041)               |              Win32k Memory Corruption Elevation              |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2015-2546](./CVE-2015-2546)               |              Win32k Memory Corruption Elevation              |           Windows 7/8/10/2008/2012/Vista            |
| [CVE-2015-2387](./CVE-2015-2387)               |                 ATMFD.DLL Memory Corruption                  |         Windows 7/8/2003/2008/2012/Vista/Rt         |
| [CVE-2015-2370](./CVE-2015-2370)               |                    Windows RPC Elevation                     |         Windows 7/8/10/2003/2008/2012/Vista         |
| [CVE-2015-1725](./TestFailure/CVE-2015-1725)   |                       Win32k Elevation                       |         Windows 7/8/10/2003/2008/2012/Vista         |
| [CVE-2015-1701](./CVE-2015-1701)               |                 Windows Kernel Mode Drivers                  |              Windows 7/2003/2008/Vista              |
| [CVE-2015-0062](./TestFailure/CVE-2015-0062)   |               Windows Create Process Elevation               |                Windows 7/8/2008/2012                |
| [CVE-2015-0057](./CVE-2015-0057)               |                       Win32k Elevation                       |          Windows 7/8/2003/2008/2012/Vista           |
| [CVE-2015-0003](./CVE-2015-0003)               |                       Win32k Elevation                       |          Windows 7/8/2003/2008/2012/Vista           |
| [CVE-2015-0002](./TestFailure/CVE-2015-0002)   | Microsoft Application Compatibility Infrastructure Elevation |             Windows 7/8/2003/2008/2012              |
| [CVE-2014-6324](./TestFailure/CVE-2014-6324)   |               Kerberos Checksum Vulnerability                |          Windows 7/8/2003/2008/2012/Vista           |
| [CVE-2014-6321](./TestFailure/CVE-2014-6321)   |           Microsoft Schannel Remote Code Execution           |          Windows 7/8/2003/2008/2012/Vista           |
| [CVE-2014-4113](./CVE-2014-4113)               |                     Win32k.sys Elevation                     |          Windows 7/8/2003/2008/2012/Vista           |
| [CVE-2014-4076](./CVE-2014-4076)               |                       TCP/IP Elevation                       |                    Windows 2003                     |
| [CVE-2014-1767](./CVE-2014-1767)               |             Ancillary Function Driver Elevation              |          Windows 7/8/2003/2008/2012/Vista           |
| [CVE-2013-5065](./TestFailure/CVE-2013-5065)   |                         NDProxy.sys                          |                   Windows XP/2003                   |
| [CVE-2013-1345](./CVE-2013-1345)               |                        Kernel Driver                         |       Windows 7/8/2003/2008/2012/Vista/Rt/Xp        |
| [CVE-2013-1332](./CVE-2013-1332)               |        DirectX Graphics Kernel Subsystem Double Fetch        |         Windows 7/8/2003/2008/2012/Vista/Rt         |
| [CVE-2013-1300](./TestFailure/CVE-2013-1300)   |                   Win32k Memory Allocation                   |         Windows 7/8/2003/2008/2012/Vista/Xp         |
| [CVE-2013-0008](./TestFailure/CVE-2013-0008)   |               Win32k Improper Message Handling               |           Windows 7/8/2008/2012/Vista/Rt            |
| [CVE-2012-0217](./CVE-2012-0217)               |                         Service Bus                          |               Windows 7/2003/2008/Xp                |
| [CVE-2012-0002](./TestFailure/CVE-2012-0002)   |                   Remote Desktop Protocol                    |            Windows 7/2003/2008/Vista/Xp             |
| [CVE-2011-2005](./CVE-2011-2005)               |             Ancillary Function Driver Elevation              |                   Windows 2003/Xp                   |
| [CVE-2011-1974](./CVE-2011-1974)               |                      NDISTAPI Elevation                      |                   Windows 2003/Xp                   |
| [CVE-2011-1249](./CVE-2011-1249)               |             Ancillary Function Driver Elevation              |            Windows 7/2003/2008/Vista/Xp             |
| [CVE-2011-1237](./TestFailure/CVE-2011-1237)   |                    Win32k Use After Free                     |            Windows 7/2003/2008/Vista/Xp             |
| [CVE-2011-0045](./TestFailure/CVE-2011-0045)   |              Windows Kernel Integer Truncation               |                     Windows Xp                      |
| [CVE-2010-4398](./CVE-2010-4398)               |       Driver Improper Interaction with Windows Kernel        |            Windows 7/2003/2008/Vista/Xp             |
| [CVE-2010-3338](./CVE-2010-3338)               |                        Task Scheduler                        |                Windows 7/2008/Vista                 |
| [CVE-2010-2554](./TestFailure/CVE-2010-2554)   |                   Tracing Registry Key ACL                   |                Windows 7/2008/Vista                 |
| [CVE-2010-1897](./CVE-2010-1897)               |                    Win32k Window Creation                    |            Windows 7/2003/2008/Vista/Xp             |
| [CVE-2010-0270](./CVE-2010-0270)               |                    SMB Client Transaction                    |                   Windows 7/2008                    |
| [CVE-2010-0233](./CVE-2010-0233)               |                  Windows Kernel Double Free                  |           Windows 2000/2003/2008/Vista/Xp           |
| [CVE-2010-0020](./TestFailure/CVE-2010-0020)   |                    SMB Pathname Overflow                     |          Windows 7/2000/2003/2008/Vista/Xp          |
| [CVE-2009-2532](./CVE-2009-2532)               |                     SMBv2 Command Value                      |                 Windows 2008/Vista                  |
| [CVE-2009-0079](./TestFailure/CVE-2009-0079)   |               Windows RPCSS Service Isolation                |                   Windows 2003/Xp                   |
| [CVE-2008-4250](./CVE-2008-4250)               |                        Server Service                        |             Windows 2000/2003/Vista/Xp              |
| [CVE-2008-4037](./CVE-2008-4037)               |                  SMB Credential Reflection                   |           Windows 2000/2003/2008/Vista/Xp           |
| [CVE-2008-3464](./CVE-2008-3464)               |                     AFD Kernel Overwrite                     |                   Windows 2003/Xp                   |
| [CVE-2008-1084](./CVE-2008-1084)               |                          Win32.sys                           |           Windows 2000/2003/2008/Vista/Xp           |
| [CVE-2006-3439](./CVE-2006-3439)               |                    Remote Code Execution                     |                Windows 2000/2003/Xp                 |
| [CVE-2005-1983](./TestFailure/CVE-2005-1983)   |                         PnP Service                          |                   Windows 2000/Xp                   |
| [CVE-2003-0352](./CVE-2003-0352)               |               Buffer Overrun In RPC Interface                |               Windows 2000/2003/Xp/Nt               |
| [CVE-2000-0979](./TestFailure/CVE-2000-0979)   |                     Share Level Password                     |                Windows 95/98/98se/Me                |



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
  #Windows 8.1 x64
  ed2k://|file|cn_windows_8_1_x64_dvd_2707237.iso|4076017664|839CBE17F3CE8411E8206B92658A91FA|/
  #Windows 10 1709 x64
  ed2k://|file|cn_windows_10_multi-edition_vl_version_1709_updated_dec_2017_x64_dvd_100406208.iso|5007116288|317BDC520FA2DD6005CBA8293EA06DF6|/
  #Windows 10 2004 x64 (2020-05-21 release version)
  magnet:?xt=urn:btih:8E49569FDE852E4F3CCB3D13EFB296B6B02D82A6
  #Windows 10 1909 x64 
  ed2k://|file|cn_windows_10_business_editions_version_1909_x64_dvd_0ca83907.iso|5275090944|9BCD5FA6C8009E4D0260E4B23008BD47|/
  #Windows 10 1607 x64 (Updated Jul 2016)
  ed2k://|file|cn_windows_10_multiple_editions_version_1607_updated_jul_2016_x64_dvd_9056935.iso|4347183104|35EA5DB0F3BB714F5CE0740FB89D82D1|/
  #Windows 10 1903 x64
  ed2k://|file|cn_windows_10_business_editions_version_1903_x64_dvd_e001dd2c.iso|4815527936|47D4C57E638DF8BF74C59261E2CE702D|/
  #Windows 10 21H2 x64 (2021-12-21 release version)
  magnet:?xt=urn:btih:a6594f14273fe02ae59a6d6848a47e55903d919b
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

  ```bash
  #(.NET下载地址)https://dotnet.microsoft.com/download/visual-studio-sdks?utm_source=getdotnetsdk&utm_medium=referral
  VS2019（内置V142、V141、V120、V110、V100、V141_xp、V120_xp、V110_xp、MFC、.NET Framework 4.7.2）
  ```

### 关于错误

由于项目内容较多，难免有些错别字或者遗漏的CVE编号等问题，如果您发现了错误，还望提交Issues来帮助我维护该项目。

> 未测试成功编号

下列编号都是在筛选后未能通过复现测试的CVE，附带未成功原因，欢迎提交PR

| SecurityBulletin |   |   |   |
| ---------------- |---------------- | ---------------- | ---------------- |
| CVE-2000-0979| CVE-2005-1983| CVE-2009-0079| CVE-2010-0020| CVE-2010-2554|
| CVE-2011-0045| CVE-2011-1237| CVE-2013-0008| CVE-2013-1300| CVE-2013-5065|
| CVE-2014-6321| CVE-2014-6324| CVE-2015-0002| CVE-2015-0062| CVE-2015-1725|
| CVE-2016-3309| CVE-2017-0005| CVE-2017-0100| CVE-2017-0263| CVE-2017-11783|
| CVE-2017-8465| CVE-2018-0824| CVE-2018-0886| CVE-2018-1038| CVE-2018-8440|
| CVE-2019-0708| CVE-2019-0859| CVE-2019-0863| CVE-2019-0986| CVE-2019-1040|
| CVE-2019-1215| CVE-2019-1253| CVE-2019-1322| CVE-2019-1422| CVE-2020-0624|
| CVE-2020-0814| CVE-2020-1350| CVE-2020-1362| CVE-2020-17057| CVE-2020-17087|
| CVE-2021-1709| CVE-2021-21551| CVE-2021-31166| CVE-2021-34527| CVE-2021-43224|
| CVE-2021-43883   |


### 免责声明

本项目仅面向**合法授权**的企业安全建设行为，在使用本项目进行检测时，您应确保该行为符合当地的法律法规，并且已经取得了足够的授权。

如您在使用本项目的过程中存在任何非法行为，您需自行承担相应后果，我们将不承担任何法律及连带责任。

在使用本项目前，请您**务必审慎阅读、充分理解各条款内容**，限制、免责条款或者其他涉及您重大权益的条款可能会以加粗、加下划线等形式提示您重点注意。 除非您已充分阅读、完全理解并接受本协议所有条款，否则，请您不要使用本项目。您的使用行为或者您以其他任何明示或者默示方式表示接受本协议的，即视为您已阅读并同意本协议的约束。


### 参考项目&网站

- [windows-kernel-exploits](https://github.com/SecWiki/windows-kernel-exploits)
- [WindowsExploits](https://github.com/abatchy17/WindowsExploits)
- [Exploits](https://github.com/WindowsExploits/Exploits)
- [CVE](https://cve.mitre.org)
- [CVE Details](https://www.cvedetails.com/)

### Stats

![Alt](https://repobeats.axiom.co/api/embed/2f17ae8019f36294dafafd5cf9ffe9b0e62a21ae.svg "Repobeats analytics image")
