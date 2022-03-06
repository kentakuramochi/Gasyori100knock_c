#include <stdio.h>

#include "imgdata.h"

void max_pooling(Imgdata *img,  Imgdata *pool, const int kw, const int kh)
{
    // get kernel start position include padding
    const int outer_w = img->width % kw;
    const int outer_h = img->height % kh;
    const int s_kx = -(outer_w / 2);
    const int s_ky = -(outer_h / 2);

    const int fsize = kw * kh;

    for (int y = 0; y < img->height; y += kh) {
        for (int x = 0; x < img->width; x += kw) {
            // get max in kernel
            uint8_t max_r = 0;
            uint8_t max_g = 0;
            uint8_t max_b = 0;

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

                    uint8_t r = Imgdata_at(img, p_x, p_y)[0];
                    uint8_t g = Imgdata_at(img, p_x, p_y)[1];
                    uint8_t b = Imgdata_at(img, p_x, p_y)[2];

                    if (max_r < r) {
                        max_r = r;
                    }
                    if (max_g < g) {
                        max_g = g;
                    }
                    if (max_b < b) {
                        max_b = b;
                    }
                }
            }

            // set max
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

                    Imgdata_at(pool, p_x, p_y)[0] = max_r;
                    Imgdata_at(pool, p_x, p_y)[1] = max_g;
                    Imgdata_at(pool, p_x, p_y)[2] = max_b;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_maxpool8 = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    max_pooling(img, img_maxpool8, 8, 8);

    Imgdata_write_png(img_maxpool8, "./007_max_k8.png");

    Imgdata_free(&img);
    Imgdata_free(&img_maxpool8);

    return 0;
}
