#!/bin/bash
svn checkout svn://svn.code.sf.net/p/sdcc/code/trunk/sdcc sdcc
sudo apt-get install libboost-graph-dev
pushd sdcc
./configure
make
