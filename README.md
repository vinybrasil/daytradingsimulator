# Day trading Simulator built with Raylib and WASM

A simple game built with Raylib and WASM that simulates the intraday price of a stock. The player can buy the asset (will gain if the price goes up) or sell it (will gain if the price goes down).

This game was built in C using a Raylib, a wonderful library for building simple games. The code was compiled to
    WebAssembly using Emscripten,
    a compiler focused in the Web.

## To compile to play it on a window

Create a variable where RAYLIB_SRC were compiled:
```
export RAYLIB_SRC=/home/vinybrasil/projects/raylib/raylib/src
```
Build the project:
```
mkdir build
./build.sh
```

## To compile to WebAssembly

Create a variable where RAYLIB_SRC were compiled:
```
export RAYLIB_SRC=/home/vinybrasil/projects/raylib/raylib/src
```
Build the project:
```
mkdir build
./build_wasm.sh
```

Run with Python:
```
python3 -m http.server
```
