#ifndef IMGDATA_H
#define IMGDATA_H

#include <stdint.h>
#include <stdbool.h>

// image data strucute
typedef struct Imgdata {
    int width;              // image width
    int height;             // image height
    int channel;            // num of channels
    int stride;             // image stride in pixel
    int32_t *data;          // pointer to raw data
} Imgdata;

// num of color channels
#define IMGDATA_NCH_BIN  1
#define IMGDATA_NCH_GRAY 1
#define IMGDATA_NCH_RGB  3
#define IMGDATA_NCH_YUV  3
#define IMGDATA_NCH_RGBA 4

// get pointer to pixel data at (x,y)
static inline int32_t *Imgdata_at(Imgdata *img, const int x, const int y)
{
    return (img->data + y * (img->width * img->channel) + x * img->channel);
}

// allocate Imgdata
Imgdata *Imgdata_alloc(const int width, const int height, const int channel);

// free Imgdata
void Imgdata_free(Imgdata **img);

// read PNG image file as Imgdata
Imgdata *Imgdata_read_png(const char *filename);

// write Imgdata as PNG image file
bool Imgdata_write_png(const Imgdata *img, const char *filename);

#endif // IMGDATA_H
