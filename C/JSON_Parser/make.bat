@echo off
REM ---- Warnings (roughly -W -Wall -Wextra) ----
set "CWARN=/nologo /W4"

REM Optional: treat warnings as errors (like stricter CI)
REM set "CWARN=%CWARN% /WX"

REM ---- "Checks" equivalents ----
REM /RTC1 only works with /Od (debug-ish)
set "CCHECK=/Zi /Od /RTC1 /MTd /DDEBUG"

REM Optional static analysis (slower)
set "CCHECK=%CCHECK% /analyze"

REM Optional AddressSanitizer (only if your MSVC supports it)
REM set "CCHECK=%CCHECK% /fsanitize=address"

REM ---- C standard ----
REM Use /std:c11 or /std:c17 if your cl supports it.
set "CSTD=/std:c11"



setlocal

if "%~1"=="" (
  echo Usage: %~nx0 ^<file.c^>
  echo        %~nx0 ^<file.c^> --no-clean
  exit /b 1
)

set "NOCLEAN=0"
if /i "%~2"=="--no-clean" set "NOCLEAN=1"
if /i "%~2"=="-nc" set "NOCLEAN=1"

call :build "%~1" || exit /b 1
call :run   "%~1" || exit /b 1
if "%NOCLEAN%"=="0" (
  call :clean "%~1"
)

endlocal
echo [INFO] %~nx0 done!
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

echo [INFO] building %SRC%.
cl.exe %CWARN% %CCHECK% %CSTD% "%SRC%" ^
  /Fe:"%BUILDDIR%\%NAME%.exe" ^
  /Fo:"%BUILDDIR%\%NAME%.obj" ^
  /Fd:"%BUILDDIR%\%NAME%.pdb" >nul 2>nul
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
set "RC=%errorlevel%"
echo.
endlocal & exit /b %RC%



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