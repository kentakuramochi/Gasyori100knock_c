#include <stdio.h>

#include "imgdata.h"

void average_pooling(Imgdata *img,  Imgdata *pool, const int kw, const int kh)
{
    // get kernel start position include padding
    const int outer_w = img->width % kw;
    const int outer_h = img->height % kh;
    const int s_kx = -(outer_w / 2);
    const int s_ky = -(outer_h / 2);

    const int fsize = kw * kh;

    for (int y = 0; y < img->height; y += kh) {
        for (int x = 0; x < img->width; x += kw) {
            // get average in kernel
            float sum_r = 0.0f;
            float sum_g = 0.0f;
            float sum_b = 0.0f;

            for (int i = 0; i < kh; i++) {
                int p_y = y + s_ky + i;
                if ((p_y < 0) || (p_y > (img->height - 1))) {
                    continue;
                }

                for (int j = 0; j < kw; j++) {
                    int p_x = x + s_kx + j;
                    if ((p_x < 0) || (p_x > (img->width - 1))) {
                        continue;
                    }

                    sum_r += Imgdata_at(img, p_x, p_y)[0];
                    sum_g += Imgdata_at(img, p_x, p_y)[1];
                    sum_b += Imgdata_at(img, p_x, p_y)[2];
                }
            }

            uint8_t avg_r = sum_r / fsize;
            uint8_t avg_g = sum_g / fsize;
            uint8_t avg_b = sum_b / fsize;

            // set average
            for (int i = 0; i < kh; i++) {
                int p_y = y + s_ky + i;
                if ((p_y < 0) || (p_y > (img->height - 1))) {
                    continue;
                }

                for (int j = 0; j < kw; j++) {
                    int p_x = x + s_kx + j;
                    if ((p_x < 0) || (p_x > (img->width - 1))) {
                        continue;
                    }

                    Imgdata_at(pool, p_x, p_y)[0] = avg_r;
                    Imgdata_at(pool, p_x, p_y)[1] = avg_g;
                    Imgdata_at(pool, p_x, p_y)[2] = avg_b;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_avgpool8 = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    average_pooling(img, img_avgpool8, 8, 8);

    Imgdata_write_png(img_avgpool8, "./007_avg_k8.png");

    Imgdata *img_avgpool16 = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    average_pooling(img, img_avgpool16, 16, 16);

    Imgdata_write_png(img_avgpool16, "./007_avg_k16.png");

    Imgdata_free(&img);
    Imgdata_free(&img_avgpool8);
    Imgdata_free(&img_avgpool16);

    return 0;
}
