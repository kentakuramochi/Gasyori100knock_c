#!/bin/bash
########################################
# usage:
#    ./run_answers.sh
#       run all answer programs
#
#    ./run_answers.sh [number [1-100]]
#       run answer programs with specified Q. index
#       e.g.) ./run_answers.sh 1 3 10
#
########################################

ROOT_DIR=$(pwd)

# create list of answer programs
ANSWERS_DIR=${ROOT_DIR}/build/answers
if [ $# -eq 0 ]; then
    # if no arguments, get all of the existing answer programs
    LIST=($(ls ${ANSWERS_DIR} | grep -e '^[0-9]\{3\}_.\+'))
else
    # if Q numbers are specified, search matching answer programs and append it to the list
    LIST=()
    for N in $@; do
        TGT_IDX=$(printf "%03d" ${N})
        TGT=$(basename $(find ${ANSWERS_DIR} -maxdepth 1 -type f -name "${TGT_IDX}_*"))
        if [ -z ${TGT} ]; then
            echo "cannot find answer program for Q. ${N}"
            exit 1
        else
            LIST+=(${TGT})
        fi
    done
fi

# set src images used as an input for each question 
SRC_IMG_DIR=${ROOT_DIR}/Gasyori100knock/dataset/images
SRC_IMGS=(
    "" # no Q. 0
    # Q. 1-10
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256_noise.png"
    "${SRC_IMG_DIR}/imori_256x256_noise.png"
    # Q. 11-20
    "${SRC_IMG_DIR}/imori_256x256_noise.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256.png"
    "${SRC_IMG_DIR}/imori_256x256_noise.png"
    "${SRC_IMG_DIR}/imori_256x256_dark.png"
)

# create directory for output images
OUT_IMG_DIR=${ROOT_DIR}/output
mkdir -p ${OUT_IMG_DIR}

# run answer programs
echo "[run answers]"

cd ${ANSWERS_DIR}
for ANS in ${LIST[@]}; do
    # extract Q. index
    IDX=$(echo "${ANS}" | grep -o -e '^[0-9]\{3\}' | sed -e 's/^0\+//')
    echo "> ${ANS}"
    ./${ANS} ${SRC_IMGS[${IDX}]}
    # move output image
    if [ $? -eq 0 ]; then
        mv *.png ${OUT_IMG_DIR}
    fi
done

echo "[done]"
