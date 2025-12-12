@echo off
echo Building SLHook DLL...

REM Set Visual Studio environment variables
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

REM Check if compiler is found
if not exist "%VCINSTALLDIR%" (
    echo Error: Visual Studio not found!
    echo Please check the Visual Studio installation path.
    pause
    exit /b 1
)

REM Create build directories
if not exist "build" mkdir build
if not exist "build\Debug" mkdir build\Debug
if not exist "build\Release" mkdir build\Release

REM Compile Debug version
echo Building Debug version...
cl /nologo /c /Zi /W3 /WX- /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "SLHOOK_EXPORTS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /Gm- /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Gd /TP /analyze- /errorReport:queue dllmain.cpp pch.cpp /Fobuild\Debug\

if %errorlevel% neq 0 (
    echo Debug build failed!
    pause
    exit /b 1
)

REM Link Debug version
link /nologo /DLL /INCREMENTAL /DEBUG /MACHINE:X86 /OUT:build\Debug\sppc.dll /IMPLIB:build\Debug\sppc.lib /DEF:SLHook.def build\Debug\dllmain.obj build\Debug\pch.obj slwga.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

if %errorlevel% neq 0 (
    echo Debug linking failed!
    pause
    exit /b 1
)

REM Compile Release version
echo Building Release version...
cl /nologo /c /W3 /WX- /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "SLHOOK_EXPORTS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /Gm- /EHsc /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Gd /TP /analyze- /errorReport:queue dllmain.cpp pch.cpp /Fobuild\Release\

if %errorlevel% neq 0 (
    echo Release build failed!
    pause
    exit /b 1
)

REM Link Release version
link /nologo /DLL /INCREMENTAL:NO /MACHINE:X86 /OUT:build\Release\sppc.dll /IMPLIB:build\Release\sppc.lib /DEF:SLHook.def build\Release\dllmain.obj build\Release\pch.obj slwga.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

if %errorlevel% neq 0 (
    echo Release linking failed!
    pause
    exit /b 1
)

echo Build completed successfully!
echo.
echo Debug version: build\Debug\sppc.dll
echo Release version: build\Release\sppc.dll
echo.
pause