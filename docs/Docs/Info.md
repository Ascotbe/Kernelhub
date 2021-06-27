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

- 本项目是一个Windows提权相关的集合，除未通过测试EXP都有详细说明以及演示GIF图，如果项目中的代码有您的代码，本人未标注来源的请提交Issues
- 如有本项目存在遗漏的CVE漏洞，欢迎提Issues并带上利用代码
- 本项目优先追踪内核相关提权漏洞，如果当月漏洞存在远程命令执行的话，只有在互联网生存在EXP或者POC的时候才会更新
- 代码的可靠性自行验证，您造成的过失项目作者概不负责

> 协助

项目中暂缺单个文档的英文翻译，如果英文好的小伙伴欢迎提交PR

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

