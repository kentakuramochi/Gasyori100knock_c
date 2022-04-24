#include <stdio.h>

#include "imgdata.h"

void binarize_otsu(Imgdata *rgb, Imgdata *bin)
{
    // get grayscale and min/max
    Imgdata *gray = Imgdata_alloc(rgb->width, rgb->height, 1, IMGDATA_DEPTH_U8);

    uint8_t min = 255;
    uint8_t max = 0;

    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            int p = ( 0.2126 * Imgdata_at(rgb, x, y)[0]
                    + 0.7152 * Imgdata_at(rgb, x, y)[1]
                    + 0.0722 * Imgdata_at(rgb, x, y)[2]);

            if (p < min) {
                min = p;
            }
            if (p > max) {
                max = p;
            }

            Imgdata_at(gray, x, y)[0] = p;
        }
    }

    const int size = gray->width * gray->height;

    // max inter-class variance
    long double max_icvar = -1;

    // search best threshold
    uint8_t best_th = 0;

    for (int i = min; i < max; i++) {
        int th = i;

        // num of pix class 0/1
        int w0 = 0;
        int w1 = 0;

        // mean of class 0/1
        double m0 = 0;
        double m1 = 0;

        // get num of pix and mean of class 0/1
        for (int j = 0; j < size; j++) {
            uint8_t p = gray->data[j];
            if (p <= th) {
                w0++;
                m0 += p;
            } else {
                w1++;
                m1 += p;
            }
        }
        m0 = (w0 > 0) ? (m0 / w0) : 0;
        m1 = (w1 > 0) ? (m1 / w1) : 0;

        // if inter-class variance is maximum, current threshold is the best
        // inter-class variance
        // ((w0 * w1)/((w0 + w1) * (w0 + w1))) * (m0 - m1) * (m0 - m1)
        // -> simplify to
        // (w0 / (w0 + w1)) * (w1 / (w0 + w1)) * (m0 - m1) * (m0 - m1)
        int n = w0 + w1;
        double r0 = (double)w0 / n;
        double r1 = (double)w1 / n;
        double m_diff = m0 - m1;
        double icvar = (r0 * r1) * m_diff * m_diff;

        if (icvar > max_icvar) {
            max_icvar = icvar;
            best_th = th;
        }
    }

    printf("threshold >> %u\n", best_th);

    // binarization
    for (int y = 0; y < gray->height; y++) {
        for (int x = 0; x < gray->width; x++) {
            uint8_t p = Imgdata_at(gray, x, y)[0];
            Imgdata_at(bin, x, y)[0] = ((p <= best_th) ? 0 : 255);
        }
    }

    Imgdata_free(&gray);
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_bin = Imgdata_alloc(img->width, img->height, 1, IMGDATA_DEPTH_U8);
    binarize_otsu(img, img_bin);
    Imgdata_write_png(img_bin, "./004_bin.png");

    Imgdata_free(&img);
    Imgdata_free(&img_bin);

    return 0;
}
