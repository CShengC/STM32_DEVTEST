
@echo off

ECHO Open a project and data file, start auto processing and exit
cd ..
cd ..
set curdir=%cd%
set JLinkPath=C:\Program Files\SEGGER\JLink
set JFlashPath=%curdir%\template.jflash
set BinPath=%curdir%\build\template.elf

cd %JLinkPath%

JFlash.exe -openprj%JFlashPath% -open%BinPath%,0x08000000 -auto -exit

IF ERRORLEVEL 1 goto ERROR
goto END
:ERROR
ECHO J-Flash ARM: Error!
pause
:END