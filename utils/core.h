#ifndef PQKEM_COMMON_H
#define PQKEM_COMMON_H

#include "params.h"
#include "fips202.h"
#include <cstdint>

void polyw1_pack (uint8_t *r, const poly *a);
void challenge (poly &c, const uint8_t mu[CRHBYTES], polyveck &w1);

void genVecl (polyvecl &vec_s, int bound, uint32_t buflen, unsigned char *seed);
void rej_sample (poly &p, uint32_t bound, uint32_t buflen, unsigned char *seed);
void cbd (poly &p, uint32_t nu, uint32_t n_coeffs, unsigned char *seed);

#endif
