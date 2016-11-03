#!/bin/bash

sudo apt-get install flex gputils libboost-graph-dev subversion
svn checkout svn://svn.code.sf.net/p/sdcc/code/trunk/sdcc sdcc
pushd sdcc
./configure --disable-mcs51-port --disable-z80-port --disable-z180-port --disable-r2k-port --disable-r3ka-port --disable-gbz80-port --disable-tlcs90-port --disable-ds390-port --disable-ds400-port --disable-pic14-port --disable-hc08-port --disable-s08-port --disable-stm8-port
make `nproc`

# install a recent version of gputils
wget http://ftp.de.debian.org/debian/pool/main/g/gputils/gputils-common_1.4.0-0.1_all.deb
wget http://ftp.de.debian.org/debian/pool/main/g/gputils/gputils_1.4.0-0.1_amd64.deb
sudo dpkg -i gputils-common_1.4.0-0.1_all.deb
sudo dpkg -i gputils_1.4.0-0.1_amd64.deb
