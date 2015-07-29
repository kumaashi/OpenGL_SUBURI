@echo off
ctags -R

REM cl  main.cpp util.cpp /MP4 /Ox /EHsc /GS- /nologo  /c
cl  main.cpp util.cpp /MP4 /Ox /EHsc /GS- /nologo /Z7 /c /DEBUG:FASTLINK

link *.obj /nologo
REM del *.obj
@echo on

REM start main.exe
REM 
REM vim .
REM 
REM 
REM kill

