#include "header.h"
char * load_format(FILE *din, int mask) {
    char * string = (char *) calloc(18, sizeof(char));
    switch (error_correction_level)
    {
    case 0:
        for (int i = 1; i <= 8; i++) {
            fscanf(din, "%s", string);
        }
        for (int i = 0; i < mask; i++) {
            fscanf(din, "%s", string);
        }
        fscanf(din, "%s", string);
        return string;
        break;
    case 1:
        for (int i = 0; i < mask; i++) {
            fscanf(din, "%s", string);
        }
        fscanf(din, "%s", string);
        return string;
        break;
    case 2:
        for (int i = 1; i <= 24; i++) {
            fscanf(din, "%s", string);
        }
        for (int i = 0; i < mask; i++) {
            fscanf(din, "%s", string);
        }
        fscanf(din, "%s", string);
        return string;
        break;
    case 3:
        for (int i = 1; i <= 16; i++) {
            fscanf(din, "%s", string);
        }
        for (int i = 0; i < mask; i++) {
            fscanf(din, "%s", string);
        }
        fscanf(din, "%s", string);
        return string;
        break;
    default:
        break;
    }
}
void apply_format(char **qr, int mask) {
    FILE *din = fopen("utils/format_information_string.txt", "r");
    char *format_string = load_format(din, mask);

    for (int i = 0; i <= 6; i++) {
        qr[8][i] = format_string[i] - '0';
        qr[size - 1 - i][8] = format_string[i] - '0';
    }
    qr[8][6] = 1;
    qr[8][7] = format_string[6] - '0';
    qr[8][8] = format_string[7] - '0';
    qr[7][8] = format_string[8] - '0';
    for (int i = 0; i < 8; i++) {
        qr[8][size - 8 + i] = format_string[7 + i] - '0';
    }
    for (int i = 0; i <= 5; i++) {
        qr[i][8] = format_string[14-i] - '0';
    }
}