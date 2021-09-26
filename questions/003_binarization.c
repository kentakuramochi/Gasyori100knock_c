#include <stdio.h>

#include "imgdata.h"

Imgdata *binarize(Imgdata *img, int th)
{
    Imgdata *bin = Imgdata_alloc(img->width, img->height, 1);

    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            uint8_t r = Imgdata_at(img, x, y)[0];
            uint8_t g = Imgdata_at(img, x, y)[1];
            uint8_t b = Imgdata_at(img, x, y)[2];

            uint8_t gray = (uint8_t)(0.2126 * r + 0.7152 * g + 0.0722 * b);

            Imgdata_at(bin, x, y)[0] = ((gray < th) ? 0 : 255);
        }
    }

    return bin;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_bin = binarize(img, 127);

    Imgdata_write_png(img_bin, "./003_bin.png");

    Imgdata_free(img);
    Imgdata_free(img_bin);

    return 0;
}
