#include <stdio.h>
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

#define NUM_BINS 256

void equalize_hist(Imgdata *img, Imgdata *equal_hist, const int mean, const int std)
{
    const int size = img->height * img->width * img->channel;
    int max = -UINT8_MAX;
    for (int i = 0; i < size; i++) {
        if (img->data[i] > max) {
            max = img->data[i];
        }
    }

    int histogram[NUM_BINS];
    get_histogram(img, histogram, NUM_BINS);

    for (int i = 0; i < size; i++) {
        int pix = img->data[i];
        int sum_h = 0;
        for (int j = 0; j < pix; j++) {
            sum_h += histogram[j];
        }

        equal_hist->data[i] = (double)max / size * sum_h;
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    Imgdata *img_equal_hist = Imgdata_alloc(img->width, img->height, img->channel);

    equalize_hist(img, img_equal_hist, 128, 52);

    int histogram[NUM_BINS];
    get_histogram(img_equal_hist, histogram, NUM_BINS);

    Imgdata_write_png(img_equal_hist, "./023_equal.png");

    // Write a histogram as CSV file
    FILE *fp = fopen("./023_hist.csv", "w");
    for (int i = 0; i < NUM_BINS; i++) {
        fprintf(fp, "%d,%u\n", i, histogram[i]);
    }
    fclose(fp);

    Imgdata_free(&img);
    Imgdata_free(&img_equal_hist);

    return 0;
}
