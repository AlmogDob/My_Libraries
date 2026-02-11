@echo off
REM ---- Warnings (roughly -W -Wall -Wextra) ----
set "CWARN=/nologo /W4"

REM Optional: treat warnings as errors (like stricter CI)
REM set "CWARN=%CWARN% /WX"

REM ---- "Checks" equivalents ----
REM /RTC1 only works with /Od (debug-ish)
set "CCHECK=/Zi /Od /RTC1 /MTd /DDEBUG"

REM Optional static analysis (slower)
@REM set "CCHECK=%CCHECK% /analyze"

REM Optional AddressSanitizer (only if your MSVC supports it)
REM set "CCHECK=%CCHECK% /fsanitize=address"

REM ---- C standard ----
REM Use /std:c11 or /std:c17 if your cl supports it.
set "CSTD=/std:c11"

set "CLINKS= /link ws2_32.lib"

setlocal EnableExtensions EnableDelayedExpansion

REM ----------------------------
REM Parse args: many .c files + flags
REM ----------------------------
set "NOCLEAN=0"
set "BUILDONLY=0"
set "CLEANONLY=0"
set "FILES="

:parse
if "%~1"=="" goto doneparse

REM Flags (you can add more here)
if /i "%~1"=="--no-clean"   (set "NOCLEAN=1"  & shift & goto parse)
if /i "%~1"=="-nc"          (set "NOCLEAN=1"  & shift & goto parse)
if /i "%~1"=="--build-only" (set "BUILDONLY=1" & shift & goto parse)
if /i "%~1"=="-b"           (set "BUILDONLY=1" & shift & goto parse)
if /i "%~1"=="--clean-only" (set "CLEANONLY=1" & shift & goto parse)
if /i "%~1"=="-c"           (set "CLEANONLY=1" & shift & goto parse)

REM Otherwise treat as file
set "FILES=!FILES! "%~1""
shift
goto parse

:doneparse

if "%FILES%"=="" (
  echo Usage: %~nx0 ^<file1.c^> [file2.c ...] [--no-clean^|-nc] [--build-only^|-b] [--clean-only^|-c]
  exit /b 1
)

if "%BUILDONLY%"=="1" if "%CLEANONLY%"=="1" (
  echo Error: --build-only and --clean-only cannot be used together.
  exit /b 1
)

REM ----------------------------
REM Perform requested action on all files
REM ----------------------------
if "%CLEANONLY%"=="1" (
  for %%F in (%FILES%) do (
    call :clean "%%~fF" || exit /b !errorlevel!
  )
  echo [INFO] done!
  exit /b 0
)

for %%F in (%FILES%) do (
  call :build "%%~fF" || exit /b !errorlevel!
)

if "%BUILDONLY%"=="1" (
  echo [INFO] done!
  exit /b 0
)

for %%F in (%FILES%) do (
  call :run "%%~fF" || exit /b !errorlevel!
)

if "%NOCLEAN%"=="0" (
  for %%F in (%FILES%) do (
    call :clean "%%~fF"
  )
)

echo [INFO] done!
exit /b 0


REM /////////////////////////////
REM build
REM /////////////////////////////
:build
setlocal EnableExtensions

if "%~1"=="" (
  echo Usage: %~nx0 ^<file.c^>
  exit /b 1
)
if /i not "%~x1"==".c" (
  echo Error: expected a .c file, got "%~1"
  exit /b 1
)
if not exist "%~1" (
  echo Error: file not found: "%~1"
  exit /b 1
)

set "SRC=%~1"
set "NAME=%~n1"
set "BUILDDIR=build"

mkdir "%BUILDDIR%" 2>nul

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul 2>nul
if errorlevel 1 exit /b 1

echo [INFO] building:
cl.exe %CWARN% %CCHECK% %CSTD% "%SRC%" ^
  /Fe:"%BUILDDIR%\%NAME%.exe" ^
  /Fo:"%BUILDDIR%\%NAME%.obj" ^
  /Fd:"%BUILDDIR%\%NAME%.pdb" %CLINKS%
set "RC=%errorlevel%"
endlocal & exit /b %RC%


REM /////////////////////////////
REM run
REM /////////////////////////////
:run
setlocal EnableExtensions

if "%~1"=="" (
  echo Usage: %~nx0 ^<file.c^>
  exit /b 1
)

set "NAME=%~n1"
set "BUILDDIR=build"
set "EXE=%BUILDDIR%\%NAME%.exe"

if not exist "%EXE%" (
  echo Error: "%EXE%" not found. Run build.bat %1
  exit /b 1
)

echo [INFO] running %EXE%.
echo.
"%EXE%"
echo.
endlocal
exit /b 0


REM /////////////////////////////
REM clean
REM /////////////////////////////
:clean
setlocal EnableExtensions

if "%~1"=="" (
  echo Usage: %~nx0 ^<file.c^>
  exit /b 1
)

set "NAME=%~n1"
echo [INFO] removing %NAME% build files.
del /q build\%NAME%.* >nul 2>nul
endlocal
exit /b 0