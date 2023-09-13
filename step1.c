#include "header.h"
void apple_padding(char **matrix) {
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
}
void apply_correction(char **matrix) {
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
    apple_padding(qr_matrix);
    apply_correction(qr_matrix);
    apply_mask(qr_matrix);
    apply_data_type(qr_matrix);
    return qr_matrix;
}
