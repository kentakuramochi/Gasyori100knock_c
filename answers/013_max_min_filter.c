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

void max_min_filter(Imgdata *img,  Imgdata *filtered, const int kw, const int kh)
{
    // set start position include padding
    const int s_kx = -(kw / 2);
    const int s_ky = -(kh / 2);

    // filtering
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {

                int min = INT8_MAX;
                int max = -INT8_MAX;

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

                        // renew min/max
                        int val = Imgdata_at(img, p_x, p_y)[c];
                        if (min > val) {
                            min = val;
                        }
                        if (max < val) {
                            max = val;
                        }
                    }
                }

                // get diff of min and max
                Imgdata_at(filtered, x, y)[c] = (max - min);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *gray = Imgdata_alloc(img->width, img->height, 1, IMGDATA_DEPTH_U8);
    rgb2gray(img, gray);

    Imgdata *img_max_min_k5 = Imgdata_alloc(img->width, img->height, 1, IMGDATA_DEPTH_U8);

    max_min_filter(gray, img_max_min_k5, 5, 5);

    Imgdata_write_png(img_max_min_k5, "./013_max_min_k5.png");

    Imgdata_free(&img);
    Imgdata_free(&gray);
    Imgdata_free(&img_max_min_k5);

    return 0;
}
