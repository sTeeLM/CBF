mkdir Release\zip
mkdir Release\zip\x86
mkdir Release\zip\x64

copy x64\Release\CBF.exe Release\zip\x64
copy x86\Release\CBF.exe Release\zip\x86

Tools\7-Zip\7z.exe a latest.zip Release\zip\*