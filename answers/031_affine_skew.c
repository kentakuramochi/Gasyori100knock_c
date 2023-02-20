#include <math.h>
#include <stdio.h>

#include "imgdata.h"

Imgdata *affine(Imgdata *img,
                const double scale_x, const double scale_y,
                const int tx, const int ty,
                const double theta,
                const int dx, const int dy)
{
    double a_, b_, c_, d_;
    double coef;
    int t_x, t_y;
    int out_width, out_height;

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

        out_width = img->width + dx;
        out_height = img->height + dy;
    } else {
        if (dx != 0) {
            b_ = (double)dx / img->height;
        }
        if (dy != 0) {
            c_ = (double)dy / img->width;
        }
        coef = 1.0 / (a_ * d_ - b_ * c_);

        t_x = tx;
        t_y = ty;

        out_width = img->width * scale_x + dx;
        out_height = img->height * scale_y + dy;
    }

    Imgdata *out = Imgdata_alloc(out_width, out_height, img->channel);

    for (int y = 0; y < out->height; y++) {
        for (int x = 0; x < out->width; x++) {
            int bx = coef * (d_ * x - b_ * y) - t_x;
            int by = coef * (-c_ * x + a_ * y) - t_y;
            for (int c = 0; c < out->channel; c++) {
                if ((bx >= 0) && (bx < img->width) &&
                    (by >= 0) && (by < img->height)) {
                    Imgdata_at(out, x, y)[c] = Imgdata_at(img, bx, by)[c];
                } else {
                    Imgdata_at(out, x, y)[c] = 0;
                }
            }
        }
    }

    return out;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    Imgdata *img_affine_1 = affine(img, 1, 1, 0, 0, 0, 30, 0);
    Imgdata *img_affine_2 = affine(img, 1, 1, 0, 0, 0, 0, 30);
    Imgdata *img_affine_3 = affine(img, 1, 1, 0, 0, 0, 30, 30);

    Imgdata_write_png(img_affine_1, "./031_affine_1.png");
    Imgdata_write_png(img_affine_2, "./031_affine_2.png");
    Imgdata_write_png(img_affine_3, "./031_affine_3.png");

    Imgdata_free(&img);
    Imgdata_free(&img_affine_1);
    Imgdata_free(&img_affine_2);
    Imgdata_free(&img_affine_3);

    return 0;
}
