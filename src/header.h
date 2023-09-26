#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macros.h"

/* calculate QR code size*/
#define size 21 + (version - 1) * 4
#define uchar unsigned char 

typedef struct {
    char x, y, type;
    int prev;
}_bit_coord_;

typedef struct {
    int G1, G2;     // num of blocks in group 1 and group 2
    int B1, B2;     // num of data codewords in each of group 1's and group 2's blocks
    int EC;         // num of error correction codewords per block
    int **data_blocks;     // array of data codewords sequences    (data_blocks[i] <=> block i)
    int **ec_blocks;       // array of error correction codewords sequences corresponding to each block
}_groups_;

/* polynomials */
typedef struct poly{
    int grad;   // gradul polinomului
    int *coef;  // vector coeficienti
}polynomial;
typedef struct {
    uchar *_exp, *_log;
}tables;
tables load_gf256();

polynomial poly_init(int, const int *);
polynomial reed_solomon(polynomial, int);

void polyprint(polynomial);
void normalise(polynomial *);

/* multi file functions*/
char **initMatrix();                   // step 1
void free_matrix(char **);
int fill_data(char **);                // step 2
void mask_matrix(char **);             // step 3
void apply_format(char **, int);       // step 4

int _is_set(char, int);
void bitprint(int);         //
void error(int);            //
void printMatrix(char **);  //
void makeQR(char **);       //
#ifdef QRCODE_MAKE_PNG
void makeQR_PNG(char **);
#endif
void invert_int_array (int* arr, unsigned int end);

/* custom free functions */
void free_polynomial(polynomial*);
void free_tables(tables*);
void free_groups(_groups_*);