#include <stdio.h>

#include "imgdata.h"

void color_subtraction(Imgdata *img,  Imgdata *sub, const int threshold)
{
    int th = 256 / threshold;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            Imgdata_at(sub, x, y)[0] = Imgdata_at(img, x, y)[0] / th * th + th / 2;
            Imgdata_at(sub, x, y)[1] = Imgdata_at(img, x, y)[1] / th * th + th / 2;
            Imgdata_at(sub, x, y)[2] = Imgdata_at(img, x, y)[2] / th * th + th / 2;
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_sub4 = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    color_subtraction(img, img_sub4, 4);

    Imgdata *img_sub8 = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    color_subtraction(img, img_sub8, 8);

    Imgdata_write_png(img_sub4, "./006_sub_4.png");
    Imgdata_write_png(img_sub8, "./006_sub_8.png");

    Imgdata_free(&img);
    Imgdata_free(&img_sub4);
    Imgdata_free(&img_sub8);

    return 0;
}
