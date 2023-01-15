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

void laplacian_filter(Imgdata *img,  Imgdata *filtered)
{
    // create kernel
    int k[3 * 3] = { 0 };
    k[1] = 1;
    k[3] = 1;
    k[4] = -4;
    k[5] = 1;
    k[7] = 1;

    // set start position include padding
    const int k_size = 3;
    const int s_k = -(k_size / 2);

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

                for (int i = 0; i < k_size; i++) {
                    int p_y = y + s_k + i;
                    // thougnt outer pixel as zero padding
                    if ((p_y < 0) || (p_y > (img->height - 1))) {
                        continue;
                    }
                    for (int j = 0; j < k_size; j++) {
                        int p_x = x + s_k + j;
                        // thougnt outer pixel as zero padding
                        if ((p_x < 0) || (p_x > (img->width - 1))) {
                            continue;
                        }

                        val += Imgdata_at(img, p_x, p_y)[c] * k[i * k_size + j];
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

    Imgdata *img_lapl = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY);
    laplacian_filter(gray, img_lapl);
    Imgdata_write_png(img_lapl, "./017_lapl.png");

    Imgdata_free(&img);
    Imgdata_free(&gray);
    Imgdata_free(&img_lapl);

    return 0;
}
