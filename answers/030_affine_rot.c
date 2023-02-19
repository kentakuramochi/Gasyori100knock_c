#include <math.h>

#include "imgdata.h"

void affine(Imgdata *img, Imgdata *affine,
            const int tx, const int ty,
            const double scale_x, const double scale_y,
            const double theta)
{
    double a_, b_, c_, d_;
    double coef;
    int t_x, t_y;
    if (theta != 0) {
        double rad = 3.141592 / 180 * theta;
        a_ = cos(rad);
        b_ = -sin(rad);
        c_ = sin(rad);
        d_ = cos(rad);
        coef = 1.0 / (a_ * d_ - b_ * c_);

        int c_x = img->width / 2;
        int c_y = img->height / 2;
        int new_c_x = coef * (d_ * c_x - b_ * c_y);
        int new_c_y = coef * (-c_ * c_x + a_ * c_y);
        t_x = new_c_x - c_x;
        t_y = new_c_y - c_y;
    } else {
        a_ = scale_x;
        b_ = 0;
        c_ = 0;
        d_ = scale_y;
        coef = 1.0 / (a_ * d_ - b_ * c_);

        t_x = tx;
        t_y = ty;
    }

    for (int y = 0; y < affine->height; y++) {
        for (int x = 0; x < affine->width; x++) {
            int bx = coef * (d_ * x - b_ * y) - t_x;
            int by = coef * (-c_ * x + a_ * y) - t_y;
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

    affine(img, img_affine, 0, 0, 1, 1, -30);

    Imgdata_write_png(img_affine, "./030_affine.png");

    Imgdata_free(&img);
    Imgdata_free(&img_affine);

    return 0;
}
