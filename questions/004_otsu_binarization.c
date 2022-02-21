#include <stdio.h>

#include "imgdata.h"

Imgdata *binarize_otsu(Imgdata *img)
{
    // get grayscale
    Imgdata *gray = Imgdata_alloc(img->width, img->height, 1);
    uint8_t min = 255;
    uint8_t max = 0;
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
       {
            uint8_t r = Imgdata_at(img, x, y)[0];
            uint8_t g = Imgdata_at(img, x, y)[1];
            uint8_t b = Imgdata_at(img, x, y)[2];
            uint8_t i = (uint8_t)(0.2126 * r + 0.7152 * g + 0.0722 * b);

            if (i < min)
            {
                min = i;
            }
            if (i > max)
            {
                max = i;
            }

            Imgdata_at(gray, x, y)[0] = i;
        }
    }

    const int size = gray->width * gray->height;

    // max inter-class variance
    long double max_icvar = -1;

    // search best threshold
    uint8_t best_th = 0;
    for (int i = min; i < max; i++)
    {
        int th = i;

        // num of pix class 0/1
        int w0 = 0;
        int w1 = 0;
        // mean of class 0/1
        double m0 = 0;
        double m1 = 0;

        // get num of pix and mean of class 0/1
        for (int j = 0; j < size; j++)
        {
            uint8_t p = gray->data[j];
            if (p <= th)
            {
                w0++;
                m0 += p;
            }
            else
            {
                w1++;
                m1 += p;
            }
        }

        m0 = (w0 > 0) ? (m0 / w0) : 0;
        m1 = (w1 > 0) ? (m1 / w1) : 0;

        // if inter-class variance is maximum, current threshold is the best
        // need to consider overflow
        // e.g.) 256x256: (w0+w1)=65536, 655360^2=4.3x10^9 > UINT32_MAX (2^32-1)
        int64_t w_prod = w0 * w1;
        int64_t n_sq = (int64_t)(w0 + w1) * (w0 + w1);
        double m_diff = m0 - m1;
        double icvar = ((double)w_prod / n_sq) * m_diff * m_diff;
        if (icvar > max_icvar)
        {
            max_icvar = icvar;
            best_th = th;
        }
    }

    printf("threshold >> %d\n", best_th);

    // binarization
    Imgdata *bin = Imgdata_alloc(img->width, img->height, 1);
    for (int i = 0; i < size; i++)
    {
        uint8_t p = gray->data[i];
        bin->data[i] = ((p <= best_th) ? 0 : 255);
    }

    Imgdata_free(gray);

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
