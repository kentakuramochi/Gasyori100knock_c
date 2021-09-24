# Gasyori100knock_c

## Description

[yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock)

「画像処理100本ノック」をC言語で挑戦してみる。

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

```sh
# @project root
$ mkdir build
$ cd build
$ cmake ../
$ cmake --build .
```

## Image operation

簡易的な画像データ構造とその操作を、ライブラリで提供する
(`build/utils/libimgdata.a` 。ヘッダ：`include/imgdata.h`, ソース：`utils/imgdata.c`)

（サンプル：`example/example.c`）

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
