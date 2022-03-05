#include <stdio.h>
#include <math.h>

#include "imgdata.h"

#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))

void rgb_to_hsv(Imgdata *rgb, Imgdata *hsv)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            uint8_t r = Imgdata_at(rgb, x, y)[0];
            uint8_t g = Imgdata_at(rgb, x, y)[1];
            uint8_t b = Imgdata_at(rgb, x, y)[2];

            uint8_t min = MIN(r, MIN(g, b));
            uint8_t max = MAX(r, MAX(g, b));

            // value
            float v = max;

            // saturation
            float s = max - min;

            // hue
            float h = (min == max) ? 0 :
                      // B is minimum
                      (min == b) ? (60.0f * ((g - r) / s) + 60) :
                      // R is minimum
                      (min == r) ? (60.0f * ((b - g) / s) + 180) :
                      // G is minimum
                      (60.0f * ((r - b) / s) + 300) ;

            IMGDATA_AT(hsv, float, x, y)[0] = h;
            IMGDATA_AT(hsv, float, x, y)[1] = s;
            IMGDATA_AT(hsv, float, x, y)[2] = v;
        }
    }
}

void hsv_to_rgb(Imgdata *hsv, Imgdata *rgb)
{
    for (int y = 0; y < hsv->height; y++) {
        for (int x = 0; x < hsv->width; x++) {
            float h = IMGDATA_AT(hsv, float, x, y)[0];
            float s = IMGDATA_AT(hsv, float, x, y)[1];
            float v = IMGDATA_AT(hsv, float, x, y)[2];

            float h_ = h / 60.0f;
            float x_ = s * (1 - fabs(fmod(h_, 2) - 1));

            if (h_ < 1) { // h_ >= 0
                Imgdata_at(rgb, x, y)[0] = (uint8_t)((v - s) + s);
                Imgdata_at(rgb, x, y)[1] = (uint8_t)((v - s) + x_);
                Imgdata_at(rgb, x, y)[2] = (uint8_t)(v - s);
            } else if (h_ < 2) {
                Imgdata_at(rgb, x, y)[0] = (uint8_t)((v - s) + x_);
                Imgdata_at(rgb, x, y)[1] = (uint8_t)((v - s) + s);
                Imgdata_at(rgb, x, y)[2] = (uint8_t)(v - s);
            } else if (h_ < 3) {
                Imgdata_at(rgb, x, y)[0] = (uint8_t)(v - s);
                Imgdata_at(rgb, x, y)[1] = (uint8_t)((v - s) + s);
                Imgdata_at(rgb, x, y)[2] = (uint8_t)((v - s) + x_);
            } else if (h_ < 4) {
                Imgdata_at(rgb, x, y)[0] = (uint8_t)(v - s);
                Imgdata_at(rgb, x, y)[1] = (uint8_t)((v - s) + x_);
                Imgdata_at(rgb, x, y)[2] = (uint8_t)((v - s) + s);
            } else if (h_ < 5) {
                Imgdata_at(rgb, x, y)[0] = (uint8_t)((v - s) + x_);
                Imgdata_at(rgb, x, y)[1] = (uint8_t)(v - s);
                Imgdata_at(rgb, x, y)[2] = (uint8_t)((v - s) + s);
            } else { // h_ < 6
                Imgdata_at(rgb, x, y)[0] = (uint8_t)((v - s) + s);
                Imgdata_at(rgb, x, y)[1] = (uint8_t)(v - s);
                Imgdata_at(rgb, x, y)[2] = (uint8_t)((v - s) + x_);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png(argv[1]);

    // store HSV image with float for calculation
    Imgdata *img_hsv = Imgdata_alloc(img->width, img->height, 3, IMGDATA_DEPTH_F32);

    // RGB to HSV
    rgb_to_hsv(img, img_hsv);

    // reverse hue
    for (int y = 0; y < img_hsv->height; y++) {
        for (int x = 0; x < img_hsv->width; x++) {
            float h = IMGDATA_AT(img_hsv, float, x, y)[0];
            IMGDATA_AT(img_hsv, float, x, y)[0] = fmod((h + 180), 360);
        }
    }

    // return HSV to RGB
    Imgdata *img_ret_rgb = Imgdata_alloc(img->width, img->height, img->channel, IMGDATA_DEPTH_U8);

    // HSV to RGB
    hsv_to_rgb(img_hsv, img_ret_rgb);

    Imgdata_write_png(img_ret_rgb, "./005_h_rev.png");

    Imgdata_free(&img);
    Imgdata_free(&img_hsv);
    Imgdata_free(&img_ret_rgb);

    return 0;
}
