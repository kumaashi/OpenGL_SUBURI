@echo off
ctags -R

cl  main.cpp util.cpp /Iinclude /MP4 /Ox /EHsc /GS- /nologo

