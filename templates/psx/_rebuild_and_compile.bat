@echo off
rd /s /q %~dp0build
call _build.bat
set PATH=C:\Program Files\CMake\bin;%PATH%
call _compile.bat