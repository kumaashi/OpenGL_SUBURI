@echo off
ctags -R

cl  main.cpp util.cpp /MP4 /Ox /EHsc /GS- /nologo /Zi /DEBUG:FASTLINK

