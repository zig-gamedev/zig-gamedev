@echo off
setlocal enableextensions enabledelayedexpansion
cd samples
for /D %%G in (*) do (
    cd %%G
    echo --- %%G --- output directory is "samples\%%G\zig-out\bin"
    zig build
    cd..
)
cd..
