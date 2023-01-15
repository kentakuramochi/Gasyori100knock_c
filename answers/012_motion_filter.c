#include <stdio.h>
#include <stdlib.h>

#include "imgdata.h"

void motion_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh)
{
    // create kernel
    if (kw != kh) {
        return;
    }
    double *k = malloc(sizeof(double) * kw * kh);
    for (int i = 0; i < kh; i++) {
        for (int j = 0; j < kw; j++) {
            k[i * kw + j] = (i == j) ? (1.0 / kw) : 0;
        }
    }

    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {

                double f_val = 0;
                for (int i = 0; i < kh; i++) {
                    int p_y = y + s_ky + i;
                    // thougnt outer pixel as zero padding
                    if ((p_y < 0) || (p_y > (img->height - 1))) {
                        continue;
                    }
                    for (int j = 0; j < kw; j++) {
                        int p_x = x + s_kx + j;
                        // thougnt outer pixel as zero padding
                        if ((p_x < 0) || (p_x > (img->width - 1))) {
                            continue;
                        }

                        f_val += Imgdata_at(img, p_x, p_y)[c] * k[i * kw + j];
                    }
                }

                Imgdata_at(filtered, x, y)[c] = (uint8_t)f_val;
            }
        }
    }

    free(k);
    k = NULL;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *img_motion_k5 = Imgdata_alloc(img->width, img->height, img->channel);
    motion_filter(img, img_motion_k5, 5, 5);
    Imgdata_write_png(img_motion_k5, "./012_motion_k5.png");

    Imgdata_free(&img);
    Imgdata_free(&img_motion_k5);

    return 0;
}
