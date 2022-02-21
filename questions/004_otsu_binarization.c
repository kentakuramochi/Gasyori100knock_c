#include <stdio.h>

#include "imgdata.h"

Imgdata *binarize_otsu(Imgdata *img)
{
    Imgdata *bin = Imgdata_alloc(img->width, img->height, 1);

    // max inter-class variance
    long double max_icvar = -1;

    // decide best threshold
    int best_th = 0;
    for (int i = 1; i < 255; i++)
    {
        int th = i;

        // num of pix class 0/1
        int w0 = 0;
        int w1 = 0;
        // mean of class 0/1
        double m0 = 0;
        double m1 = 0;

        // get num of pix and mean of class 0/1
        for (int y = 0; y < img->height; y++)
        {
            for (int x = 0; x < img->width; x++)
            {
                uint8_t r = Imgdata_at(img, x, y)[0];
                uint8_t g = Imgdata_at(img, x, y)[1];
                uint8_t b = Imgdata_at(img, x, y)[2];
                uint8_t gray = (uint8_t)(0.2126 * r + 0.7152 * g + 0.0722 * b);

                if (gray <= th)
                {
                    w0++;
                    m0 += gray;
                }
                else
                {
                    w1++;
                    m1 += gray;
                }
            }
        }

        if (w0 > 0)
        {
            m0 /= w0;
        }
        if (w1 > 0)
        {
            m1 /= w1;
        }

        // if inter-class variance is maximum, current threshold is the best
        int64_t d = w0 * w1;
        int64_t n = (w0 + w1) * (w0 + w1);
        long double icvar = ((long double)d / n) * (m0 - m1) * (m0 - m1);
        if (icvar > max_icvar)
        {
            max_icvar = icvar;
            best_th = th;
        }
    }

    printf("threshold >> %d\n", best_th);

    // binarization
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            uint8_t r = Imgdata_at(img, x, y)[0];
            uint8_t g = Imgdata_at(img, x, y)[1];
            uint8_t b = Imgdata_at(img, x, y)[2];
            uint8_t gray = (uint8_t)(0.2126 * r + 0.7152 * g + 0.0722 * b);
            Imgdata_at(bin, x, y)[0] = ((gray <= best_th) ? 0 : 255);
        }
    }

    return bin;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    Imgdata *img_bin = binarize_otsu(img);

    Imgdata_write_png(img_bin, "./004_bin.png");

    Imgdata_free(img);
    Imgdata_free(img_bin);

    return 0;
}
