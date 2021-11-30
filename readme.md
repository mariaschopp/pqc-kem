
# PQKEM
A Post Quantum KEM, with code borrowed from Scabbard (Mera, et al) and intended to be paired with the DSA by Kirshanova, et al.

Brief summary:

* Algorithm 1 Key Generation
Input: l > 1, q > p, t
Output: pk = (A, b), sk = (s, H(pk), r)

1: A ← Rl×lq
2: r ← U({0, 1} ^ 256)
3: s ← βn((Rnq)l)
4: b = Round (A · s + h1)
5: pk ← (A, b)
6: sk ← (s, H(pk), r)
7: return pk = (A, b), sk = (s, H(pk), r)

* Algorithm 2 Encapsulation
Input: pk =(A, b)
Output: cipher_txt = (c', b'), key = K

1: m' ← U({0, 1} ^ 256)
2: (K', r') ← G(m∥H(pk))
//3: r' = U({0, 1} ^ 256)
4: s' ← βn((Rnq)l)
5: b' = Round (AT · s' + h1)
6: u' = bT · (s' mod p)
7: c' = Round (u' + h3 - 2 ^ (ep - B)m)
8: K ← H(K', H(c'))
9: return cipher_txt = (c', b'), key = K

* Algorithm 3 Decapsulation
Input: pk = (A, b), sk = (s, H(pk), r), cipher_txt = (c', b')
Output: key = K

1: u = b · (s mod p)
2: m'1 = Round (u + h2 - 2 ^ (ep - et - B) m)
3: (K'1, r'1) ← G(m2∥H(pk))
4: s'1 ← βn((Rnq)l)
5: b'1 = Round (AT · s'1 + h1)
6: u'1 = bT · (s'1 mod p)
7: c'1 = Round (u'1 + h3 - 2 ^ (ep - B) m)
8: if c' == c'1 then
9: return K = H(K'1, H(c'))
10: else
11: return K = H(r, H(c'))

# Requirements
- gcc compiler >= 9.2.1
- OpenSSL library development package
- NTL library

# Instructions to compile, install and run tests

*TODO (base on signature)

# Files
* pqkem.h: primary header file for user as external library (see section below)
* kem.c, kem.h: implementation of the full key encapsulation algorithm (basic Florete algorithms, using PQS utilities).
* keygen.c, keygen.h: implementation of the key generation algorithm (Algorithm 1, p. 5 of paper).
* encaps.c, encaps.h: implementation of the key encapsulation algorithm (Algorithm 2, p. 5 of paper).
* decaps.c, decaps.h: implementation of the key decapsulation algorithm (Algorithm 3, p. 5 of paper).
* utils/
    * utils.h: contains all utils without having to include each separately.
    * params.h: parameters for the protocol.
    * randombytes.c, randombytes.h: generates random bytes.
    * fips202.c, fips202.h: implementations of SHAKE used in random generation.
    * arith.c arith.h: arithmetic operations.
    * poly_mul.c, poly_mul.h: functions for multiplying polynomials.
    * core.c core.h: sampling, challenge generation, and other supporting functions.
    * matrix_A.h: fixed matrix A used in KAT generation and testing correctness.
    * serialize.c serialize.h: convert between key / signature structures and byte arrays.
* test/
    * cpucycles.c, cpucycles.h: performance benchmarking.
    * getCPUTime.c: returns internal time, used in cpucycles.c.
    * test_config.h: config options for testing.
    * pqkem_test.h pqs_test.c: test code.
* KAT/
    * api.h, api.c: api expected by PQCgenKAT_sign.
    * rng.c rng.h: random generation for random messages and random secret bytes.
    * PQCgenKAT_kem: generate req (request) and rsp (response) files for KAT.

# Usage as an external library

*TODO (base on signature)