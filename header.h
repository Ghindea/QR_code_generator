#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

/* determinare size QR*/
#define size 21 + (version - 1) * 4

char **initMatrix();
void printMatrix(char **);
void makeQR(char **);