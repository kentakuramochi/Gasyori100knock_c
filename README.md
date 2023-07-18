# Gasyori100knock_c

## Description

「画像処理100本ノック」をC言語で実装する。

- [yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock) 
- [(Fork) ryoppippi/Gasyori100knock](https://github.com/ryoppippi/Gasyori100knock)

## Prerequesites

- GCC
- GNU Make
- CMake (> 3.10.2)
- [libpng](http://www.libpng.org/pub/png/libpng.html)

### Docker container

Dockerが利用できる場合、スクリプト `run_docker.sh` で動作環境のDockerコンテナを起動する
（初回起動時にDockerイメージを作成）。

```sh
$ ./run_docker.sh
ubuntu@f82cef023651:/workspace$
```

イメージ名は `g100knock`、コンテナ名は `g100knock_work` を設定している。

```sh
$ docker images
REPOSITORY   TAG       IMAGE ID       CREATED             SIZE
g100knock    latest    69c27e22fd89   About an hour ago   453MB
...

$ docker ps -a
CONTAINER ID   IMAGE          COMMAND                  CREATED         STATUS                      PORTS     NAMES
f82cef023651   g100knock      "/bin/bash"              3 seconds ago   Up 3 seconds                          g100knock_work
...
```

## Imgdata library

CでPNG画像データを扱うための簡易的なライブラリ`imgdata`を作成する（`imgdata/`）。

`--target example` の指定でサンプル（`example/example.c`）をビルドできる。

```sh
$ cmake --build . --target example
```

## Answer

各問題に対する回答ソースを`answers/`以下に格納する。

デフォルトターゲットとしてビルドされる。

```sh
# @project root
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

スクリプト`./run_answers.sh`で全回答プログラムを一括でビルド・実行する
（実行バイナリ、出力画像を`build/output/`以下に格納する）。

```sh
# @project root
$ ./run_answers.sh
-- Configuring done
-- Generating done
-- Build files have been written to: <repository>/build
> 001_rgb_to_bgr
Consolidate compiler generated dependencies of target imgdata
[ 50%] Built target imgdata
Consolidate compiler generated dependencies of target 001_rgb_to_bgr
[100%] Built target 001_rgb_to_bgr
> 002_grayscale
[ 50%] Built target imgdata
Consolidate compiler generated dependencies of target 002_grayscale
[100%] Built target 002_grayscale
> 003_binarization
...
```

## TODO

- ビルド·実行環境の整理
