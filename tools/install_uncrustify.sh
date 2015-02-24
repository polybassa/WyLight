#!/bin/bash
git clone git://github.com/bengardner/uncrustify.git
cd uncrustify
./configure
make
sudo make install
