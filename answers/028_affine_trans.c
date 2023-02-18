#include "imgdata.h"

void affine(Imgdata *img, Imgdata *affine, const int tx, const int ty)
{
    int a_ = 1;
    int b_ = 0;
    int c_ = 0;
    int d_ = 1;
    double coef = 1.0 / (a_ * d_ - b_ * c_);
    for (int y = 0; y < affine->height; y++) {
        for (int x = 0; x < affine->width; x++) {
            int bx = coef * (d_ * x - b_ * y) - tx;
            int by = coef * (-c_ * x + a_ * y) - ty;
            for (int c = 0; c < affine->channel; c++) {
                if ((bx >= 0) && (bx < img->width) &&
                    (by >= 0) && (by < img->height)) {
                    Imgdata_at(affine, x, y)[c] = Imgdata_at(img, bx, by)[c];
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    Imgdata *img_affine = Imgdata_alloc(img->width, img->height, img->channel);

    affine(img, img_affine, 30, -30);

    Imgdata_write_png(img_affine, "./028_affine.png");

    Imgdata_free(&img);
    Imgdata_free(&img_affine);

    return 0;
}
