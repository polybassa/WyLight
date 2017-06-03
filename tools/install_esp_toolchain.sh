#!/bin/bash

script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

esp_sdk=esp-open-sdk
esp_rtos=esp-open-rtos

pushd $script_path
git clone --recursive https://github.com/pfalcon/${esp_sdk}.git
git clone --recursive https://github.com/Superhouse/${esp_rtos}.git

pushd $esp_sdk
make
