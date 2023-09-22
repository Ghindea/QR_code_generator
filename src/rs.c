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
        // int *tmp = realloc(P->coef, (first_zero+1) * sizeof(int));   
        // P->coef = tmp;
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
/* This function passes ownership of the allocated polynomial if val == NULL */
polynomial poly_init(int n, int * val) {
    polynomial P;
    P.grad = n;
    if (!val) {
        P.coef = (int *)calloc(n+1, sizeof(int));
    } else {
        P.coef = val;
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

/* This function passes ownership of the allocated polynomial */
polynomial poly_multiplication(polynomial P1, polynomial P2, tables t) {
    polynomial P3 = poly_init(P1.grad + P2.grad, NULL);
        for (int i = 0; i <= P1.grad; i++) {
            for (int j = 0; j <= P2.grad; j++) {
                if (P1.coef[i] && P2.coef[j]) {
                    P3.coef[i+j] ^= t._exp[t._log[P1.coef[i]] + t._log[P2.coef[j]]];
                } 
            }
        }
    // normalise(&P3); // leading term must be non-zero

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
    //polynomial remainder = poly_init(sep, msg_out.coef+sep+1);
    polynomial remainder = poly_init(P2.grad - 1, msg_out.coef+sep+1);
    invert_int_array(remainder.coef, remainder.grad);

    return remainder;
}
polynomial generator(int n) {
    tables table = load_gf256();
    //int tmp[]={1};
    //polynomial P = poly_init(0,tmp);
    polynomial P;
    P.grad = 0;
    P.coef = calloc(1, sizeof(int));
    P.coef[0] = 1;

    polynomial aux = poly_init(1,NULL);
    aux.coef[1] = 1;

    for (int i = 0; i < n; i++) {
        aux.coef[0] = (int)table._exp[i] ;
        polynomial P_aux = poly_multiplication(P, aux, table);
        free(P.coef);
        P = P_aux;
    }
    // polynomial P = poly_init(n,NULL);
    // for (int i = 0; i <= n; i++) {
    //     int termen = 1;
    //     for (int j = 0; j < i; j++) {
    //         termen = (termen * 2) % 256;         // ChatGPT was given a chance. it failed...
    //     }
    //     P.coef[i] = termen;
    // }

    // I'm sure ChatGPT would also free some memory here
    free(table._exp);
    free(table._log);
    free(aux.coef);

    return P;
}

polynomial reed_solomon(polynomial M, int nerc) {       // nerc = number of error correction codewords
    tables t = load_gf256();                            // exponential and logarithmic values in GF(256)
    polynomial G = generator(nerc);                     // generator polynomial

    int *aux_coef = (int *)calloc(nerc+1, sizeof(int));
    aux_coef[nerc] = 1;                                 // To make sure that the exponent of the lead term
    polynomial aux = poly_init(nerc, aux_coef);         // doesn't become too small during the division, multiply
    M = poly_multiplication(M, aux, t);                 // the message polynomial by x^n

    polynomial EC = poly_division(M, G, t);             // error correction polynomial
    // polynomial msg_out = poly_sum(M, EC);            // encoded message

    // Don't forget to free memory after you no longer need it!
    free(t._exp);
    free(t._log);
    free(aux_coef);

    return EC;
} 
// dg