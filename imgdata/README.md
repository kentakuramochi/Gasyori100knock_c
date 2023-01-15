# Imgdata

## Description

PNG画像処理のための簡易ライブラリ。

## Data structure

構造体`Imgdata`で画像データを扱う。ピクセルデータは1次元配列として保持する
（HWCオーダ）。

```c
// image data strucute
typedef struct Imgdata {
    int width;              // image width
    int height;             // image height
    int channel;            // num of channels
    int stride;             // image stride in pixel
    int32_t *data;          // pointer to raw data
} Imgdata;
```

## Data allocation/deallocation

```c
// 640x480, 3チャネル, 8bit 画像の生成
Imgdata *img = Imgdata_alloc(640, 480, 3);

// 破棄
Imgdata_free(&img);
```

YUV等の画像処理のため、各画素値は32bit整数値で保持している。

## Data access

```c
// 座標(x,y)での色要素を取得
uint8_t r = Imgdata_at(img, x, y)[0]; // R
uint8_t g = Imgdata_at(img, x, y)[1]; // G
uint8_t b = Imgdata_at(img, x, y)[2]; // B
```

## File I/O

PNG画像の入出力のみをサポートする（符号なし8bitデータのみ）。

```c
// PNG画像入力
Imgdata *img = Imgdata_read_png("./input.png");

// PNG画像出力
Imgdata_write_png(img, "./output.png");
```

その他、再利用性のある処理は適宜追加する。
