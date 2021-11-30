#include "api.h"

#include <cstring>
#include <cstdio>

int crypto_kem_keypair (unsigned char *pk, unsigned char *sk)
{
    return keygen_bytes (pk, sk);
}

int crypto_encaps (unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk, const unsigned char *pk) 
{
    unsigned char mcopy[mlen];
    memcpy (mcopy, m, mlen);

    unsigned char skcopy[CRYPTO_SECRETKEYBYTES];
    memcpy (skcopy, sk, CRYPTO_SECRETKEYBYTES);

    unsigned char pkcopy[CRYPTO_PUBLICKEYBYTES];
    memcpy (pkcopy, pk, CRYPTO_PUBLICKEYBYTES);

    return encaps_from_keybytes (sm, smlen, mcopy, mlen, skcopy, pkcopy);
}

int crypto_decaps (unsigned char *m, unsigned long long *mlen,
                const unsigned char *sm, unsigned long long smlen,
                const unsigned char *pk)
{
    unsigned char pkcopy[PUBLICKEYBYTES];
    memcpy (pkcopy, pk, PUBLICKEYBYTES);

    unsigned char smcopy[smlen];
    memcpy (smcopy, sm, smlen);

    return decaps_from_bytes (m, mlen, smcopy, smlen, pkcopy);
}
