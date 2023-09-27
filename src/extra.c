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

int _is_set(void *num, int bit, char type) {

    switch (type)
    {
    case 0:     // char
        if (*(char*)num & (1 << bit)) return 1; // bit is set
            else return 0;                      // bit isn't set
        break;
    case 1:     // int
        if (*(int*)num & (1 << bit)) return 1;
            else return 0;
        break;
    default:
        break;
    }
    
}

void free_polynomial(polynomial* poly)
{
    free(poly->coef);
}
void free_tables(tables* table)
{
    free(table->_log);
    free(table->_exp);
}
void free_groups(_groups_* seg) {
    for (int i = 0; i < seg->G1+seg->G2; i++) {
        free(seg->data_blocks[i]);
    }
    for (int i = 0; i < seg->G1+seg->G2; i++) {
        free(seg->ec_blocks[i]);
    }
    free(seg->data_blocks);
    free(seg->ec_blocks);
    free(seg);
}
//dg