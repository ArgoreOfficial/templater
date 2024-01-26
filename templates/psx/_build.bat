@echo off
cmake --preset default .
break>texture_list.txt
for %%i in (res\*.bmp) do call tools\img2tim.exe %%i
for %%i in (res\*.tim) do (call :tex_list %%~ni %%i )
goto :eof

:tex_list
set "path=%2"
set "path=%path:\=/%"
echo psn00bsdk_target_incbin(game PRIVATE %1 %path%) >> texture_list.txt
goto :eof