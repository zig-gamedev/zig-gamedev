@echo off
setlocal enableextensions enabledelayedexpansion
cd samples
for /D %%G in (*) do (
    cd %%G
    echo --- %%G --- output directory is "samples\%%G\zig-out\bin"
    if exist content\shaders\*.cso del content\shaders\*.cso
    if exist zig-out rmdir /s /q zig-out
    zig build
    cd..
)
cd..
