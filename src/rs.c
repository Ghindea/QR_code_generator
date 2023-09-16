#include "header.h"

int max(int a, int b) {
    return a>b?a:b;
}
int min(int a, int b) {
    return a<b?a:b;
}
/* polynomial stuff */

polynomial polinit(int n, unsigned char * val) {
    polynomial P;
    P.grad = n;
    if (!val) {
        P.coef = (unsigned char *)calloc(n+1, sizeof(unsigned char));
    } else {
        P.coef = val;
    }

    return P;
}
polynomial poly_mul(polynomial P1, polynomial P2) {
    int k = 0, mul = 0;
    polynomial P3 = polinit(P1.grad + P2.grad, NULL);
    for (int i = 0; i <= max(P1.grad,P2.grad); i++) {
        for (int j = 0; j <= min(P1.grad,P2.grad); j++) {
            k = i + j;
            mul = P1.coef[i] * P2.coef[j];
            if (mul > 255) {
                mul = mul ^ 285;
            }
            P3.coef[k] = P3.coef[k] ^ mul;
        }
    }
    return P3;
}
polynomial generator(int n) {
    unsigned char *p = gf256();
    unsigned char tmp[]={-p[0],1};
    polynomial P = polinit(1,tmp), aux = polinit(1,NULL);
    aux.coef[1] = 1;
    for (int i = 1; i < n; i++) {
        aux.coef[0] = -p[i];
        P = poly_mul(P,aux);
    }
    return P;
}

unsigned char * gf256() {       // log table
    unsigned char * v = (unsigned char *) calloc(256, sizeof(unsigned char));
    unsigned int x = 1;
    v[0] = 1;
    for (int i = 0; i < 255; i++) {
        x = x << 1;
        if (x > 255) {
            x = x ^ 285;
        }
        v[i+1] = (unsigned char) x;
    }
    return v;
}
unsigned char * antilog(unsigned char *v) {     // antilog table
    unsigned char * alog = (unsigned char *) calloc(256, sizeof(unsigned char));
    for (int i = 1; i <= 255; i++) {
        for (int j = 0; j <= 255; j++) {
            if (v[j] == i) alog[i] = j;
        }
    }
    alog[1] = 0;
    return alog;
}

void polyprint(polynomial P) {
    for (int i = 0; i < P.grad; i++) {
        printf("%dx^%d+", P.coef[i], i);
    }
    printf("%dx^%d\n", P.coef[P.grad], P.grad);
}