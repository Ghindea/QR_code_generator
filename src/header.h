#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"         // file for #defines

/* calculate QR code size*/
#define size 21 + (version - 1) * 4

/* polynomials */
typedef struct poly{
    char grad;
    unsigned char *coef;
}polynomial;
polynomial generator(int);
void polyprint(polynomial);
/* multi file functions*/
char **initMatrix();        // step 1
int fill_data(char **);     // step 2

unsigned char * gf256();
void bitprint(int);         //
void error(int);            //
void printMatrix(char **);  //
void makeQR(char **);       // auxiliary functions (extra.c)