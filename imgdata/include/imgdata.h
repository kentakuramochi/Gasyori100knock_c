#ifndef IMGDATA_H
#define IMGDATA_H

#include <stdint.h>
#include <stdbool.h>

// Image data strucute with (H,W,C) format
typedef struct Imgdata {
    int width;      // Image width in pixel
    int height;     // Image height in pixel
    int channel;    // Num of channels
    int stride;     // Image stride in pixel
    int32_t *data;  // Pointer to raw data
} Imgdata;

// Num of color channels
static const int IMGDATA_NCH_BIN  = 1;
static const int IMGDATA_NCH_GRAY = 1;
static const int IMGDATA_NCH_RGB  = 3;
static const int IMGDATA_NCH_YUV  = 3;
static const int IMGDATA_NCH_RGBA = 4;

// Saturate the value with a range of unsigned 8bit integer [0, 255]
static inline uint8_t Imgdata_sat_u8(const int value)
{
    return (value > UINT8_MAX) ? UINT8_MAX :
           (value < 0) ? 0 :
           value;
}

// Get pointer to pixel data at (x,y)
static inline int32_t *Imgdata_at(Imgdata *img, const int x, const int y)
{
    return (img->data + y * img->stride + x * img->channel);
}

// Allocate Imgdata
Imgdata *Imgdata_alloc(const int width, const int height, const int channel);

// Free Imgdata
void Imgdata_free(Imgdata **img);

// Read PNG image file as Imgdata
Imgdata *Imgdata_read_png(const char *filename);

// Write Imgdata as PNG image file
bool Imgdata_write_png(const Imgdata *img, const char *filename);

#endif // IMGDATA_H
