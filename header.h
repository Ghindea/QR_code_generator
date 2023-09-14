#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"         // configuration file

/* calculate QR code size*/
#define size 21 + (version - 1) * 4

char **initMatrix();        // step 1
int fill_data(char **);    // step 2

void error(int);            //
void printMatrix(char **);  //
void makeQR(char **);       // auxiliary