#include "header.h"

char len_bit_no() {         // determines len encoding length based on data type
    switch (data_type)
    {
        case 1: // numeric
            if (version < 10) return 10;
                else if (version < 27) return 12;
                        else return 14;
            break;
        case 2: // alphanumeric
            if (version < 10) return 9;
                else if (version < 27) return 11;
                        else return 13;
            break;
        case 3: // byte
            if (version < 10) return 8;
                else return 16;
            break;
        case 4: // kanji
            if (version < 10) return 8;
                else if (version < 27) return 10;
                        else return 12;
            break;
        default:
        break;
    }
}
int load_capacity_data(FILE *in) {
    int x = 0;
    for (int i = 1; i < version; i++) {
        fscanf(in, "%d", &x);   // L
        fscanf(in, "%d", &x);   // M
        fscanf(in, "%d", &x);   // Q
        fscanf(in, "%d", &x);   // H
    }
    switch (error_correction_level) {
        case 0:
            fscanf(in, "%d", &x);   // L
            fscanf(in, "%d", &x);   // M
            break;
        case 1:
            fscanf(in, "%d", &x);   // L
            break;
        case 2:
            fscanf(in, "%d", &x);   // L
            fscanf(in, "%d", &x);   // M
            fscanf(in, "%d", &x);   // Q
            fscanf(in, "%d", &x);   // H
            break;
        case 3:
            fscanf(in, "%d", &x);   // L
            fscanf(in, "%d", &x);   // M
            fscanf(in, "%d", &x);   // Q
            break;
        default:
            break;
    }
    return x;
}
int available(char **qr, char x, char y) {        // checks if module coordonates are available to place data
    char aux = size;
    if (x < 0 || x >= aux || y < 0 || y >=aux) return 2;  // matrix limits
    if (x > -1 && x < 9 && y >-1 && y < 9) return 2;        // find pttrn up left
    if (x > size-9 && x < size && y > -1 && y < 9) return 2;// find pttrn dwn left
    if (x > -1 && x < 9 && y > size-9 && y < size) return 2;// find pttrn up right
    if (qr[x][y]) return 0;                     // format pttrn
    if (y == 6) return 0;

    return 1;
}
void position_up(char *prev, char *x, char *y) {
    if ((*prev) % 2 != 0) {
        (*x)--; (*y)++;
    } else (*y)--;
    (*prev)++;
}
void position_down(char *prev, char *x, char *y) {
    if ((*prev) % 2 != 0) {
        (*x)++; (*y)++;
    } else (*y)--;
    (*prev)++;
}
void load_group(char **qr, char *x, char *y, char *prev, char *type, int val, char bit_nr) {
    int i = bit_nr-1, msk = 1;
    // prev % 2 == 0 -> left 
    // prev % 2 == 1 -> upper-right/down-right
    while (i >= 0) {
        if (*type == 1 && available(qr, *x, *y) == 2) {
            (*x)++; *y -= 2; *type = 2;
        }
        if (*type == 2 && available(qr, *x, *y) == 2) {
            (*x)--; (*y) -= 2; *type = 1;
        }
        if (available(qr, *x, *y) == 1){
            msk = 1 << i;
            if (msk & val) qr[*x][*y] = 1;
            bitprint(msk&val);
            i--;
        } 
        if (*type == 1) position_up(prev, x, y);   // type == 1 -> up
            else position_down(prev, x, y);        // type == 2 -> down
    }
    printf(" ");
}

int fill_data(char **matrix) {
    FILE *in = fopen("utils/byte_mode_capacity.txt", "r");
    FILE *fin = fopen("utils/data_codewords_capacity.txt", "r");

    int capacity = load_capacity_data(in), codewords = load_capacity_data(fin); // check thonky QR code tutorial to understand (link in README)
    
    char x = size - 1, y = size - 1, type = 1, prev = 0; 
    char *string = (char *)calloc(MAXLEN, sizeof(char));
    fgets(string, MAXLEN, stdin);
    printf("%ld\n", strlen(string)-1);
    if (strlen(string)-1 <= capacity) {
        load_group(matrix, &x, &y, &prev, &type, 1 << (data_type - 1), 4); // load data type
        load_group(matrix, &x, &y, &prev, &type, strlen(string) - 1, len_bit_no());   // load string len
        for (int i = 0; i < strlen(string) - 1; i++) {
            load_group(matrix, &x, &y, &prev, &type, string[i], 8);
        }
        load_group(matrix, &x, &y, &prev, &type, 0, 4);

        int data_len = (strlen(string) - 1) * 8 + 4 + len_bit_no();
        if (data_len % 8) {
            load_group(matrix, &x, &y, &prev, &type, 0, data_len % 8);
            data_len += data_len % 8;
        }
        int ok = 0;
        while (data_len < codewords * 8) {
            if (!ok) {
                load_group(matrix, &x, &y, &prev, &type, 236, 8);
                ok = 1;
            } else {
                load_group(matrix, &x, &y, &prev, &type, 17, 8);
                ok = 0;
            } 
            data_len += 8;
        }
        


        printf("\n%d %d\n", data_len, codewords*8);
        return 1;
    } else {
        error(0);
        return 0;
    } 
    
}
