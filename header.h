#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

/* determinare size QR*/
#if version == 0    
#define size 21
#else
#define size 21 + (version - 1) * 4
#endif

char **initMatrix();
void printMatrix(char **);
void makeQR(char **);