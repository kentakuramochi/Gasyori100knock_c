#include <stdio.h>

#include "imgdata.h"

void binarize(Imgdata *rgb, Imgdata *bin, int th)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            int gray = 0.2126 * Imgdata_at(rgb, x, y)[0]
                     + 0.7152 * Imgdata_at(rgb, x, y)[1]
                     + 0.0722 * Imgdata_at(rgb, x, y)[2];
            Imgdata_at(bin, x, y)[0] = ((gray < th) ? 0 : 255);
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_bin = Imgdata_alloc(img->width, img->height, 1, IMGDATA_DEPTH_U8);
    binarize(img, img_bin, 127);
    Imgdata_write_png(img_bin, "./003_bin.png");

    Imgdata_free(&img);
    Imgdata_free(&img_bin);

    return 0;
}
