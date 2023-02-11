#include <stdio.h>
#include <math.h>

#include "imgdata.h"

#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))

void rgb_to_hsv(Imgdata *rgb, Imgdata *hsv)
{
    for (int y = 0; y < rgb->height; y++) {
        for (int x = 0; x < rgb->width; x++) {
            int r = Imgdata_at(rgb, x, y)[0];
            int g = Imgdata_at(rgb, x, y)[1];
            int b = Imgdata_at(rgb, x, y)[2];

            int min = r;
            if (g < min) {
                min = g;
            }
            if (b < min) {
                min = b;
            }

            int max = r;
            if (g > max) {
                max = g;
            }
            if (b > max) {
                max = b;
            }

            // value
            int v = max;

            // saturation
            int s = max - min;

            // hue
            int h = (min == max) ? 0 :
                    // B is minimum
                    (min == b) ? (60 * ((float)(g - r) / s) + 60) :
                    // R is minimum
                    (min == r) ? (60 * ((float)(b - g) / s) + 180) :
                    // G is minimum
                    (60 * ((float)(r - b) / s) + 300) ;

            Imgdata_at(hsv, x, y)[0] = h;
            Imgdata_at(hsv, x, y)[1] = s;
            Imgdata_at(hsv, x, y)[2] = v;
        }
    }
}

void hsv_to_rgb(Imgdata *hsv, Imgdata *rgb)
{
    for (int y = 0; y < hsv->height; y++) {
        for (int x = 0; x < hsv->width; x++) {
            int h = Imgdata_at(hsv, x, y)[0];
            int s = Imgdata_at(hsv, x, y)[1];
            int v = Imgdata_at(hsv, x, y)[2];

            float h_ = h / 60.0f;
            float x_ = s * (1 - fabs(fmod(h_, 2) - 1));

            if (h_ < 1) { // h_ >= 0
                Imgdata_at(rgb, x, y)[0] = ((v - s) + s);
                Imgdata_at(rgb, x, y)[1] = ((v - s) + x_);
                Imgdata_at(rgb, x, y)[2] = (v - s);
            } else if (h_ < 2) {
                Imgdata_at(rgb, x, y)[0] = ((v - s) + x_);
                Imgdata_at(rgb, x, y)[1] = ((v - s) + s);
                Imgdata_at(rgb, x, y)[2] = (v - s);
            } else if (h_ < 3) {
                Imgdata_at(rgb, x, y)[0] = (v - s);
                Imgdata_at(rgb, x, y)[1] = ((v - s) + s);
                Imgdata_at(rgb, x, y)[2] = ((v - s) + x_);
            } else if (h_ < 4) {
                Imgdata_at(rgb, x, y)[0] = (v - s);
                Imgdata_at(rgb, x, y)[1] = ((v - s) + x_);
                Imgdata_at(rgb, x, y)[2] = ((v - s) + s);
            } else if (h_ < 5) {
                Imgdata_at(rgb, x, y)[0] = ((v - s) + x_);
                Imgdata_at(rgb, x, y)[1] = (v - s);
                Imgdata_at(rgb, x, y)[2] = ((v - s) + s);
            } else { // h_ < 6
                Imgdata_at(rgb, x, y)[0] = ((v - s) + s);
                Imgdata_at(rgb, x, y)[1] = (v - s);
                Imgdata_at(rgb, x, y)[2] = ((v - s) + x_);
            }
        }
    }
}

void get_h_as_rgb(Imgdata *hsv, Imgdata *h)
{
    // convert hue as RGB
    for (int y = 0; y < hsv->height; y++) {
        for (int x = 0; x < hsv->width; x++) {
            int h_ = Imgdata_at(hsv, x, y)[0];
            if (h_ < 60) { // h >= 0
                Imgdata_at(h, x, y)[0] = 255;
                Imgdata_at(h, x, y)[1] = (uint8_t)((255.0f / 60) * h_);
                Imgdata_at(h, x, y)[2] = 0;
            } else if (h_ < 120) {
                Imgdata_at(h, x, y)[0] = (uint8_t)(255 - (255.0f / 60) * (h_ - 60));
                Imgdata_at(h, x, y)[1] = 255;
                Imgdata_at(h, x, y)[2] = 0;
            } else if (h_ < 180) {
                Imgdata_at(h, x, y)[0] = 0;
                Imgdata_at(h, x, y)[1] = 255;
                Imgdata_at(h, x, y)[2] = (uint8_t)((255.0f / 60) * (h_ - 120));
            } else if (h_ < 240) {
                Imgdata_at(h, x, y)[0] = 0;
                Imgdata_at(h, x, y)[1] = (uint8_t)(255 - (255.0f / 60) * (h_ - 180));
                Imgdata_at(h, x, y)[2] = 255;
            } else if (h_ < 300) {
                Imgdata_at(h, x, y)[0] = (uint8_t)((255.0f / 60) * (h_ - 240));
                Imgdata_at(h, x, y)[1] = 0;
                Imgdata_at(h, x, y)[2] = 255;
            } else { // h < 360
                Imgdata_at(h, x, y)[0] = 255;
                Imgdata_at(h, x, y)[1] = 0;
                Imgdata_at(h, x, y)[2] = (uint8_t)(255 - (255.0f / 60) * (h_ - 300));
            }
        }
    }
}

void get_s_as_grayscale(Imgdata *hsv, Imgdata *s)
{
    // get saturation as grayscale
    for (int y = 0; y < hsv->height; y++) {
        for (int x = 0; x < hsv->width; x++) {
            Imgdata_at(s, x, y)[0] = Imgdata_sat_u8(Imgdata_at(hsv, x, y)[1]);
        }
    }
}

void get_v_as_grayscale(Imgdata *hsv, Imgdata *v)
{
    // get value as grayscale
    for (int y = 0; y < hsv->height; y++) {
        for (int x = 0; x < hsv->width; x++) {
            Imgdata_at(v, x, y)[0] = Imgdata_sat_u8(Imgdata_at(hsv, x, y)[2]);
        }
    }
}

int main(int argc, char *argv[])
{
    Imgdata *img = Imgdata_read_png("./imori_256x256.png");

    // store HSV image with float for calculation
    Imgdata *img_hsv = Imgdata_alloc(img->width, img->height, 3);
    // RGB to HSV
    rgb_to_hsv(img, img_hsv);

    // get H/S/V values as a image
    Imgdata *img_h = Imgdata_alloc(img->width, img->height, 3);
    get_h_as_rgb(img_hsv, img_h);
    Imgdata_write_png(img_h, "./005_h.png");

    Imgdata *img_s = Imgdata_alloc(img->width, img->height, 1);
    get_s_as_grayscale(img_hsv, img_s);
    Imgdata_write_png(img_s, "./005_s.png");

    Imgdata *img_v = Imgdata_alloc(img->width, img->height, 1);
    get_v_as_grayscale(img_hsv, img_v);
    Imgdata_write_png(img_v, "./005_v.png");

    // reverse hue
    for (int y = 0; y < img_hsv->height; y++) {
        for (int x = 0; x < img_hsv->width; x++) {
            int h = Imgdata_at(img_hsv, x, y)[0];
            Imgdata_at(img_hsv, x, y)[0] = (h + 180) % 360;
        }
    }

    // return HSV to RGB
    Imgdata *img_ret_rgb = Imgdata_alloc(img->width, img->height, img->channel);
    // HSV to RGB
    hsv_to_rgb(img_hsv, img_ret_rgb);
    Imgdata_write_png(img_ret_rgb, "./005_h_rev.png");

    Imgdata_free(&img);
    Imgdata_free(&img_hsv);
    Imgdata_free(&img_h);
    Imgdata_free(&img_s);
    Imgdata_free(&img_v);
    Imgdata_free(&img_ret_rgb);

    return 0;
}
