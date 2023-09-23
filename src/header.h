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

/* polynomials */
typedef struct poly{
    int grad;   // gradul polinomului
    int *coef;  // vector coeficienti
    int is_heap_alloc;
}polynomial;
typedef struct {
    uchar *_exp, *_log;
}tables;
tables load_gf256();

void polyprint(polynomial);
polynomial poly_init(int, int *);
polynomial reed_solomon(polynomial, int);

void polyprint(polynomial);
void normalise(polynomial *);

/* custom free functions */
void free_polynomial(polynomial* poly);
void free_tables(tables* table);

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
void makeQR(char **);       // auxiliary functions (extra.c)
void invert_int_array (int* arr, unsigned int end);