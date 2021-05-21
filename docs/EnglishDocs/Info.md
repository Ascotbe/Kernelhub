 <h1 align="center" >Welcome to Kernelhub</h1>
 <p align="center">
    <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://img.shields.io/badge/Ascotbe-Kernelhub-green"></a>
    <a href="https://github.com/Ascotbe/Kernelhub"><img alt="Release" src="https://img.shields.io/badge/LICENSE-GPL-ff69b4"></a>
	<a href="https://github.com/Ascotbe/Kernelhub/stargazers"><img alt="Release" src="https://img.shields.io/github/stars/ascotbe/Kernelhub.svg"></a>
	<a href="https://github.com/Ascotbe/Medusa"><img alt="Release" src="https://img.shields.io/badge/scope-2000—2021-success"></a>
 </p>



> Preface

- This project is a collection of Windows privileges, in addition to not pass the test Exp, Demo GIF map, if the code in the project has your code, you have not labeled the source, please submit your Issues
- If there is an omission of the project, please pleasely add Issues and bring the use of code.
- This project is prioritized by the kernel-related rights vulnerability. If there is a remote command execution in the month, only when the Internet is expuple, it will be updated.
- The reliability of the code is self-verified, and the illness items you have caused are not responsible.

> Assist

In the project, the lack of ordinary order documentation, if English is good, friends welcome to submit PR

### Required environment

- Test target system

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
  ```

- Linux compilation environment

  ```bash
  sudo vim /etc/apt/sources.list
  #在sources.list末尾添加deb http://us.archive.ubuntu.com/ubuntu trusty main universe
  sudo apt-get update
  sudo apt-get install mingw32 mingw32-binutils mingw32-runtime
  sudo apt-get install gcc-mingw-w64-i686 g++-mingw-w64-i686 mingw-w64-tools
  ```

- Windows compilation environment

  ```bash
  #(.NET download address)https://dotnet.microsoft.com/download/visual-studio-sdks?utm_source=getdotnetsdk&utm_medium=referral
  VS2019（内置V142、V141、V120、V110、V100、V141_xp、V120_xp、V110_xp、MFC、.NET Framework 4.7.2）
  ```

### About the error

Due to the large content of the project, it is inevitable that there will be some typos or missing CVE numbers. If you find an error, you still hope to submit Issues to help me maintain the project.

### Disclaimer

This project is only oriented to **legally authorized** corporate safety construction behaviors. When using this project for testing, you should ensure that the behavior complies with local laws and regulations and has obtained sufficient authorization.

If you have any illegal behavior in the process of using this project, you need to bear the corresponding consequences yourself, and we will not bear any legal and joint liabilities. 

Before using this project, please **read carefully and fully understand the content of each clause**. Restrictions, exemption clauses or other clauses involving your major rights and interests may be bolded, underlined, etc. to remind you to pay attention. Unless you have fully read, fully understood and accepted all the terms of this agreement, please do not use this item. Your use behavior or your acceptance of this agreement in any other express or implied manner shall be deemed to have been read and agreed to be bound by this agreement.


### Reference project & website

- [windows-kernel-exploits](https://github.com/SecWiki/windows-kernel-exploits)
- [WindowsExploits](https://github.com/abatchy17/WindowsExploits)
- [Exploits](https://github.com/WindowsExploits/Exploits)
- [CVE](https://cve.mitre.org)
- [CVE Details](https://www.cvedetails.com/)

