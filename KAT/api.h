#ifndef api_kem_h
#define api_kem_h

#include "../pqkem.h"
#include "../utils/utils.h"

#define CRYPTO_SECRETKEYBYTES SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES PUBLICKEYBYTES
#define CRYPTO_BYTES BYTES

#define CRYPTO_ALGNAME "kem"

int crypto_kem_keypair (unsigned char *pk, unsigned char *sk);

int crypto_encaps (unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk, const unsigned char *pk);

int crypto_decaps (unsigned char *m, unsigned long long *mlen,
                const unsigned char *sm, unsigned long long smlen,
                const unsigned char *pk);

#endif