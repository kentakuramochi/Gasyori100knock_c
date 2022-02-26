#ifndef IMGDATA_H
#define IMGDATA_H

#include <stdint.h>
#include <stdbool.h>

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

// data type for byte unit
typedef uint8_t Byte;

// image data strucute
typedef struct Imgdata {
    int width;              // image width
    int height;             // image height
    int channel;            // num of channels
    int bpp;                // byte per pixel
    int stride;             // image stride in bytes
    IMGDATA_DEPTH depth;    // bit depth per 1 channel
    Byte *data;             // pointer to raw data
} Imgdata;

// num of color channels
#define IMG_CH_BIN  1
#define IMG_CH_GRAY 1
#define IMG_CH_RGB  3
#define IMG_CH_RGB  3
//#define IMG_CH_RGBA 4

// get pointer to pixel data at (x,y)
static inline Byte *Imgdata_at(Imgdata *img, const int x, const int y)
{
    return (img->data + y * img->stride + x * img->bpp);
}

// get pointer to pixel data at (x,y) with specified data type
#define IMGDATA_AT(img, dtype, x, y) ((dtype*)(Imgdata_at((img), (x), (y))))

// allocate Imgdata
Imgdata *Imgdata_alloc(const int width, const int height, const int channel, const IMGDATA_DEPTH depth);

// free Imgdata
void Imgdata_free(Imgdata **img);

// read PNG image file as Imgdata
Imgdata *Imgdata_read_png(const char *filename);

// write Imgdata as PNG image file
bool Imgdata_write_png(const Imgdata *img, const char *filename);

#endif // IMGDATA_H
