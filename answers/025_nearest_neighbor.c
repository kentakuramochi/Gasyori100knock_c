#include <math.h>

#include "imgdata.h"

void nearest_neighbor(Imgdata *img, Imgdata *interpolated, const double scale_x, const double scale_y)
{
    for (int y = 0; y < interpolated->height; y++) {
        for (int x = 0; x < interpolated->width; x++) {
            for (int c = 0; c < interpolated->channel; c++) {
                int p_x = round(x / scale_x);
                int p_y = round(y / scale_y);
                Imgdata_at(interpolated, x, y)[c] = Imgdata_at(img, p_x, p_y)[c];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    double scale = 1.5;
    Imgdata *img_nearest_neighbor = Imgdata_alloc((img->width * scale), (img->height * scale), img->channel);

    nearest_neighbor(img, img_nearest_neighbor, scale, scale);

    Imgdata_write_png(img_nearest_neighbor, "./025_nearest_neighbor.png");

    Imgdata_free(&img);
    Imgdata_free(&img_nearest_neighbor);

    return 0;
}
