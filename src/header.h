#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"         // file for #defines
/* calculate QR code size*/
#define size 21 + (version - 1) * 4
#define uchar unsigned char 

/* polynomials */
typedef struct poly{
    int grad;   // gradul polinomului
    int *coef;  // vector coeficienti
}polynomial;
typedef struct {
    uchar *_exp, *_log;
}tables;
tables load_gf256();

polynomial reed_solomon(polynomial, int);

void polyprint(polynomial);
void normalise(polynomial *);

/* multi file functions*/
char **initMatrix();        // step 1
int fill_data(char **);     // step 2

unsigned char * pow_gf256();
unsigned char * log_gf256(unsigned char *);
void bitprint(int);         //
void error(int);            //
void printMatrix(char **);  //
void makeQR(char **);       // auxiliary functions (extra.c)