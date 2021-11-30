#ifndef PQKEM_DECAPS_H
#define PQKEM_DECAPS_H

#include <cstdio>
#include <cstring>

#include "utils/utils.h"

void decaps(pk_t &pk, sk_t &sk, ctxt_t &ctxt);

void calculate_u (poly &u, polyvecl &b_p, polyvecl &s);
void calculate_m_1_p (poly &m_1_p, poly &u);
void generate_s_1_p (polyvecl &s_1_p);
void calculate_b_1_p (polyvecl &b_1_p, polymatkl &A, polyvecl &s_1_p);
void calculate_u_1_p (poly &u_1_p, polyvecl &b, polyvecl &s_1_p);
void calculate_c_1_p (poly &c_1_p, poly &u_1_p, poly &m_1_p);

#endif