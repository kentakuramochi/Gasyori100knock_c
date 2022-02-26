# Gasyori100knock_c

## Description

「画像処理100本ノック」(
[yoyoyo-yo/Gasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock) 
) をC言語で実装する

## Prerequesites

- CMake > 3.10.2

- [libpng](http://www.libpng.org/pub/png/libpng.html)

    - [sourceforge](https://sourceforge.net/projects/libpng/files/)
    - aptでのインストール: `sudo apt install libpng-dev`

## Setup

スクリプト`setup.sh`で環境をセットアップする

- サンプル画像のダウンロード（[Gasyori100knockのリポジトリ](https://github.com/yoyoyo-yo/Gasyori100knock.git) からクローン、`Gasyori100knock/`に配置）
- libpngのダウンロード（aptを使用）

## Imgdata library

画像データを扱うための簡易的なライブラリ`imgdata`を作成している（ソース: `imgdata/`）

### Data structure

構造体`Imgdata`で画像データを扱う。ピクセルデータは1次元配列として保持する
（HWC形式）

```c
// data type for byte unit
typedef uint8_t Byte;

// image data strucute
typedef struct Imgdata {
    int width;              // image width
    int height;             // image height
    int channel;            // num of channels
    int bpp;                // byte per pixel
    int stride;             // image stride in bytes
    IMGDATA_DEPTH depth;    // bit depth
    Byte *data;             // pointer to raw data
} Imgdata;
```


### Data allocation/deallocation

画像データの生成と破棄

```c
// 640x480, 3チャネル, 8bit 画像の生成
Imgdata *img = Imgdata_alloc(640, 480, 3, IMGDATA_DEPTH_U8);

// 破棄
Imgdata_free(&img);
```

生成時、1チャネル当たりのビット深度をパラメータで指定する

```c
// bit depth for Imgdata
typedef enum IMGDATA_DEPTH {
    IMGDATA_DEPTH_INVALID = 0,
    IMGDATA_DEPTH_S8 = 1,   // signed 8bit
    IMGDATA_DEPTH_U8 = 1,   // unsigned 8bit
    IMGDATA_DEPTH_S16 = 2,  // unsigned 16bit
    IMGDATA_DEPTH_U16 = 2,  // unsigned 16bit
    IMGDATA_DEPTH_S32 = 4,  // unsigned 32bit
    IMGDATA_DEPTH_U32 = 4,  // unsigned 32bit
    IMGDATA_DEPTH_F32 = 4,  // float 32bit
    IMGDATA_DEPTH_F64 = 8,  // double presicion float 64bit
} IMGDATA_DEPTH;
```

### Data access

画像中の指定位置(x, y)にアクセスする。
各チャネル要素へのアクセスはインデックスを指定する

```c
// 座標(x,y)での色要素を取得
uint8_t r = Imgdata_at(img, x, y)[0]; // R
uint8_t g = Imgdata_at(img, x, y)[1]; // G
uint8_t b = Imgdata_at(img, x, y)[2]; // B
```

Byte要素の配列として保持するため、ビット深度に応じてキャストが必要になる。
型キャストのためのマクロが使用可能

```c
// int (32bit) 単位での要素アクセス
int i = ((int*)Imgdata_at(img, x, y))[0]; 

// 指定型での要素アクセス
i = IMGDATA_AT(img, int, x, y)[0];
```


### File I/O

PNG画像の入出力をサポートする（符号なし8bitデータのみ）

```c
// PNG画像入力
Imgdata *img = Imgdata_read_png("./input.png");

// PNG画像出力
Imgdata_write_png(img, "./output.png");
```

再利用性のある処理は適宜追加する

### Example

`--target example` の指定でサンプルソース（`example/example.c`）をビルドする

```sh
$ cmake --build . --target example
```

## Answer

各問題に対する回答ソースを`answers/`以下に格納する。
デフォルトのターゲットとしてビルドされる

```sh
# @project root
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

スクリプト`./run_all_answers.sh` で全回答プログラムを一括実行する
（プログラム、出力画像をそれぞれ `build/answers/`, `build/answers/out/` 以下に格納する）
