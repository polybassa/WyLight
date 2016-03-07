#!/bin/sh
BOBLIGHT=boblightd
svn checkout http://boblight.googlecode.com/svn/trunk/ ${BOBLIGHT}
cd ${BOBLIGHT}
./configure
make
sudo make install
