## 概念验证

在这个 repo 中，我们提供了 2 个不同的源代码：

- RpcClient.exe：触发 RPC 调用。
- SprintCSP.dll：可以放置它来利用 DLL 劫持。此 PoC 运行`whoami`命令并将输出写入`C:\ProgramData\whoamiall.txt`. 如果您想扩展此 PoC 的功能，您可以`DoStuff()`在main.c中编辑函数。

**为了使其工作，必须更改storsvc_c.c`#define`中的宏，以便利用程序适应目标机器的操作系统。**

## 步骤

1. 查找可写的 SYSTEM 路径`reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" -v Path`
2. 将SprintCSP.dll复制到可写路径。有些路径是不需要管理员权限的
3. 执行RpcClient.exe
4. 查看`C:\ProgramData\whoamiall.txt`

PS：生成好的脚本是调用cmd弹窗

![](https://raw.github.com/Ascotbe/Image/master/Kernelhub/LPE_via_StorSvc.gif)

## 代码来源

- [blackarrowsec](https://github.com/blackarrowsec/redteam-research)
- [OWwWO](https://github.com/OWwWO)
