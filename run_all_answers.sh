#!/bin/sh

ROOT_DIR=$(pwd)

# build
BUILD_DIR=${ROOT_DIR}/build
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ..
cmake --build .

# create list of executables
LIST=$(ls ${ROOT_DIR}/answers | grep -e '\.c' | sed -e 's/\.c//')

# set default src image
DEFAULT_SRC_IMG=${ROOT_DIR}/Gasyori100knock/dataset/images/imori_256x256.png

# run all executables
ANSWERS_DIR=${BUILD_DIR}/answers
cd ${ANSWERS_DIR}
echo "[run all answers]"
for ITEM in ${LIST}; do
    echo ">> ${ITEM}"
    ./${ITEM} ${DEFAULT_SRC_IMG}
done
echo "[done]"

# move output files 
OUT_IMG_DIR=${ANSWERS_DIR}/out
mkdir -p ${OUT_IMG_DIR}
mv *.png ${OUT_IMG_DIR}
