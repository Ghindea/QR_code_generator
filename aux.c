#include "header.h"
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
            if (matrix[i][j]) {
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
