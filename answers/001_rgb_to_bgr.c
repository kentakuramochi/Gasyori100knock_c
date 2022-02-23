#include <stdio.h>

#include "imgdata.h"

Imgdata *rgb2bgr(Imgdata *img)
{
    Imgdata *bgr = Imgdata_alloc(img->width, img->height, img->channel);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t r = Imgdata_at(img, x, y)[0];
            uint8_t g = Imgdata_at(img, x, y)[1];
            uint8_t b = Imgdata_at(img, x, y)[2];

            Imgdata_at(bgr, x, y)[0] = b;
            Imgdata_at(bgr, x, y)[1] = g;
            Imgdata_at(bgr, x, y)[2] = r;
        }
    }

    return bgr;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_bgr = rgb2bgr(img);

    Imgdata_write_png(img_bgr, "./001_bgr.png");

    Imgdata_free(img);
    Imgdata_free(img_bgr);

    return 0;
}
