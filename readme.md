1: 
`mklink "%programfiles%\Microsoft Office\root\vfs\System\sppcs.dll" "%windir%\System32\sppc.dll"`
2:
`copy /y sppc64.dll "%programfiles%\Microsoft Office\root\vfs\System\sppc.dll"`
3.可选：如果提示微软服务器关闭问题，可以 修改hosts 
`0.0.0.0 ols.officeapps.live.com`

4.
如果你有visual studio可以选择手动编译build.bat
```
echo Debug version: build\Debug\sppc.dll
echo Release version: build\Release\sppc.dll
```