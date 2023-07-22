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

# Project root
root_dir=$(pwd)

# Get a list of build/run targets
get_list() {
    answers_dir=${root_dir}/answers

    if [ $# -eq 0 ]; then
        # If no arguments are specified, put all of the existing answer programs to the list
        list=($(ls ${answers_dir} | grep -e '^[0-9]\{3\}_.\+' | sed 's/\.c//'))
    else
        # If numbers are specified, search answer programs which numbers are matched with
        # and append them to the list
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

# Configure CMake build
configure_build() {
    build_dir=$1

    pushd ${build_dir} > /dev/null
    cmake ..
    popd > /dev/null
}

# Build answer program
build_and_copy_answer() {
    build_dir=$1
    target=$2
    out_dir=$3

    pushd ${build_dir} > /dev/null
    cmake --build . --target ${target}

    cp ${build_dir}/answers/${target} ${out_dir}
    popd > /dev/null
}

# Run answer program
run_answer() {
    target=$1
    out_dir=$2

    pushd ${out_dir} > /dev/null
    ./${target}
    popd > /dev/null
}

main() {
    get_list $@

    # Build directory
    build_dir=${root_dir}/build
    configure_build ${build_dir}

    # Output directory
    out_dir=${build_dir}/output
    mkdir -p ${out_dir}

    # Create symblic links to input images
    image_dir=${root_dir}/images
    ln -s ${image_dir}/* ${out_dir}

    # Build and run answer programs
    for answer in ${list[@]}; do
        echo "> ${answer}"
        build_and_copy_answer ${build_dir} ${answer} ${out_dir}
        run_answer ${answer} ${out_dir}
    done

    # Unlink input images
    for link in $(ls -l ${out_dir} | grep -e "^l" | awk '{ print $9 }'); do
        unlink ${out_dir}/${link}
    done

    # Move binaries
    out_bin_dir=${out_dir}/bin
    mkdir -p ${out_bin_dir}
    for answer in ${list[@]}; do
        mv ${out_dir}/${answer} ${out_bin_dir}
    done

    echo "done"
}

main $@
