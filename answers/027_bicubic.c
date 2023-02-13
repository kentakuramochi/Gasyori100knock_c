#include <math.h>

#include "imgdata.h"

// Weight value
static double h(const double t)
{
    const double a = -1;
    if (t <= 1) {
        return (a + 2) * t * t * t - (a + 3) * t * t + 1;
    } else if (t <= 2) {
        return a * t * t * t - 5 * a * t * t + 8 * a * t - 4 * a;
    }

    return 0;
}

#define LIMIT(value, min, max) ((value > max) ? max : (value < min) ? min : value)

void bicubic(Imgdata *img, Imgdata *interpolated, const double scale_x, const double scale_y)
{
    for (int y = 0; y < interpolated->height; y++) {
        for (int x = 0; x < interpolated->width; x++) {
            for (int c = 0; c < interpolated->channel; c++) {
                int px = floor(x / scale_x);
                int py = floor(y / scale_y);

                double val = 0;
                double sum = 0;
                for (int j = -1; j < 3; j++) {
                    for (int i = -1; i < 3; i++) {
                        double dx = fabs(x / scale_x - (px + i));
                        double dy = fabs(y / scale_y - (py + j));
                        double hx = h(dx);
                        double hy = h(dy);
                        int ix = LIMIT((px + i), 0, (img->width - 1));
                        int jy = LIMIT((py + j), 0, (img->height - 1));
                        val += hx * hy * Imgdata_at(img, ix, jy)[c];
                        sum += hx * hy;
                    }
                }

                if (sum != 0) {
                    val /= sum;
                }

                Imgdata_at(interpolated, x, y)[c] = Imgdata_sat_u8((int)val);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_128x128.png");

    double scale = 1.5;
    Imgdata *img_bicubic = Imgdata_alloc((img->width * scale), (img->height * scale), img->channel);

    bicubic(img, img_bicubic, scale, scale);

    Imgdata_write_png(img_bicubic, "./027_bicubic.png");

    Imgdata_free(&img);
    Imgdata_free(&img_bicubic);

    return 0;
}
