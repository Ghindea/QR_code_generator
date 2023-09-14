#include "header.h"
void draw_alignment(int x, int y, char **matrix) {
    if (matrix[x][y] == 1) return;
    for (int i = -2; i <= 2; i++) {             // verificare spatiu gol
        for (int j = -2; j <= 2; j++) {
            if (matrix[x+i][y+j] == 1) return; 
        }
    }
    matrix[x][y] = 1;
    for (int i = -2; i <= 2; i++) {
        matrix[x-2][y+i] = 1;
        matrix[x+2][y+i] = 1;
    }
    for (int i = -1; i <= 1; i++) {
        matrix[x+i][y-2] = 1;
        matrix[x+i][y+2] = 1;
    }
}
void apply_base(char **matrix) {    // finder patterns
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
}
void apply_alignment(char **matrix) {
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
void apply_correction(char **matrix) {      // error correction; dark module; timing pattern
    switch (error_correction) {
        case 1:
            matrix[8][1] = 1;
            matrix[size-2][8] = 1;
            break;
        case 2:
            matrix[8][0] = 1;
            matrix[size-1][8] = 1;
            break;
        case 3:
            matrix[8][1] = 1;
            matrix[size-2][8] = 1;
            matrix[8][0] = 1;
            matrix[size-1][8] = 1;
            break;
        default:
            break;
    }
    for (int j = 7; j < size - 7; j++) {
        if (j % 2 == 0) {
            matrix[6][j] = 1;
            matrix[j][6] = 1;
        }
    }
    matrix[size-8][8] = 1;
}
void apply_mask(char **matrix) {
    switch (mask)
    {
    case 1:
        matrix[8][4] = 1;
        matrix[size-5][8] = 1;
        break;
    case 2:
        matrix[8][3] = 1;
        matrix[size-4][8] = 1;
        break;
    case 3:
        matrix[8][4] = 1;
        matrix[size-5][8] = 1;
        matrix[8][3] = 1;
        matrix[size-4][8] = 1;
        break;
    case 4:
        matrix[8][2] = 1;
        matrix[size-3][8] = 1;
        break;
    case 5:
        matrix[8][2] = 1;
        matrix[size-3][8] = 1;
        matrix[8][4] = 1;
        matrix[size-5][8] = 1;
        break;
    case 6:
        matrix[8][2] = 1;
        matrix[size-3][8] = 1;
        matrix[8][3] = 1;
        matrix[size-4][8] = 1;
        break;
    case 7:
        matrix[8][4] = 1;
        matrix[size-5][8] = 1;
        matrix[8][3] = 1;
        matrix[size-4][8] = 1;
        matrix[8][2] = 1;
        matrix[size-3][8] = 1;
        break;
    default:
        break;
    }
}
void apply_data_type(char **matrix) {
    switch (data_type)
    {
    case 1:
        matrix[size-2][size-2] = 1;
        break;
    case 2:
        matrix[size-2][size-1] = 1;
        break;
    case 3:
        matrix[size-1][size-1] = 1;
        break;
    case 4:
        matrix[size-1][size-2] = 1;
        break;
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
    apply_correction(qr_matrix);
    apply_mask(qr_matrix);
    apply_data_type(qr_matrix);
    return qr_matrix;
}
