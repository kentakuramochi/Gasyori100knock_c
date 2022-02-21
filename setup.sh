#!/bin/sh

# clone dataset
get_dataset()
{
    # get dataset from https://github.com/yoyoyo-yo/Gasyori100knock/ with "sparse checkout"
    # (https://qiita.com/kuriya/items/3b72c90eebd79a10b7a4)
    #
    # images are cloned to: Gasyori100knock/dataset/images/*

    # create directory
    mkdir Gasyori100knock
    cd Gasyori100knock

    git init

    # enable sparse checkout
    git config core.sparsecheckout true

    git remote add origin https://github.com/yoyoyo-yo/Gasyori100knock.git

    # specify directory to clone
    echo /dataset/images > .git/info/sparse-checkout

    git pull origin master
}

# install libpng
sudo apt update
sudo apt install libpng-dev

get_dataset

