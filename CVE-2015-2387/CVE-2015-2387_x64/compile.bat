REM @echo off
REM set PATH=c:\lcc\bin;%PATH%

REM lcc.exe -c -g2 source\insert_cert.c
REM lcclnk.exe  -s -subsystem console -o insert_cert.exe insert_cert.obj wtsapi32.lib crypt32.lib
REM del insert_cert.obj insert_cert.exp insert_cert.lib

lcc.exe -c -O elevator.c
lcc.exe -c -O loader.c

REM lcclnk.exe  -s -dll -o elevator.dll elevator.obj wtsapi32.lib crypt32.lib wininet.lib
REM lcclnk.exe  -s -dll -entry DllMain -o elevator.dll elevator.obj wtsapi32.lib crypt32.lib wininet.lib

lcclnk.exe  -subsystem console -o elevator.exe elevator.obj loader.obj wtsapi32.lib crypt32.lib wininet.lib Shell32.lib

del elevator.obj elevator.exp elevator.lib loader.obj