目前还在收集当中，需要找系统测试，所以更新没那么快

还有各位大佬的项目地址我也没写文档，后面总结好会在每个CVE下面标明出处的，现在望见谅，各位看官可以点个star然后过一个月再来看（





### 所需环境

- 系统环境

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
  VS2019（内置V142、V141、V120、V110、V100、V120_xp、V110_xp、V110_xp、MFC）
  ```

  

