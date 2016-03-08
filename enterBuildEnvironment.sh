#!/bin/bash

docker run -i -t -v $(pwd):/src -w /src --net="host"  polybassa/pmd:latest /bin/bash 
