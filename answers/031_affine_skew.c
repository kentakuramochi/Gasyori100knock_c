#include <math.h>
#include <stdio.h>

#include "imgdata.h"

void affine(Imgdata *img, Imgdata *affine,
            const int tx, const int ty,
            const double scale_x, const double scale_y,
            const double theta,
            const int dx, const int dy)
{
    double a_, b_, c_, d_;
    double coef;
    int t_x, t_y;

    a_ = scale_x;
    b_ = 0;
    c_ = 0;
    d_ = scale_y;

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
        coef = 1.0 / (a_ * d_ - b_ * c_);

        if (dx != 0) {
            b_ = (double)dx / img->height;
        }
        if (dy != 0) {
            c_ = (double)dy / img->width;
        }

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
                } else {
                    Imgdata_at(affine, x, y)[c] = 0;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    int dx = 30;
    int dy = 30;
    int skew_width = img->width + dx;
    int skew_height = img->height + dy;
    Imgdata *img_affine_1 = Imgdata_alloc(skew_width, img->height, img->channel);
    Imgdata *img_affine_2 = Imgdata_alloc(img->width, skew_height, img->channel);
    Imgdata *img_affine_3 = Imgdata_alloc(skew_width, skew_height, img->channel);

    affine(img, img_affine_1, 0, 0, 1, 1, 0, dx, 0);
    affine(img, img_affine_2, 0, 0, 1, 1, 0, 0, dy);
    affine(img, img_affine_3, 0, 0, 1, 1, 0, dx, dy);

    Imgdata_write_png(img_affine_1, "./031_affine_1.png");
    Imgdata_write_png(img_affine_2, "./031_affine_2.png");
    Imgdata_write_png(img_affine_3, "./031_affine_3.png");

    Imgdata_free(&img);
    Imgdata_free(&img_affine_1);
    Imgdata_free(&img_affine_2);
    Imgdata_free(&img_affine_3);

    return 0;
}
