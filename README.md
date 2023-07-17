# Gasyori100knock_c

## Description

「画像処理100本ノック」をC言語で実装する。

- [yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock) 
- [(Fork) ryoppippi/Gasyori100knock](https://github.com/ryoppippi/Gasyori100knock)

## Prerequesites

- GCC
- CMake (> 3.10.2)
- [libpng](http://www.libpng.org/pub/png/libpng.html)
    - ソース:[sourceforge](https://sourceforge.net/projects/libpng/files/)
    - APTでのインストール:`sudo apt install libpng-dev`

スクリプト`setup.sh`で上記必要物をダウンロード、インストールする。

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
（実行バイナリを`output/bin`、出力画像を`output/`以下に格納する）。

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
