#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "imgdata.h"

void rgb2gray(Imgdata *rgb, Imgdata *gray)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            int val = 0.2126 * Imgdata_at(rgb, x, y)[0]
                      + 0.7152 * Imgdata_at(rgb, x, y)[1]
                      + 0.0722 * Imgdata_at(rgb, x, y)[2];
            Imgdata_at(gray, x, y)[0] = Imgdata_sat_u8(val);
        }
    }
}

const double PI = 3.1415926535;

void dft(Imgdata *gray, double complex *g)
{
    const int w = gray->width;
    const int h = gray->height;

    for (int l = 0; l < h; l++) {
        for (int k = 0; k < w; k++) {
            double complex p = CMPLX(0, 0);
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    double theta = -2 * PI * ((double)(k * x) / w + (double)(l * y) / h);
                    p += ((double)*Imgdata_at(gray, x, y) * (cos(theta) + I * sin(theta)));
                }
            }
            g[l * w + k] = p / sqrt((double)h * w);
        }
    }
}

void idft(double complex* g, Imgdata *gray)
{
    const int w = gray->width;
    const int h = gray->height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double complex p = CMPLX(0, 0);
            for (int l = 0; l < h; l++) {
                for (int k = 0; k < w; k++) {
                    double theta = 2 * PI * ((double)(k * x) / w + (double)(l * y) / h);
                    p += (g[l * w + k] * (cos(theta) + I * sin(theta)));
                }
            }
            *Imgdata_at(gray, x, y) = (int)(cabs(p) / sqrt((double)w * h));
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    Imgdata *img_gray = Imgdata_alloc(img->width, img->height, IMGDATA_NCH_GRAY);
    rgb2gray(img, img_gray);

    double complex *g = malloc(sizeof(double complex) * img->height * img->width);
    dft(img_gray, g);

    Imgdata *img_ps = Imgdata_alloc(img_gray->width, img_gray->height, img_gray->channel);
    // Get max for scaling of the power spectrum
    double g_max = -DBL_MAX;
    for (int l = 0; l < img_ps->height; l++) {
        for (int k = 0; k < img_ps->width; k++) {
            double g_abs = cabs(g[l * img_ps->width+ k]);
            if (g_abs > g_max) {
                g_max = g_abs;
            }
        }
    }
    for (int l = 0; l < img_ps->height; l++) {
        for (int k = 0; k < img_ps->width; k++) {
            double gabs = cabs(g[l * img_ps->width+ k]);
            // Scaling the spectrum to [0, 255]
            *Imgdata_at(img_ps, k, l) = Imgdata_sat_u8((int)(gabs / g_max * 255));
        }
    }

    Imgdata_write_png(img_ps, "./032_ps.png");

    Imgdata *img_idft = Imgdata_alloc(img_gray->width, img_gray->height, img_gray->channel);
    idft(g, img_idft);

    Imgdata_write_png(img_idft, "./032_idft.png");

    Imgdata_free(&img);
    Imgdata_free(&img_gray);
    free(g);
    g = NULL;
    Imgdata_free(&img_ps);
    Imgdata_free(&img_idft);

    return 0;
}
