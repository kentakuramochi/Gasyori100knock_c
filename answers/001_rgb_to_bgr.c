#include <stdio.h>

#include "imgdata.h"

void rgb2bgr(Imgdata *rgb, Imgdata *bgr)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            Imgdata_at(bgr, x, y)[0] = Imgdata_at(rgb, x, y)[2];
            Imgdata_at(bgr, x, y)[1] = Imgdata_at(rgb, x, y)[1];
            Imgdata_at(bgr, x, y)[2] = Imgdata_at(rgb, x, y)[0];
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *img_bgr = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    rgb2bgr(img, img_bgr);
    Imgdata_write_png(img_bgr, "./001_bgr.png");

    Imgdata_free(&img);
    Imgdata_free(&img_bgr);

    return 0;
}
