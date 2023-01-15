#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "imgdata.h"

void log_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh, const double sigma)
{
    // create kernel
    double *k = malloc(sizeof(double) * kw * kh);
    const double PI = 3.141592;
    double k_sum = 0;
    for (int y = 0; y < kh; y++) {
        int p_ky = y - (kh / 2);
        for (int x = 0; x < kw; x++) {
            int p_kx = x - (kw / 2);
            double kval = (((p_kx * p_kx + p_ky * p_ky - 2 * sigma * sigma) / (2 * PI * pow(sigma, 6)))
                           * exp(-(p_kx * p_kx + p_ky * p_ky) / (2 * sigma * sigma)));
            k[y * kw + x] = kval;
            k_sum += kval;
        }
    }
    // normalize kernel
    for (int y = 0; y < kh; y++) {
        for (int x = 0; x < kw; x++) {
            k[y * kw + x] /= k_sum;
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
    Imgdata *img = Imgdata_read_png("./imori_256x256_noise.png");

    Imgdata *img_log_k5 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    log_filter(img, img_log_k5, 5, 5, 3);
    Imgdata_write_png(img_log_k5, "./019_log_k5.png");

    Imgdata *img_log_k12 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    log_filter(img, img_log_k12, 12, 12, 3);
    Imgdata_write_png(img_log_k12, "./019_log_k12.png");

    Imgdata *img_log_k9 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    log_filter(img, img_log_k9, 9, 9, 9);
    Imgdata_write_png(img_log_k9, "./019_log_k9.png");

    Imgdata_free(&img);
    Imgdata_free(&img_log_k5);
    Imgdata_free(&img_log_k12);
    Imgdata_free(&img_log_k9);

    return 0;
}
