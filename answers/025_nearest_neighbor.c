#include <math.h>

#include "imgdata.h"

Imgdata *nearest_neighbor(Imgdata *img, const double scale_x, const double scale_y)
{
    Imgdata *out = Imgdata_alloc((img->width * scale_x), (img->height * scale_y), img->channel);

    for (int y = 0; y < out->height; y++) {
        for (int x = 0; x < out->width; x++) {
            for (int c = 0; c < out->channel; c++) {
                int p_x = round(x / scale_x);
                int p_y = round(y / scale_y);
                Imgdata_at(out, x, y)[c] = Imgdata_at(img, p_x, p_y)[c];
            }
        }
    }

    return out;
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    Imgdata *img_nearest_neighbor = nearest_neighbor(img, 1.5, 1.5);

    Imgdata_write_png(img_nearest_neighbor, "./025_nearest_neighbor.png");

    Imgdata_free(&img);
    Imgdata_free(&img_nearest_neighbor);

    return 0;
}
