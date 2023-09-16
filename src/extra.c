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
                    "  Input text is too long for the specified version of QR code.\n\t"
                    "To edit QR version use --config option and check README.\n");
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
    fprintf(out, "%d %d\n255\n", size, size);
    unsigned char alb = 255, r = red, g = green, b = blue;
    for (int i = 0; i < size; i++) {
        for (int j = 0 ; j < size; j++) {
            if (matrix[i][j] == 1) {
                fwrite(&r, sizeof(unsigned char), 1, out);
                fwrite(&g, sizeof(unsigned char), 1, out);
                fwrite(&b, sizeof(unsigned char), 1, out);
            } else {
                fwrite(&alb, sizeof(unsigned char), 1, out);
                fwrite(&alb, sizeof(unsigned char), 1, out);
                fwrite(&alb, sizeof(unsigned char), 1, out);
            }
        }
    }
}

