#include "arith.h"
#include "poly_mul.h"
#include <cstring>

// ARITHMETIC FUNCTIONS

// Multiply a k*l matrix by an l-vector, returning a k-vector.
void mult_MatVecl(polyveck &res, polymatkl A, polyvecl v) {
    int i, j;
    poly acc;
    memset(&res, 0, sizeof(polyveck));
    for (i=0; i<PQKEM_k; i++){
        for (j=0; j<PQKEM_l; j++){
            MULT_POLYPOLY(A.elem[i][j], v.polynomial[j], acc, PQKEM_n, PQKEM_q);
            add_PolyPoly (res.polynomial[i], res.polynomial[i], acc);
        }
    }
}

// Transpose a (square) polymatkl, returning a polymatkl.
void transpose_polymatll(polymatkl AT, polymatkl A) {
    for (int i=0; i<PQKEM_l; i++){
        for (int j=0; j<PQKEM_l; j++){
            AT.elem[i][j] = A.elem[j][i];
        }
    }
}

// Add polynomial p1 to polynomial p2.
void add_PolyPoly(poly &res, poly p1, poly p2) {
    for (int n=0; n<PQKEM_n; n++)
        res.coeffs[n] = modq(p1.coeffs[n] + p2.coeffs[n]);
}

// Subtract polynomial p2 from polynomial p1.
void subt_PolyPoly(poly &res, poly p1, poly p2) {
    for (uint32_t i=0; i<PQKEM_n; i++)
        res.coeffs[i] = modq(p1.coeffs[i] - p2.coeffs[i]);
}

// Round an integer PQKEM_q -> PQKEM_p.
int32_t round(int32_t x) {
    uint32_t h = 1 << (PQKEM_nu - PQKEM_mu - 1);
    return (x + h) >> (PQKEM_nu - PQKEM_mu);
}

// Return the bits of an integer between positions j and i.
int32_t bits(int32_t x, uint32_t i, uint32_t j) {
    int32_t mask = ((1 << (i - j)) - 1) << j;
    return x & mask;
}

// Reduce x to the range (-q/2, q/2].
int32_t modq_neg(int32_t x) {
    int32_t y = modq(x);
    return y - (y >> (PQKEM_nu-1)) * PQKEM_q;
}

// Reduce x to the range [0,p].
int32_t modp(int32_t x) {
    return x & (PQKEM_p - 1);
}

// Reduce x to the range [0,q].
int32_t modq(int32_t x) {
    return x & (PQKEM_q - 1);
}