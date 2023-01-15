#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "imgdata.h"

void gaussian_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh, const double sigma)
{
    // create gaussian kernel
    double *k = malloc(sizeof(double) * kh * kw);
    const double PI = 3.141592;
    double k_sum = 0;
    // set values from gauss distribution
    for (int y = 0; y < kh; y++) {
        int p_ky = y - (kh / 2);
        for (int x = 0; x < kw; x++) {
            int p_kx = x - (kw / 2);
            double kval = (1.0 / (2 * PI * sigma * sigma)) * exp(-(p_kx * p_kx + p_ky * p_ky) / (2 * sigma * sigma));
            k[y * kw + x] = kval;
            k_sum += kval;
        }
    }
    // normalize filter values
    for (int y = 0; y < kh; y++) {
        for (int x = 0; x < kw; x++) {
            k[y * kw + x] /= k_sum;
        }
    }

    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {
                double sum = 0;
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
                        sum += Imgdata_at(img, p_x, p_y)[c] * k[i * kw + j];
                    }
                }
                Imgdata_at(filtered, x, y)[c] = sum;
            }
        }
    }

    free(k);
    k = NULL;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256_noise.png");

    Imgdata *img_gauss_k3 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    gaussian_filter(img, img_gauss_k3, 3, 3, 1.3);
    Imgdata_write_png(img_gauss_k3, "./009_gauss_k3.png");

    Imgdata *img_gauss_k7 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    gaussian_filter(img, img_gauss_k7, 7, 7, 1.3);
    Imgdata_write_png(img_gauss_k7, "./009_gauss_k7.png");

    Imgdata *img_gauss_k7s3 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    gaussian_filter(img, img_gauss_k7s3, 7, 7, 3);
    Imgdata_write_png(img_gauss_k7s3, "./009_gauss_k7s3.png");

    Imgdata_free(&img);
    Imgdata_free(&img_gauss_k3);
    Imgdata_free(&img_gauss_k7);
    Imgdata_free(&img_gauss_k7s3);

    return 0;
}
