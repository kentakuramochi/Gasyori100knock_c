#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "imgdata.h"

// compare function for qsort
static int compare_int(const void *a, const void *b)
{
    int ia = *(int*)a;
    int ib = *(int*)b;

    return (ia - ib);
}

void median_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh)
{
    // create kernel
    const int ksize = kh * kw;
    int *k = malloc(sizeof(int) * ksize);
    bool ksize_is_even = (ksize % 2 == 0) ? true : false;
    const int mid_idx = ksize / 2;

    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {
                // get values in kernel
                int idx = 0;
                for (int i = 0; i < kh; i++) {
                    int p_y = y + s_ky + i;
                    for (int j = 0; j < kw; j++) {
                        int p_x = x + s_kx + j;
                        // thougnt outer pixel as zero padding
                        if ((p_y < 0) || (p_y > (img->height - 1)) ||
                            (p_x < 0) || (p_x > (img->width - 1))) {
                            k[idx] = 0;
                        } else {
                            k[idx] = Imgdata_at(img, p_x, p_y)[c];
                        }
                        idx++;
                    }
                }

                // sort and get median
                qsort(k, ksize, sizeof(int), compare_int);
                int median = (ksize_is_even) ? ((k[mid_idx - 1] + k[mid_idx]) / 2) :
                                               k[mid_idx];

                Imgdata_at(filtered, x, y)[c] = median;
            }
        }
    }

    free(k);
    k = NULL;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_median_k3 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    median_filter(img, img_median_k3, 3, 3);
    Imgdata_write_png(img_median_k3, "./010_median_k3.png");

    Imgdata *img_median_k7 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    median_filter(img, img_median_k7, 7, 7);
    Imgdata_write_png(img_median_k7, "./010_median_k7.png");

    Imgdata_free(&img);
    Imgdata_free(&img_median_k3);
    Imgdata_free(&img_median_k7);

    return 0;
}
