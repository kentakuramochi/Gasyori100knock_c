#include "imgdata.h"

Imgdata *affine(Imgdata *img, const double scale_x, const double scale_y, const int tx, const int ty)
{
    Imgdata *out = Imgdata_alloc((img->width * scale_x), (img->height * scale_y), img->channel);

    double a_ = scale_x;
    double b_ = 0;
    double c_ = 0;
    double d_ = scale_y;
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

    Imgdata *img_affine_1 = affine(img, 1.3, 0.8, 0, 0);
    Imgdata *img_affine_2 = affine(img, 1.3, 0.8, 30, -30);

    Imgdata_write_png(img_affine_1, "./029_affine_1.png");
    Imgdata_write_png(img_affine_2, "./029_affine_2.png");

    Imgdata_free(&img);
    Imgdata_free(&img_affine_1);
    Imgdata_free(&img_affine_2);

    return 0;
}
