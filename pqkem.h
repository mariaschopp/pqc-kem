#ifndef PQKEM_H
#define PQKEM_H

// Interface of only the elements that we want to externally expose

// util structures, constants, and functions
#include "utils/params.h"
#include "utils/randombytes.h"

// keygen
void keygen (sk_t &sk, pk_t &pk);
int keygen_bytes (unsigned char *sk, unsigned char *pk);

// encapsulate
void encaps (pk_t &pk, ctxt_t &cip, unsigned char *K);
int encaps_from_keybytes (unsigned char *pk, unsigned char *c,
                          unsigned char *b, unsigned char *K);

// decapsulate
void decaps (sk_t &sk, pk_t &pk, ctxt_t &cip, unsigned char *K);
int decaps_from_keybytes (unsigned char *sk, unsigned char *pk,
                          unsigned char *c, unsigned char *b,
                          unsigned char *K);

// May not need these:
//uint64_t clock1,clock2;
//uint64_t clock_kp_mv,clock_cl_mv, clock_kp_sm, clock_cl_sm;

#endif
