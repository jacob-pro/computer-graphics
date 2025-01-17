#!/bin/bash

set -e

## We need C++ 17 support, the lab machines are outdated
## We don't have root access
## So download and built GCC 10 from source
## This will take a long time!
if [ ! -d $HOME/GCC-10.1.0 ]; then

  if [ ! -d gcc-10.1.0 ]; then
    echo "Downloading GCC"
    wget -O gcc.tar.gz ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-10.1.0/gcc-10.1.0.tar.gz
    tar -xzf gcc.tar.gz
    rm gcc.tar.gz ## Save some space!
  fi

  echo "Compiling GCC"
  cd gcc-10.1.0
  ./contrib/download_prerequisites
  cd ..
  mkdir objdir
  cd objdir
  $PWD/../gcc-10.1.0/configure --prefix=$HOME/GCC-10.1.0 --enable-languages=c++ --disable-bootstrap
  make -j 12  ## i7-8700 = 6 cores
  make install
  cd ../

fi

## Compile CG2020
cmake -DCMAKE_C_COMPILER="$HOME/GCC-10.1.0/bin/gcc" -DCMAKE_CXX_COMPILER="$HOME/GCC-10.1.0/bin/g++" -DCMAKE_BUILD_TYPE=Release .
make
## We need to use the new libstdc++
export LD_LIBRARY_PATH="$HOME/GCC-10.1.0/lib64":$LD_LIBRARY_PATH
./CG2020
