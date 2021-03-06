#include "serialize.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

// Serialize and deserialize functions. 
// There is no way to verify that arrays are large enough so it is up to the calling function to pass in sufficiently sized arrays.

void serialize_secretkey (sk_t &sk, unsigned char *sk_bytes) {
    serialize_polyvecl (sk.s, sk_bytes, PQKEM_nu);
    memcpy (sk_bytes+sizeof(polyvecl), sk.H_pk, 32);
    memcpy (sk_bytes+sizeof(polyvecl)+32, sk.r, 32);
}

void deserialize_secretkey (sk_t &sk, unsigned char *sk_bytes) {
    deserialize_polyvecl (sk.s, sk_bytes, PQKEM_nu);
    memcpy (sk.H_pk, sk_bytes+sizeof(polyvecl), 32);
    memcpy (sk.r, sk_bytes+sizeof(polyvecl)+32, 32);
}

void serialize_publickey (pk_t &pk, unsigned char *pk_bytes) {
    int i, j, array_index = 0;

    for (i = 0; i < PQKEM_k; i++) {
        poly p = pk.b.polynomial[i];
        for (j = 0; j < PQKEM_n; j++) {
            pk_bytes[array_index++] = (p.coeffs[j] & 0xff000000) >> 24;
            pk_bytes[array_index++] = (p.coeffs[j] & 0xff0000) >> 16;
            pk_bytes[array_index++] = (p.coeffs[j] & 0xff00) >> 8;
            pk_bytes[array_index++] = (p.coeffs[j] & 0xff); 
        }
    }

    serialize_polyvecl (pk.b, pk_bytes+sizeof(polymatkl), PQKEM_mu);
}

void deserialize_publickey (pk_t &pk, unsigned char *pk_bytes) {
    int i, j, array_index = 0;

    memset (&(pk.b), 0, sizeof(pk.b));

    for (i = 0; i < PQKEM_k; i++) {
        for (j = 0; j < PQKEM_n; j++) {
            pk.b.polynomial[i].coeffs[j] = pk_bytes[array_index++] << 24;
            pk.b.polynomial[i].coeffs[j] |= pk_bytes[array_index++] << 16;
            pk.b.polynomial[i].coeffs[j] |= pk_bytes[array_index++] << 8;
            pk.b.polynomial[i].coeffs[j] |= pk_bytes[array_index++];
        }
    }

    deserialize_polyvecl (pk.b, pk_bytes+sizeof(polymatkl), PQKEM_mu);
}
/*
unsigned long long pack_signed_message (unsigned char *sm, signat_t &sig, unsigned char *m, unsigned long long mlen) {
    poly c = sig.c;
    polyvecl z = sig.z;
    int i, j;
    unsigned long long byte_index;

    memset (sm, 0, mlen + SIGNATUREBYTES);
    if (mlen != 0)
        memcpy (sm, m, mlen);
    byte_index = mlen;

    for (i = 0; i < PQKEM_n; i++) {
        sm[byte_index++] = c.coeffs[i] >> 24;
        sm[byte_index++] = (c.coeffs[i] & 0xff0000) >> 16;
        sm[byte_index++] = (c.coeffs[i] & 0xff00) >> 8;
        sm[byte_index++] = c.coeffs[i] & 0xff;
    }

    for (i = 0; i < PQKEM_l; i++) {
        poly p = z.polynomial[i];
        for (j = 0; j < PQKEM_n; j++) {
            sm[byte_index++] = p.coeffs[j] >> 24;
            sm[byte_index++] = (p.coeffs[j] & 0xff0000) >> 16;
            sm[byte_index++] = (p.coeffs[j] & 0xff00) >> 8;
            sm[byte_index++] = p.coeffs[j] & 0xff;
        }
    }

    return byte_index;
}


unsigned long long unpack_signed_message (unsigned char *sm, signat_t &sig, unsigned char *m, unsigned long long smlen) {
    int i, j;
    unsigned long long mlen, byte_index;

    memset (m, 0, smlen);
    mlen = smlen - SIGNATUREBYTES;

    memcpy (m, sm, (size_t) mlen);
    byte_index = mlen;

    for (i = 0; i < PQKEM_n; i++) {
        sig.c.coeffs[i] = sm[byte_index++] << 24;
        sig.c.coeffs[i] |= sm[byte_index++] << 16;
        sig.c.coeffs[i] |= sm[byte_index++] << 8;
        sig.c.coeffs[i] |= sm[byte_index++];
    }

    for (i = 0; i < PQKEM_l; i++) {
        for (j = 0; j < PQKEM_n; j++) {
            sig.z.polynomial[i].coeffs[j] = sm[byte_index++] << 24;
            sig.z.polynomial[i].coeffs[j] |= sm[byte_index++] << 16;
            sig.z.polynomial[i].coeffs[j] |= sm[byte_index++] << 8;
            sig.z.polynomial[i].coeffs[j] |= sm[byte_index++];
        }
    }

    return byte_index - SIGNATUREBYTES;
}


void printBstr(char *lead, unsigned char *bstr, unsigned int blen)
{
	unsigned int i;

	printf("%s\n", lead);

	for (i=0; i < blen; i++) {
        if (i != 0 && (i%32) == 0)
            printf ("\n");
        else if (i != 0 && (i%4) == 0)
            printf (" ");

		printf("%02x", bstr[i]);
    }

	printf("\n\n");
}
*/