@echo off
ctags -R

<<<<<<< HEAD
cl  main.cpp util.cpp /MP4 /Ox /EHsc /GS- /nologo /Zi /DEBUG:FASTLINK
=======
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
>>>>>>> 3513a1b78996d0048adf12b477ae4cfc8753b450

