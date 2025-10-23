#!/bin/zsh
set -e

cd build
./premake5.osx gmake2
cd ..
make
./bin/debug/jogo-estatistica
