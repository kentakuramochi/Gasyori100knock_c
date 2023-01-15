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


void sobel_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh, const bool diff_x)
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

            // x2 center weight
            if (i == (kh / 2)) {
                k[k_idx] *= 2;
            }
            if (j == (kw / 2)) {
                k[k_idx] *= 2;
            }

            k_idx++;
        }
    }

    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // max/min
    int *maxs = malloc(sizeof(int) * img->channel);
    int *mins = malloc(sizeof(int) * img->channel);
    for (int i = 0; i < img->channel; i++) {
        maxs[i] = -INT32_MAX;
        mins[i] = INT32_MAX;
    }

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

                if (val > maxs[c]) {
                    maxs[c] = val;
                }
                if (val < mins[c]) {
                    mins[c] = val;
                }

                Imgdata_at(filtered, x, y)[c] = val;
            }
        }
    }

    // normaize with max/min
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {
                int val = Imgdata_at(filtered, x, y)[c];
                Imgdata_at(filtered, x, y)[c] = 255 * ((double)(val - mins[c]) / (maxs[c] - mins[c]));
            }
        }
    }

    free(k);
    k = NULL;
    free(maxs);
    maxs = NULL;
    free(mins);
    mins = NULL;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *gray = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY);
    rgb2gray(img, gray);

    Imgdata *img_sobel_x_k3 = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY);
    Imgdata *img_sobel_y_k3 = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY);

    sobel_filter(gray, img_sobel_x_k3, 3, 3, true);
    sobel_filter(gray, img_sobel_y_k3, 3, 3, false);

    Imgdata_write_png(img_sobel_x_k3, "./016_sobel_x_k3.png");
    Imgdata_write_png(img_sobel_y_k3, "./016_sobel_y_k3.png");

    Imgdata_free(&img);
    Imgdata_free(&gray);
    Imgdata_free(&img_sobel_x_k3);
    Imgdata_free(&img_sobel_y_k3);

    return 0;
}
