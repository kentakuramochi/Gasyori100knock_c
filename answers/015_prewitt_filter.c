#include <stdio.h>
#include <stdlib.h>

#include "imgdata.h"

void rgb2gray(Imgdata *rgb, Imgdata *gray)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            Imgdata_at(gray, x, y)[0] = 0.2126 * Imgdata_at(rgb, x, y)[0]
                                      + 0.7152 * Imgdata_at(rgb, x, y)[1]
                                      + 0.0722 * Imgdata_at(rgb, x, y)[2];
        }
    }
}

void prewitt_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh, const bool diff_x)
{
    // create kernel
    int *k = malloc(sizeof(int) * kw * kh);
    int k_idx = 0;
    for (int i = 0; i < kh; i++) {
        for (int j = 0; j < kw; j++) {
            k[k_idx] = 0;

            if (!diff_x) {
                if (i == 0) {
                    k[k_idx] = -1;
                } else if (i == (kh - 1)) {
                    k[k_idx] = 1;
                }
            } else {
                if (j == 0) {
                    k[k_idx] = -1;
                } else if (j == (kw - 1)) {
                    k[k_idx] = 1;
                }
            }

            k_idx++;
        }
    }

    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {

                int val = 0;

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

                        val += Imgdata_at(img, p_x, p_y)[c] * k[i * kw + j];
                    }
                }

                Imgdata_at(filtered, x, y)[c] = (val > 0) ? val : 0;
            }
        }
    }

    free(k);
    k = NULL;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *gray = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY, IMGDATA_DEPTH_U8);
    rgb2gray(img, gray);

    Imgdata *img_prewitt_x_k5 = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY, IMGDATA_DEPTH_U8);
    Imgdata *img_prewitt_y_k5 = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY, IMGDATA_DEPTH_U8);

    prewitt_filter(gray, img_prewitt_x_k5, 5, 5, true);
    prewitt_filter(gray, img_prewitt_y_k5, 5, 5, false);

    Imgdata_write_png(img_prewitt_x_k5, "./015_prewitt_x_k5.png");
    Imgdata_write_png(img_prewitt_y_k5, "./015_prewitt_y_k5.png");

    Imgdata_free(&img);
    Imgdata_free(&gray);
    Imgdata_free(&img_prewitt_x_k5);
    Imgdata_free(&img_prewitt_y_k5);

    return 0;
}
