#!/bin/bash
########################################
# usage:
#   ./run_answers.sh
#       build and run all answer programs
#
#   ./run_answers.sh [number [1-100]]
#       build and run answer programs with specified index
#       e.g.) ./run_answers.sh 1 3 10
#       -> build and run answer program 1, 3, 10
########################################

root_dir=$(pwd)

# answers
answers_dir=${root_dir}/answers

# get a list of targets
get_list()
{
    if [ $# -eq 0 ]; then
        # if no arguments, get all of the existing answer programs
        list=($(ls ${answers_dir} | grep -e '^[0-9]\{3\}_.\+' | sed 's/\.c//'))
    else
        # if numbers are specified, search matching answer programs and append it to the list
        list=()
        for n in $@; do
            idx=$(printf "%03d" ${n})
            tgt=$(basename $(find ${answers_dir} -maxdepth 1 -type f -name "${idx}_*"))
            if [ -z ${tgt} ]; then
                echo "cannot find answer program for Q. ${n}"
                exit 1
            else
                list+=(${tgt%.c})
            fi
        done
    fi
}

# build directory
build_dir=${root_dir}/build

# configure CMake build
configure_build()
{
    pushd ${build_dir} > /dev/null
    cmake ..
    popd > /dev/null
}

# build answer program
build_and_copy_answer()
{
    pushd ${build_dir} > /dev/null
    cmake --build . --target $1

    cp ${build_dir}/answers/$1 ${out_dir}
    popd > /dev/null
}

# output directory
out_dir=${build_dir}/output

# run answer program
run_answer()
{
    pushd ${out_dir} > /dev/null
    ./$1
    popd > /dev/null
}

get_list $@

configure_build

mkdir -p ${out_dir}

# input images
image_dir=${root_dir}/Gasyori100knock/dataset/images
# create symlink for input images
ln -s ${image_dir}/* ${out_dir}

# run answer programs
for answer in ${list[@]}; do
    echo "> ${answer}"
    build_and_copy_answer ${answer}
    run_answer ${answer}
done

# unlink input images
for link in $(ls -l ${out_dir} | grep -e "^l" | awk '{ print $9 }'); do
    unlink ${out_dir}/${link}
done

# move binaries
out_bin_dir=${out_dir}/bin
mkdir -p ${out_bin_dir}
for answer in ${list[@]}; do
    mv ${out_dir}/${answer} ${out_bin_dir}
done

echo "done"
