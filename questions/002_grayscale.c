#include <stdio.h>

#include "imgdata.h"

Imgdata *rgb2gray(Imgdata *img)
{
    Imgdata *gray = Imgdata_alloc(img->width, img->height, IMGDATA_COLOR_GRAY);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t r = Imgdata_at(img, x, y)[0];
            uint8_t g = Imgdata_at(img, x, y)[1];
            uint8_t b = Imgdata_at(img, x, y)[2];

            Imgdata_at(gray, x, y)[0] = (uint8_t)(0.2126 * r + 0.7152 * g + 0.0722 * b);
        }
    }

    return gray;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_gray = rgb2gray(img);

    Imgdata_write_png(img_gray, "./002_gray.png");

    Imgdata_free(img);
    Imgdata_free(img_gray);

    return 0;
}
