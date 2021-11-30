#ifndef PQKEM_SERIALIZE_H
#define PQKEM_SERIALIZE_H

#include "params.h"

void serialize_poly (poly &p, unsigned char *p_bytes);

void serialize_polyvecl (polyvecl &pv, unsigned char *pv_bytes);
void deserialize_polyvecl (polyvecl &pv, unsigned char *pv_bytes);

void serialize_secretkey (sk_t &sk, unsigned char *sk_bytes);
void deserialize_secretkey (sk_t &sk, unsigned char *sk_bytes);

void serialize_publickey (pk_t &vk, unsigned char *pk_bytes);
void deserialize_publickey (pk_t &vk, unsigned char *pk_bytes);

//unsigned long long pack_signed_message (unsigned char *sm, signat_t &sig, unsigned char *m, unsigned long long mlen);
//unsigned long long unpack_signed_message (unsigned char *sm, signat_t &sig, unsigned char *m, unsigned long long smlen);

void printBstr (char *lead, unsigned char* bstr, unsigned int blen);

#endif
