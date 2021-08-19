@echo off
setlocal enableextensions enabledelayedexpansion
cd samples
for /D %%G in (*) do (
    cd %%G
    echo %%G
    rmdir /s /q zig-out
    rmdir /s /q zig-cache
    cd..
)
cd..
