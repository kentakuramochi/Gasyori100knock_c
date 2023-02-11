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
    printf("\n");
}

void transform_hist(Imgdata *img, Imgdata *trans_hist, const int mean, const int std)
{
    const int size = img->height * img->width * img->channel;
    double m = 0;
    for (int i = 0; i < size; i++) {
        m += img->data[i];
    }
    m /= size;

    double var = 0;
    for (int i = 0; i < size; i++) {
        int pix = img->data[i];
        var += (pix - m) * (pix - m);
    }
    var /= size;

    double s = sqrt(var);
    double scale = std / s;
    for (int i = 0; i < size; i++) {
        int trans = scale * (img->data[i] - m) + mean;
        trans_hist->data[i] = Imgdata_sat_u8(trans);
    }
}

#define NUM_BINS 256

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256_dark.png");

    Imgdata *img_trans_hist = Imgdata_alloc(img->width, img->height, img->channel);

    transform_hist(img, img_trans_hist, 128, 52);

    int histogram[NUM_BINS];
    get_histogram(img_trans_hist, histogram, NUM_BINS);

    Imgdata_write_png(img_trans_hist, "./022_trans.png");

    // Write a histogram as CSV file
    FILE *fp = fopen("./022_hist.csv", "w");
    for (int i = 0; i < NUM_BINS; i++) {
        fprintf(fp, "%d,%u\n", i, histogram[i]);
    }
    fclose(fp);

    Imgdata_free(&img);
    Imgdata_free(&img_trans_hist);

    return 0;
}
