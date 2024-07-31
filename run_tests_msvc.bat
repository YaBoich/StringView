@echo off
rem Launch this from an MSVC-enabled console

set CFLAGS=/Wall /WX /wd4710 /wd5045 /std:c11 /FC /TC /Zi /nologo

cl.exe %CFLAGS% test.c

test.exe
