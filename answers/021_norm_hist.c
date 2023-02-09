#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "imgdata.h"

void get_histogram(Imgdata *img, int *histogram, const int num_bins)
{
    if ((num_bins > (UINT8_MAX + 1)) || (num_bins < 1)) {
        return;
    }

    // Initialize histogram
    for (int i = 0; i < num_bins; i++) {
        histogram[i] = 0;
    }

    // Calculate the step of histogram
    double step = (double)(UINT8_MAX + 1) / num_bins;

    // Count bins
    const int size = img->width * img->height * img->channel;
    for (int i = 0; i < size; i++) {
        int bin = (int)(img->data[i] / step);
        histogram[bin]++;
    }
}

void normalize_hist(Imgdata *img, Imgdata *norm_hist, const int range_min, const int range_max)
{
    int max = INT32_MIN;
    int min = INT32_MAX;

    const int size = img->height * img->width * img->channel;
    for (int i = 0; i < size; i++) {
        int pixel = img->data[i];
        if (pixel > max) {
            max = pixel;
        }
        if (pixel < min) {
            min = pixel;
        }
    }

    double scale = (double)(range_max - range_min) / (max - min);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            for (int c = 0; c < img->channel; c++) {
                int pix = Imgdata_at(img, x, y)[c];
                if (pix < min) {
                    Imgdata_at(norm_hist, x, y)[c] = range_min;
                } else if (pix < max) {
                    Imgdata_at(norm_hist, x, y)[c] = scale * (pix - min) + range_min;
                } else {
                    Imgdata_at(norm_hist, x, y)[c] = range_max;
                }
            }
        }
    }
}

#define NUM_BINS 256

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256_dark.png");

    Imgdata *img_norm_hist = Imgdata_alloc(img->width, img->height, img->channel);

    normalize_hist(img, img_norm_hist, 0, 255);

    int histogram[NUM_BINS];
    get_histogram(img_norm_hist, histogram, NUM_BINS);

    Imgdata_write_png(img_norm_hist, "./021_norm.png");

    // Write a histogram as CSV file
    FILE *fp = fopen("./021_hist.csv", "w");
    for (int i = 0; i < NUM_BINS; i++) {
        fprintf(fp, "%d,%u\n", i, histogram[i]);
    }
    fclose(fp);

    Imgdata_free(&img);
    Imgdata_free(&img_norm_hist);

    return 0;
}
