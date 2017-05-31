#!/bin/bash

script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

esp_sdk=esp-open-sdk

pushd $script_path
git clone --recursive https://github.com/pfalcon/${esp_sdk}.git

pushd $esp_sdk
make
