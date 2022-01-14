@echo off
setlocal enableextensions enabledelayedexpansion
cd samples
for /D %%G in (*) do (
    cd %%G
    echo %%G
    if exist content\shaders\*.cso del content\shaders\*.cso
    if exist imgui.ini del imgui.ini
    if exist zig-out rmdir /s /q zig-out
    if exist zig-cache rmdir /s /q zig-cache
    cd..
)
cd..
