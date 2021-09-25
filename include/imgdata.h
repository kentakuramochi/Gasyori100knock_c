#ifndef IMGDATA_H
#define IMGDATA_H

#include <stdint.h>
#include <stdbool.h>

// image data strucute
typedef struct {
    int     width;
    int     height;
    int     channel;
    uint8_t *data;
} Imgdata;

// num of color channels
typedef enum {
    IMGDATA_COLOR_GRAY  = 1,
    IMGDATA_COLOR_RGB   = 3,
    IMGDATA_COLOR_RGBA  = 4 
} IMGDATA_COLOR;

// get reference of pixel data at (x,y)
static inline uint8_t *Imgdata_at(Imgdata *img, const int x, const int y)
{
    return (img->data + y * img->width * img->channel + x * img->channel);
}

// allocate Imgdata with specified size
Imgdata *Imgdata_alloc(const int width, const int height, const int channel);

// free Imgdata
void Imgdata_free(Imgdata *img);

// read PNG image file as Imgdata
Imgdata *Imgdata_read_png(const char *filename);

// write Imgdata as PNG image file
bool Imgdata_write_png(const Imgdata *img, const char *filename);

#endif // IMGDATA_H
