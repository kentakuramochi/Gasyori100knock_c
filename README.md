# Gasyori100knock_c

**(2023/1/15) リポジトリ [yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock) は削除され、アクセスできなくなっている。**

## Description

「画像処理100本ノック」(
[yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock) 
) をC言語で実装する。

## Prerequesites

- CMake > 3.10.2
- [libpng](http://www.libpng.org/pub/png/libpng.html)
    - [sourceforge](https://sourceforge.net/projects/libpng/files/)
    - aptでのインストール: `sudo apt install libpng-dev`
- サンプル画像
    - [Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock.git) からクローン、`Gasyori100knock/`に配置

スクリプト`setup.sh`で上記必要物のダウンロード、インストールを行う。

## Imgdata library

CでPNG画像データを扱うための簡易的なライブラリ`imgdata`を作成している（ソース: `imgdata/`）。

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

スクリプト`./run_all_answers.sh` で全回答プログラムを一括実行する
（出力画像は `build/output/` 以下に格納する）。
