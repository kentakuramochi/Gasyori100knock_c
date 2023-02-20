#include "imgdata.h"

Imgdata *affine(Imgdata *img, const int tx, const int ty)
{
    Imgdata *out = Imgdata_alloc(img->width, img->height, img->channel);

    int a_ = 1;
    int b_ = 0;
    int c_ = 0;
    int d_ = 1;
    double coef = 1.0 / (a_ * d_ - b_ * c_);
    for (int y = 0; y < out->height; y++) {
        for (int x = 0; x < out->width; x++) {
            int bx = coef * (d_ * x - b_ * y) - tx;
            int by = coef * (-c_ * x + a_ * y) - ty;
            for (int c = 0; c < out->channel; c++) {
                if ((bx >= 0) && (bx < img->width) &&
                    (by >= 0) && (by < img->height)) {
                    Imgdata_at(out, x, y)[c] = Imgdata_at(img, bx, by)[c];
                }
            }
        }
    }

    return out;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    Imgdata *img_affine = affine(img, 30, -30);

    Imgdata_write_png(img_affine, "./028_affine.png");

    Imgdata_free(&img);
    Imgdata_free(&img_affine);

    return 0;
}
