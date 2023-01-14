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
    int stride;             // image stride in bytes
    IMGDATA_DEPTH depth;    // bit depth per 1 element
    int32_t *data;          // pointer to raw data
} Imgdata;
```

## Data allocation/deallocation

```c
// 640x480, 3チャネル, 8bit 画像の生成
Imgdata *img = Imgdata_alloc(640, 480, 3, IMGDATA_DEPTH_U8);

// 破棄
Imgdata_free(&img);
```

生成時に1チャネル当たりのビット深度をパラメータで指定する
（YUV処理のため内部的には32bit整数値で保持しており、現状利用していない）。

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
