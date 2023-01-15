#include <stdio.h>

#include "imgdata.h"

void max_pooling(Imgdata *img,  Imgdata *pool, const int kw, const int kh)
{
    // get kernel start position include padding
    // outer pixels
    const int outer_w = img->width % kw;
    const int outer_h = img->height % kh;
    // halve size of outer pixels and thought it as start/end padding
    const int s_kx = -(outer_w / 2);
    const int s_ky = -(outer_h / 2);

    const int fsize = kw * kh;

    for (int y = 0; y < img->height; y += kh) {
        for (int x = 0; x < img->width; x += kw) {
            for (int c = 0; c < img->channel; c++ ) {
                // get max in kernel
                int max = -INT32_MAX;
                for (int i = 0; i < kh; i++) {
                    int p_y = y + s_ky + i;
                    // skip outer pixel (zero padding)
                    if ((p_y < 0) || (p_y > (img->height - 1))) {
                        continue;
                    }
                    for (int j = 0; j < kw; j++) {
                        int p_x = x + s_kx + j;
                        // skip outer pixel (zero padding)
                        if ((p_x < 0) || (p_x > (img->width - 1))) {
                            continue;
                        }
                        int p = Imgdata_at(img, p_x, p_y)[c];
                        if (max < p) {
                            max = p;
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
                        Imgdata_at(pool, p_x, p_y)[c] = max;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *img_maxpool8 = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_U8);
    max_pooling(img, img_maxpool8, 8, 8);
    Imgdata_write_png(img_maxpool8, "./008_max_k8.png");

    Imgdata_free(&img);
    Imgdata_free(&img_maxpool8);

    return 0;
}
