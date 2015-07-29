@echo off
ctags -R

REM cl  main.cpp util.cpp /MP4 /Ox /EHsc /GS- /nologo  /c

cl  main.cpp util.cpp /MP4 /Ox /EHsc /GS- /nologo /Zi /c /DEBUG:FASTLINK

link *.obj /nologo
del *.obj
@echo on

start main.exe

vim .


kill

