@echo off
setlocal enableextensions enabledelayedexpansion
cd samples
for /D %%G in (*) do (
    cd %%G
    echo %%G
    zig build
    cd..
)
cd..
