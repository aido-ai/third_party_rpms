#! /bin/bash

INSTALL_ROOT=$PWD/../_external/usr/local
echo $INSTALL_ROOT

# mkdir -p build && \
#     cd build && \
    cmake . -DBUILD_SHARED_LIBS=ON \
          -DABSL_ENABLE_INSTALL=ON \
          -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_ROOT/ && \
    make -j 96 && \
    make install
