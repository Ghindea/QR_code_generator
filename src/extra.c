#include "header.h"

void bitprint(int n) {
    if (!n) printf("0");
        else printf("1");
}
void error(int x) {
    switch (x)
    {
        case 0: {
            printf(COLOR_RED BOLD UNDERLINE "error:" COLOR_OFF 
                    "  Input text is too long for the specified version of QR code!\n\t"
                    "To edit QR version use --config option and check README.\n");
        } 
        break;
        case 404: {
            printf(COLOR_RED BOLD UNDERLINE "error:" COLOR_OFF 
                    "  Could not allocate enough memory!\n");
        }
        break;
        default:
            break;
    }
}
void printMatrix(char **matrice) {
    for (int i = 0; i < size; i++) {
        // printf("%d ", i);
        for (int j = 0; j < size; j++) {
            printf("%d", matrice[i][j]);
        }
        printf("\n");
    }
}
void makeQR(char **matrix) {
    FILE *out = fopen(file, "w+");
    fprintf(out, "P6\n");
    fprintf(out, "%d %d\n255\n", 2*scale + scale * size, 2*scale + scale * size);
    uchar alb = 255, r = red, g = green, b = blue;
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= 2*scale + scale * size; j++) {
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
        }
    }
    for (int i = 0; i < size; i++) {
        for (int k = 1; k <= scale; k++) {
            for (int m = 1; m <= scale; m++) {
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
            }
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
            for (int m = 1; m <= scale; m++) {
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
                fwrite(&alb, sizeof(uchar), 1, out);
            }
        }
    }
    for (int i = 1; i <= scale; i++) {
        for (int j = 1; j <= 2*scale + scale * size; j++) {
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
            fwrite(&alb, sizeof(uchar), 1, out);
        }
    }
    fclose(out);
}

void invert_int_array (int * arr, unsigned int end) {
    int tmp;
    unsigned start = 0;
    while (start < end) {
        tmp = arr[start];
        arr[start] = arr[end];
        arr[end] = tmp;
        start++; end--;
    }
}
uchar * invert_uchar_array (uchar * a, int arr_size) {
    uchar * b = (uchar *)calloc(arr_size+1, sizeof(uchar));
    for (int i = 0; i <= arr_size; i++) {
        b[i] = a[arr_size - i];
    }
    return b;
}

int _is_set(char oct, int bit) {
    if (oct & (1 << bit)) return 1;
        else return 0;
}

