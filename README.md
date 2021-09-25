# Gasyori100knock_c

## Description

[yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock)

「画像処理100本ノック」をC言語で挑戦してみる

## Environment

WSL Ubuntu 18.04.5 LTS

## Prerequesites

- CMake 3.10.2

- [libpng](http://www.libpng.org/pub/png/libpng.html)

    PNG画像の読み込みに使用。
    [sourceforge](https://sourceforge.net/projects/libpng/files/)から取得したソースコードのビルドほか、`apt`でライブラリのインストールが可能

    ```bash
    $ sudo apt install libpng-dev

    $ apt-show-versions -p libpng-dev
    libpng-dev:amd64/bionic-security 1.6.34-1ubuntu0.18.04.2 uptodate
    ```

## Build

デフォルトのビルドターゲットは`questions/*`以下各ソース（ライブラリが依存してビルドされる）

```sh
# @project root
$ mkdir build
$ cd build

$ cmake ..
-- The C compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done

$ cmake --build .
Scanning dependencies of target imgdata
[ 25%] Building C object utils/CMakeFiles/imgdata.dir/imgdata.c.o
[ 50%] Linking C shared library libimgdata.so
[ 50%] Built target imgdata
Scanning dependencies of target 001_rgb_to_bgr
[ 75%] Building C object questions/CMakeFiles/001_rgb_to_bgr.dir/001_rgb_to_bgr.c.o
[100%] Linking C executable 001_rgb_to_bgr
[100%] Built target 001_rgb_to_bgr
```

## Image operation

簡易的な画像データ構造とその操作をライブラリ`libimgdata`として作成
(`include/imgdata.h`, `utils/imgdata.c`)

### Data structure

画像データは`include/imgdata.h`中の`Imgdata`構造体で扱う
（HWC形式、各色要素8bit、RGBオーダ）

```c
typedef struct {
    int     width;      // 幅
    int     height;     // 高さ
    int     channel;    // チャネル数
    uint8_t *data;      // 画素データ配列 (HWC/RGB)
} Imgdata;
```

### Data allocation/deallocation

指定サイズ・チャネル数での画像データ生成

```c
// 640x480, 3チャネル画像の生成
Imgdata *img = Imgdata_alloc(640, 480, 3);

// 破棄
Imgdata_free(img);
```

### Data access

座標(x,y)（左上原点）における画素へのアクセス

```c
// 座標(x,y)での色要素を取得
uint8_t r = Imgdata_at(img, x, y)[0]; // R
uint8_t g = Imgdata_at(img, x, y)[1]; // G
uint8_t b = Imgdata_at(img, x, y)[2]; // B

// (x,y)のR要素を平均値で上書き
Imgdata_at(img, x, y)[0] = (r + g + b) / 3;
```

### File I/O

PNG画像の入出力I/F

```c
// PNG画像入力
Imgdata *img = Imgdata_read_png("./input.png");

// PNG画像出力
Imgdata_write_png(img, "./output.png");
```

サンプル（`example/example.c`）はターゲット`example`の指定でビルド可

```sh
$ mkdir build
$ cmake ..
$ cmake --build . --target example
```
