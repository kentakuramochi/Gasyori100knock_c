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

void differential_filter(Imgdata *img,  Imgdata *filtered, const bool diff_x)
{
    // create kernel (3x3)
    const int k_size = 3;
    int k[3 * 3] = { 0 };
    // set kernel values
    if (diff_x) {
        k[3] = -1;
        k[4] = 1;
    } else {
        k[1] = -1;
        k[4] = 1;
    }

    // set start position include padding
    const int s_kx = -(k_size / 2);
    const int s_ky = -(k_size / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {

                int val = 0;

                for (int i = 0; i < k_size; i++) {
                    int p_y = y + s_ky + i;
                    // thougnt outer pixel as zero padding
                    if ((p_y < 0) || (p_y > (img->height - 1))) {
                        continue;
                    }
                    for (int j = 0; j < k_size; j++) {
                        int p_x = x + s_kx + j;
                        // thougnt outer pixel as zero padding
                        if ((p_x < 0) || (p_x > (img->width - 1))) {
                            continue;
                        }

                        val += Imgdata_at(img, p_x, p_y)[c] * k[i * k_size + j];
                        //printf("%d ", val);
                    }
                }
                //printf("\n");
                //getchar();

                Imgdata_at(filtered, x, y)[c] = (val > 0) ? val : 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *gray = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY, IMGDATA_DEPTH_U8);
    rgb2gray(img, gray);

    Imgdata *img_diff_x = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY, IMGDATA_DEPTH_U8);
    Imgdata *img_diff_y = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY, IMGDATA_DEPTH_U8);
    differential_filter(gray, img_diff_x, true);
    differential_filter(gray, img_diff_y, false);

    Imgdata_write_png(img_diff_x, "./014_diff_x.png");
    Imgdata_write_png(img_diff_y, "./014_diff_y.png");

    Imgdata_free(&img);
    Imgdata_free(&gray);
    Imgdata_free(&img_diff_x);
    Imgdata_free(&img_diff_y);

    return 0;
}
