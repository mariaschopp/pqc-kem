#ifndef PQKEM_KEYGEN_H
#define PQKEM_KEYGEN_H

#include "utils/utils.h"

void keygen(sk_t &sk, pk_t &pk);
int keygen_bytes (unsigned char *pk, unsigned char *sk);

void generate_A (polymatkl &A);
void generate_s (polyvecl &s);
void calculate_b (polyvecl &b, polymatkl &A, polyvecl &s);

void debug_keygen (sk_t &sk, pk_t &pk);

#endif