#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "header.h"
/*
    Thanks to radubig (https://github.com/radubig) for suggesting stb library implementation
*/
void make_PPM(char **matrix) {
    FILE *out = fopen(file, "w+");
    fprintf(out, "P6\n");

    const int img_line_len = scale * (size + 2);
    fprintf(out, "%d %d\n255\n", img_line_len, img_line_len);
    uchar alb = 255, r = red, g = green, b = blue;

    // top padding
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= img_line_len; j++) {
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
        }
    }

    // qr code data
    for (int i = 0; i < size; i++) {
        for (int k = 1; k <= scale; k++) {

            // left padding
            for (int m = 1; m <= scale; m++) {
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
            }

            // QR pixel data
            for (int j = 0 ; j < size; j++) {
                for (int l = 1; l <= scale; l++) {
                    if (matrix[i][j] % 2) {
                        fwrite(&r, sizeof(uchar), 1, out);
                        fwrite(&g, sizeof(uchar), 1, out);
                        fwrite(&b, sizeof(uchar), 1, out);
                    } else {
                        fwrite(&alb, sizeof(uchar), 1, out);
                        fwrite(&alb, sizeof(uchar), 1, out);
                        fwrite(&alb, sizeof(uchar), 1, out);
                    }
                }
            }

            // right padding
            for (int m = 1; m <= scale; m++) {
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
            }
        }
    }

    // top padding
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= img_line_len; j++) {
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
        }
    }

    fclose(out);
}

void make_PNG(char **matrix) {
    
    const int img_line_len = scale * (size + 2);
    uchar alb = 255, r = red, g = green, b = blue;

    uchar* pixel_data = calloc(img_line_len * img_line_len * 3, sizeof(uchar));
    unsigned int pos = 0;

    // top padding
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= img_line_len; j++) {
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
        }
    }

    // qr code data
    for (int i = 0; i < size; i++) {
        for (int k = 1; k <= scale; k++) {

            // left padding
            for (int m = 1; m <= scale; m++) {
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
            }

            // QR pixel data
            for (int j = 0 ; j < size; j++) {
                for (int l = 1; l <= scale; l++) {
                    if (matrix[i][j] % 2) {
                        pixel_data[pos++] = r;
                        pixel_data[pos++] = g;
                        pixel_data[pos++] = b;
                    } else {
                        pixel_data[pos++] = alb;
                        pixel_data[pos++] = alb;
                        pixel_data[pos++] = alb;
                    }
                }
            }

            // right padding
            for (int m = 1; m <= scale; m++) {
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
            }
        }
    }

    // bottom padding
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= img_line_len; j++) {
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
        }
    }

    stbi_write_png(file, img_line_len, img_line_len, 3,  pixel_data, img_line_len * 3 * sizeof(uchar));
    free(pixel_data);
} // radubig

void make_JPG(char ** matrix) {
    const int img_line_len = scale * (size + 2);
    uchar alb = 255, r = red, g = green, b = blue;

    uchar* pixel_data = calloc(img_line_len * img_line_len * 3, sizeof(uchar));
    unsigned int pos = 0;

    // top padding
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= img_line_len; j++) {
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
        }
    }

    // qr code data
    for (int i = 0; i < size; i++) {
        for (int k = 1; k <= scale; k++) {

            // left padding
            for (int m = 1; m <= scale; m++) {
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
            }

            // QR pixel data
            for (int j = 0 ; j < size; j++) {
                for (int l = 1; l <= scale; l++) {
                    if (matrix[i][j] % 2) {
                        pixel_data[pos++] = r;
                        pixel_data[pos++] = g;
                        pixel_data[pos++] = b;
                    } else {
                        pixel_data[pos++] = alb;
                        pixel_data[pos++] = alb;
                        pixel_data[pos++] = alb;
                    }
                }
            }

            // right padding
            for (int m = 1; m <= scale; m++) {
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
                pixel_data[pos++] = alb;
            }
        }
    }

    // bottom padding
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= img_line_len; j++) {
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
            pixel_data[pos++] = alb;
        }
    }

    stbi_write_jpg(file, img_line_len, img_line_len, 3,  pixel_data, img_line_len * 3 * sizeof(uchar));
    free(pixel_data);
}

//dg