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
        for (int j = 0; j < size; j++) {
            printf("%d", matrice[i][j]);
        }
        printf("\n");
    }
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