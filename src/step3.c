#include "header.h"
int boundary(char **qr, unsigned char x, unsigned char y) {        // checks if module coordonates are available to place data
    unsigned aux = size;
    if (x < 0 || x >= aux || y < 0 || y >=aux) return 2;    // matrix limits
    if (x > -1 && x < 9 && y >-1 && y < 9) return 2;        // find pttrn up left
    if (x > size-9 && x < size && y > -1 && y < 9) return 2;// find pttrn dwn left
    if (x > -1 && x < 9 && y > size-9 && y < size) return 2;// find pttrn up right
    if (version >= 7) {
        if (x > -1 && x < 6 && y > size - 12 && y < size - 8) return 2; // special format pttrn up right 
        if (x > size - 12 && x < size - 8 && y > -1 && y < 6) return 2; // special format pttrn up right 
    }
    if (qr[x][y] == 2 || qr[x][y] == 3) return 0;           // format pttrn + alignment pttrn
    if (y == 6 || x == 6) return 0;                         // timing pttrn

    return 1;
}
// mask type formulas
int _mask0(int i, int j) { return (i+j) % 2 == 0 ? 1:0; }
int _mask1(int i, int j) { return i % 2 == 0 ? 1:0;     }
int _mask2(int i, int j) { return j % 3 == 0 ? 1:0;     }
int _mask3(int i, int j) { return (i+j) % 3 == 0 ? 1:0; }
int _mask4(int i, int j) { return (i/2 + j/3) % 2 == 0 ? 1:0;           }
int _mask5(int i, int j) { return (i*j) % 2 + (i*j) % 3 == 0 ? 1:0;     }
int _mask6(int i, int j) { return ((i*j) % 3 + i*j) % 2 == 0 ? 1:0;     }
int _mask7(int i, int j) { return ((i*j) % 3 + i + j) % 2 == 0 ? 1:0;   }

typedef int (*fct)(int, int);
fct parse_function(int n) {
    switch (n)
    {
        case 0: return &_mask0; break;
        case 1: return &_mask1; break;
        case 2: return &_mask2; break;
        case 3: return &_mask3; break;
        case 4: return &_mask4; break;
        case 5: return &_mask5; break;
        case 6: return &_mask6; break;
        case 7: return &_mask7; break;
        
        default: 
            return NULL;
            break;
    }
}

int penalty_1st(char ** qr, fct function){ return 0;}
int penalty_2nd(char ** qr, fct function){ return 0;}
int penalty_3rd(char ** qr, fct function){ return 0;}
int penalty_4th(char ** qr, fct function){ return 0;}
void apply_mask(char ** qr, fct function){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (boundary(qr, i, j) == 1 && function(i, j)) {
                qr[i][j] = 1 - qr[i][j]%2;
            }
        }
    }
}

void mask_matrix(char **qr) {
#if mask_type == -1     // mask type is set on auto

    int *penalties = (int *) calloc(8, sizeof(int));

    for (int i = 0; i < 8; i++) {
        penalties[i] += penalty_1st(qr, parse_function(i));
        penalties[i] += penalty_2nd(qr, parse_function(i));
        penalties[i] += penalty_3rd(qr, parse_function(i));
        penalties[i] += penalty_4th(qr, parse_function(i));
    }
    int penalty_score = __INT_MAX__, index = 0;
    for (int i = 0; i < 8; i++) {
        if (penalties[i] < penalty_score) {
            penalty_score = penalties[i];
            index = i;
        }
    }
    apply_mask(qr, parse_function(index));
    apply_format(qr, index);
#else                   // mask type is custom set
    fct ptr = parse_function(mask_type);
    apply_mask(qr, ptr);
    apply_format(qr, mask_type);
#endif

}
// dg