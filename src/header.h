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
    unsigned char x, y;
    char type;
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
int fill_data(char **);                // step 2
void mask_matrix(char **);             // step 3
void apply_format(char **, int);       // step 4

/* auxiliary functions (extra.c) */
void error(int);                        
void bitprint(int);                     
void printMatrix(char **);              
int _is_set(void *, int, char);
void invert_int_array (int* arr, unsigned int end);

/* QR file format */
void make_PPM(char **);
void make_PNG(char **); 
void make_JPG(char **); 

/* custom free functions */
void free_matrix(char **);
void free_polynomial(polynomial*);
void free_tables(tables*);
void free_groups(_groups_*);