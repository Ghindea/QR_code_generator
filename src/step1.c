#include "header.h"
/* coordinates vectors for alignment patterns*/

const int unu[]   = {0,0,18,22,26,30,34,22,24,26,28,30,32,34,26,26,26,30,30,30,34,28,26,30,28,32,30,34,26,30,26,30,34,30,34,30,24,28,32,26,30};
const int doi[]   = {0,0,0,0,0,0,0,38,42,46,50,54,58,62,46,48,50,54,56,58,62,50,50,54,54,58,58,62,50,54,52,56,60,58,62,54,50,54,58,54,58};
const int trei[]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,66,70,74,78,82,86,90,72,74,78,80,84,86,90,74,78,78,82,86,86,90,78,76,80,84,82,86};
const int patru[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,94,98,102,106,110,114,118,98,102,104,108,112,114,118,102,102,106,110,110,114};
const int cinci[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,122,126,130,134,138,142,146,126,128,132,136,138,142};
const int sase[]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,150,154,158,162,166,170}; 

void draw_alignment(int x, int y, char **matrix) {
    if (matrix[x][y] == 1) return;
    for (int i = -2; i <= 2; i++) {             // verificare spatiu gol
        for (int j = -2; j <= 2; j++) {
            if (matrix[x+i][y+j] == 1) return; 
        }
    }
    for (int i = -2; i <= 2; i++) {
        matrix[x-2][y+i] = 3;
        matrix[x+2][y+i] = 3;
    }
    for (int i = -1; i <= 1; i++) {
        matrix[x+i][y-2] = 3;
        matrix[x+i][y+2] = 3;
    }
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
                matrix[x+i][y+j] = 2;
        }
    }
    matrix[x][y] = 3;
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
            matrix[6][j] = 3;
            matrix[j][6] = 3;
        }
    }
    matrix[size-8][8] = 1;
}
void apply_alignment(char **matrix) {   // alignment patterns versions 2->13
    if (version == 1) return;

    int v[] = {6, unu[version], doi[version], trei[version], patru[version], cinci[version], sase[version]};
    for (int i = 0; i <= version/7 + 1; i++) {
        for (int j = 0; j <= version/7 + 1; j++) {
            draw_alignment(v[i],v[j],matrix);
        }
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

void free_matrix(char** qr_matrix)
{
    for(int i = 0; i < size; i++)
        free(qr_matrix[i]);
    free(qr_matrix);
}
// dg