#ifndef PQKEM_ENCAPS_H
#define PQKEM_ENCAPS_H

#include <cstdio>
#include <cstring>

#include "utils/utils.h"

void encaps(sk_t, pk_t, ctxt_t);

void generate_s_p (polyvecl &s_p);
void calculate_b_p (polyvecl &b_p, polymatkl &A, polyvecl &s_p);
void calculate_u_p (poly &u_p, polyvecl &b, polyvecl &s_p);
void calculate_c_p (poly &c_p, poly &u_p, unsigned char* m);

void debug_encaps (pk_t &pk);

#endif