@echo off
setlocal enableextensions enabledelayedexpansion
cd samples
for /D %%G in (*) do (
    cd %%G
    echo %%G
    if exist zig-out rmdir /s /q zig-out
    if exist zig-cache rmdir /s /q zig-cache
    cd..
)
cd..
