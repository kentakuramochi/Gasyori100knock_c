#!/bin/bash
########################################
# usage:
#    ./build_answers.sh
#       build all answer programs
#
########################################

ROOT_DIR=$(pwd)

# build
BUILD_DIR=${ROOT_DIR}/build
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ..
cmake --build .
