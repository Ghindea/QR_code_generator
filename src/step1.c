#include "header.h"

void draw_alignment(int x, int y, char **matrix) {
    if (matrix[x][y] == 1) return;
    for (int i = -2; i <= 2; i++) {             // verificare spatiu gol
        for (int j = -2; j <= 2; j++) {
            if (matrix[x+i][y+j] == 1) return; 
        }
    }
    for (int i = -2; i <= 2; i++) {
        matrix[x-2][y+i] = 1;
        matrix[x+2][y+i] = 1;
    }
    for (int i = -1; i <= 1; i++) {
        matrix[x+i][y-2] = 1;
        matrix[x+i][y+2] = 1;
    }
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
                matrix[x+i][y+j] = 2;
        }
    }
    matrix[x][y] = 1;
}
void apply_base(char **matrix) {    // finder patterns; timing pattern; dark module
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (i == 0 || i == 6 || j == 0 || j == 6) {
                matrix[i][j] = 1;
                matrix[size-1-i][j] = 1;
                matrix[i][size-1-j] = 1;
            }
            if (i >= 2 && i <= 4 && j >= 2 && j <= 4) {
                matrix[i][j] = 1;
                matrix[size-1-i][j] = 1;
                matrix[i][size-1-j] = 1;
            }
        }
    }
    for (int j = 7; j < size - 7; j++) {
        if (j % 2 == 0) {
            matrix[6][j] = 1;
            matrix[j][6] = 1;
        }
    }
    matrix[size-8][8] = 1;
}
void apply_alignment(char **matrix) {   // alignment patterns versions 2->13
    switch (version/7)
    {
        case 0: {                   // pentru versiunile 1-6 nu poate fi decat un alignment pattern
            if (version == 1) break;// la cordonata (a,a), caci altfel se suprapune cu finding patternurile
            int a = 18+(version-2)*4;
            draw_alignment(a, a, matrix);
            break;
        }
        case 1: {                   // teoretic sunt 9 alignment patternuri, dar in practica ies 6
            int a = 22+(version%7)*2, b = 18+(version-2)*4; // caci 3 se suprapun
            int v[] = {6, a, b};
            for (int i = 0; i <= 2; i++) {
                for (int j = 0; j <= 2; j++) {
                    draw_alignment(v[i],v[j],matrix);
                }
            }
            break;
        }
        default:
            break;
    }
}

char **initMatrix() {
    /* memory alloc */
    char **qr_matrix = calloc(size, sizeof(char *));
    for (int i = 0; i < size; i++) {
        qr_matrix[i] = calloc(size, sizeof(char));
    }
    /* apply format*/
    apply_base(qr_matrix);
    apply_alignment(qr_matrix);
    return qr_matrix;
}
// dg