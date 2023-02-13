#include <math.h>

#include "imgdata.h"

void gamma_correction(Imgdata *img, Imgdata *corrected, const double c, const double g)
{
    const int size = img->height * img->width * img->channel;
    double coef = 1 / c;
    double exponent = 1 / g;
    for (int i = 0; i < size; i++) {
        double norm = (double)img->data[i] / 255;
        int value = pow((coef * norm), exponent) * 255;
        corrected->data[i] = Imgdata_sat_u8(value);
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256_gamma.png");

    Imgdata *img_corrected = Imgdata_alloc(img->width, img->height, img->channel);

    gamma_correction(img, img_corrected, 1, 2.2);

    Imgdata_write_png(img_corrected, "./024_corrected.png");

    Imgdata_free(&img);
    Imgdata_free(&img_corrected);

    return 0;
}
