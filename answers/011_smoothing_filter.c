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

void smoothing_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh)
{
    // create kernel
    const int ksize = kh * kw;
    int *k = malloc(sizeof(int) * ksize);

    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {
                // get average in kernel
                double avg = 0;
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

                        avg += Imgdata_at(img, p_x, p_y)[c];
                    }
                }
                avg /= ksize;

                Imgdata_at(filtered, x, y)[c] = avg;
            }
        }
    }

    free(k);
    k = NULL;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_smooth_k5 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    smoothing_filter(img, img_smooth_k5, 5, 5);
    Imgdata_write_png(img_smooth_k5, "./010_smooth_k5.png");

    Imgdata *img_smooth_k9 = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);
    smoothing_filter(img, img_smooth_k9, 9, 9);
    Imgdata_write_png(img_smooth_k9, "./010_smooth_k9.png");

    Imgdata_free(&img);
    Imgdata_free(&img_smooth_k5);
    Imgdata_free(&img_smooth_k9);

    return 0;
}
