#include <stdio.h>

#include "imgdata.h"

int main(void)
{
    // read PNG image as Imgdata structure
    Imgdata *img = Imgdata_read_png("../dataset/images/imori_256x256.png");
    if (img == NULL)
    {
        return -1;
    }

    // scan left half area of image
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < (img->width / 2); x++)
        {
            // get (R,G,B) value and calc average
            uint8_t r = Imgdata_at(img, x, y)[0];
            uint8_t g = Imgdata_at(img, x, y)[1];
            uint8_t b = Imgdata_at(img, x, y)[2];
            uint8_t avg = (r + g + b) / 3;

            // update pixel data by avg
            Imgdata_at(img, x, y)[0] = avg;
            Imgdata_at(img, x, y)[1] = avg;
            Imgdata_at(img, x, y)[2] = avg;
        }
    }

    // write Imgdata as PNG image
    if (!Imgdata_write_png(img, "./output.png"))
    {
        Imgdata_free(img);
        return -1;
    }

    Imgdata_free(img);

    return 0;
}
