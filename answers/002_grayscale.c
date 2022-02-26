#include <stdio.h>

#include "imgdata.h"

void rgb2gray(Imgdata *rgb, Imgdata *gray)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            Imgdata_at(gray, x, y)[0] = (uint8_t)(
                  0.2126 * Imgdata_at(rgb, x, y)[0]
                + 0.7152 * Imgdata_at(rgb, x, y)[1]
                + 0.0722 * Imgdata_at(rgb, x, y)[2]);
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);
    Imgdata *img_gray = Imgdata_alloc(img->width, img->height, 1, IMGDATA_DEPTH_U8);

    rgb2gray(img, img_gray);

    Imgdata_write_png(img_gray, "./002_gray.png");

    Imgdata_free(&img);
    Imgdata_free(&img_gray);

    return 0;
}
