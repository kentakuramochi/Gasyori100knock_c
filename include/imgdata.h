#ifndef IMGDATA_H
#define IMGDATA_H

#include <stdint.h>
#include <stdbool.h>

// image data strucute
typedef struct {
    int width;
    int height;
    int channel;
    uint8_t *data;
} Imgdata;

// get reference of pixel data at (x,y)
static inline uint8_t *Imgdata_at(Imgdata *img, int x, int y)
{
    return (img->data + y * img->width * img->channel + x * img->channel);
}

// allocate Imgdata
Imgdata *Imgdata_alloc(const int width, const int height, const int channel);

// free Imgdata
void Imgdata_free(Imgdata *img);

// input PNG image as Imgdata
Imgdata *Imgdata_read_png(const char *filename);

// output Imgdata as PNG image
bool Imgdata_write_png(const Imgdata *img, const char *filename);

#endif // IMGDATA_H
