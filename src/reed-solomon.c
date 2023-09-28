#include "header.h"

void polyprint(polynomial P) {
    if (P.coef[P.grad] == 0) {printf("0\n"); return;}
    if (P.coef[P.grad] == 1) {
        if (P.grad == 1) {
            printf("x ");
        } else printf("x^%d ", P.grad);
    } else if (P.grad == 1){
        printf("%dx ", P.coef[P.grad]);
    } else printf("%dx^%d ", P.coef[P.grad], P.grad);
    for (int i = P.grad-1; i >= 0; i--) {
        if (P.coef[i]) {
            switch (i)
            {
            case 0:
                printf("+ %d", P.coef[i]);
                break;
            case 1:
                if (P.coef[i] == 1) printf("+ x ");
                    else printf("+ %dx ", P.coef[i]);
                break;
            default:
                if (P.coef[i] == 1) printf("+ x^%d ", i);
                    else printf("+ %dx^%d ", P.coef[i], i);
                break;
            }
        } // else printf("+ 0");
    }
    printf("\n");
}
void normalise(polynomial * P) { // TO DO: realloc memory 
    if (P->grad == 0) return;   // what could be normalised on a monomial? 

    int first_zero = P->grad;
    while (P->coef[first_zero] == 0) {
        first_zero--;
    }
    if (first_zero != P->grad) {
        P->grad = first_zero;
    }
}

tables load_gf256() {
    /*
    *    check https://en.wikiversity.org/wiki/Reed%E2%80%93Solomon_codes_for_coders#Multiplication_with_logarithms)
    */
    tables t;
    t._exp = (uchar *) calloc(512, sizeof(uchar));
    t._log = (uchar *) calloc(256, sizeof(uchar));
    unsigned x = 1;
    for (int i = 0; i < 255; i++) {
        t._exp[i] = x;
        t._log[x] = i;
        x <<= 1;            // <=>  x *= 2;
        if (x & 0x100) {    // <=>  x >= 256;
            x ^= 0x11d;     // <=>  x %= 285;
        }
    }
    for (int i = 255; i < 512; i++) {
        t._exp[i] = t._exp[i-255];
    }

    return t;
}

/* polynomial stuff */

/* This function passes ownership of the returned polynomial.
 * If val is not NULL, it copies the contents of val into the polynomial. */
polynomial poly_init(int n, const int * val) {
    polynomial P;
    P.grad = n;
    P.coef = (int *)calloc(n+1, sizeof(int));
    if (val) {
        memcpy(P.coef, val, (n+1) * sizeof(int));
    }

    return P;
}

/* This function passes ownership of the allocated polynomial */
polynomial poly_sum(polynomial A, polynomial B) {
    polynomial sum = poly_init(A.grad>B.grad?A.grad:B.grad, NULL);

    for (int i = 0; i <= A.grad; i++) {
        sum.coef[i + sum.grad - A.grad] = A.coef[i];
    }
    for (int i = 0; i <= B.grad; i++) {
        sum.coef[i + sum.grad - B.grad] ^= B.coef[i];
    }
    return sum;
}

/* This function passes ownership of the returned polynomial */
polynomial poly_multiplication(polynomial P1, polynomial P2, tables t) {
    polynomial P3 = poly_init(P1.grad + P2.grad, NULL);
        for (int i = 0; i <= P1.grad; i++) {
            for (int j = 0; j <= P2.grad; j++) {
                if (P1.coef[i] && P2.coef[j]) {
                    P3.coef[i+j] ^= t._exp[t._log[P1.coef[i]] + t._log[P2.coef[j]]];
                } 
            }
        }

    return P3;
}
polynomial poly_division(polynomial P1, polynomial P2, tables t) {
    invert_int_array(P1.coef, P1.grad);
    invert_int_array(P2.coef, P2.grad);

    polynomial msg_out = P1;
    int normalizer = P2.coef[0], coef = 0;
    for (int i = 0; i <= (P1.grad - P2.grad); i++) {
        msg_out.coef[i] /= normalizer;
        coef = msg_out.coef[i]; 
        if (coef) {
            for (int j = 1; j <= P2.grad; j++) {
                if (P2.coef[j]) {
                    msg_out.coef[i + j] ^= t._exp[t._log[P2.coef[j]] + t._log[coef]];
                }
            }
        }
    }
    int sep = msg_out.grad-P2.grad;
    polynomial remainder = poly_init(P2.grad - 1, msg_out.coef+sep+1);
    invert_int_array(remainder.coef, remainder.grad);

    return remainder;
}
polynomial generator(int n) {
    tables table = load_gf256();

    polynomial P = poly_init(0, NULL);
    P.coef[0] = 1;

    polynomial aux = poly_init(1,NULL);
    aux.coef[1] = 1;

    for (int i = 0; i < n; i++) {
        aux.coef[0] = (int)table._exp[i] ;
        polynomial P_aux = poly_multiplication(P, aux, table);
        free_polynomial(&P);
        P = P_aux;
    }

    // free memory
    free_tables(&table);
    free_polynomial(&aux);

    return P;
}

/* This function passes ownership of the returned polynomial */
polynomial reed_solomon(polynomial M, int nerc) {       // nerc = number of error correction codewords
    tables t = load_gf256();                            // exponential and logarithmic values in GF(256)
    polynomial G = generator(nerc);                     // generator polynomial

    // To make sure that the exponent of the lead term doesn't become too small during the division, multiply
    // the message polynomial by x^n
    polynomial aux = poly_init(nerc, NULL);
    aux.coef[nerc] = 1;
    polynomial M2 = poly_multiplication(M, aux, t);

    polynomial EC = poly_division(M2, G, t);             // error correction polynomial
    // polynomial msg_out = poly_sum(M, EC);            // encoded message

    // free memory
    free_tables(&t);
    free_polynomial(&aux);
    free_polynomial(&M2);
    free_polynomial(&G);

    return EC;
} 
// dg