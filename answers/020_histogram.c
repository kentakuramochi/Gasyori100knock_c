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

#define NUM_BINS 256

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256_dark.png");

    int histogram[NUM_BINS];

    get_histogram(img, histogram, NUM_BINS);

    // Write a histogram as CSV file
    FILE *fp = fopen("./020_hist.csv", "w");
    for (int i = 0; i < NUM_BINS; i++) {
        fprintf(fp, "%d,%u\n", i, histogram[i]);
    }
    fclose(fp);

    Imgdata_free(&img);

    return 0;
}
