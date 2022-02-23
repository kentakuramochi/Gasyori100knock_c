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

構造体`Imgdata`で画像データを扱う
（HWC形式、8bit深度、RGBオーダ）

```c
typedef struct {
    int     width;      // 幅
    int     height;     // 高さ
    int     channel;    // チャネル数
    uint8_t *data;      // 画素データ配列 (HWC/RGB)
} Imgdata;
```

### Data allocation/deallocation

```c
// 640x480, 3チャネル画像の生成
Imgdata *img = Imgdata_alloc(640, 480, 3);

// 破棄
Imgdata_free(img);
```

### Data access

```c
// 座標(x,y)での色要素を取得
uint8_t r = Imgdata_at(img, x, y)[0]; // R
uint8_t g = Imgdata_at(img, x, y)[1]; // G
uint8_t b = Imgdata_at(img, x, y)[2]; // B

// (x,y)のR要素を平均値で上書き
Imgdata_at(img, x, y)[0] = (r + g + b) / 3;
```

### File I/O

PNG画像の入出力をサポートする

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
