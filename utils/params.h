#ifndef PQKEM_PARAMS_H
#define PQKEM_PARAMS_H

#include <cstdint>

#define PQKEM_n 256

#define PQKEM_k 3
#define PQKEM_l 3

#define PQKEM_mu 10
#define PQKEM_nu 13
#define PQKEM_p (1 << PQKEM_mu)
#define PQKEM_q (1 << PQKEM_nu)
#define PQKEM_B 4
#define PQKEM_t (1 << 3)

#define PQKEM_h1_coeff (1 << (PQKEM_nu - PQKEM_mu - 1))
#define PQKEM_h2_coeff (PQKEM_h1_coeff + 1 << (PQKEM_mu - PQKEM_B - 1) - 1 << (PQKEM_mu - PQKEM_t - 1) )
#define PQKEM_h3_coeff (1 << (PQKEM_nu - PQKEM_mu - 1))

#define PQKEM_s 4
#define PQKEM_s_bits 3
#define PQKEM_d 3

#define PQKEM_gamma 1048096
#define PQKEM_gamma_bits 20
#define PQKEM_omega 60
#define PQKEM_beta 240

#define CRHBYTES 48
#define SEEDBYTES 32
#define POLW1_SIZE_PACKED ((PQKEM_n*4)/8)

// Number of bytes needed for rejection sampling, precomputed from Hoeffding's inequality rounded up to nearest multiple of SHAKE_RATE
#define BUFLEN_A 840 
#define BUFLEN_gamma 840
#define BUFLEN_s 336 

#define MAXRANDOM 33554431

// Each element is an int32_t which is 4 bytes
#define SECRETKEYBYTES (4*PQKEM_n*PQKEM_l)
#define PUBLICKEYBYTES (4*PQKEM_n*PQKEM_k)
#define SIGNATUREBYTES (4*PQKEM_n + 4*PQKEM_n*PQKEM_l)

#define DEBUG
#define REGEN_A

// A polynomial in R_q, represented by a vector of coefficients (x^0, x^1, ..., x^(PQKEM_n-1))
typedef struct{
    int32_t coeffs[PQKEM_n];
} poly;

// A vector of length PQKEM_l of polynomials
typedef struct{
    poly polynomial[PQKEM_l];
} polyvecl;

// A vector of length PQKEM_k of polynomials
typedef struct{
    poly polynomial[PQKEM_k];
} polyveck;

// Matrix of size PQKEM_k*PQKEM_l of polynomials
typedef struct{
    poly elem[PQKEM_k][PQKEM_l];
} polymatkl;

// Secret key
typedef struct{
    polyvecl s;
    unsigned char H_pk[32];
    unsigned char r[32];
} sk_t;

// Public key
typedef struct{
    polymatkl A;
    polyvecl b;
} pk_t;

// Cipher text
typedef struct{
    poly c_p;
    polyvecl b_p;
} ctxt_t;

#endif
