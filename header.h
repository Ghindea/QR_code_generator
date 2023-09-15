#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"         // configuration file

/* calculate QR code size*/
#define size 21 + (version - 1) * 4

/* macros */

/* multi file functions*/
char **initMatrix();        // step 1
int fill_data(char **);     // step 2

void bitprint(int);         //
void error(int);            //
void printMatrix(char **);  //
void makeQR(char **);       // auxiliary functions (extra.c)