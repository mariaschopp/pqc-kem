#include <cstdio>

#include "keygen.h"

void keygen (sk_t &sk, pk_t &pk) {
    // Generate r.
    randombytes (sk.r, 32);

    // Generate A.
    #ifdef REGEN_A
    generate_A (pk.A);
    #else
    pk.A = matrix_A;
    #endif

    // Generate s. (Centered Binomial Distribution)
    generate_s (sk.s);

    // Calculate b: b = round(A*s + h_1).
    calculate_b (pk.b, pk.A, sk.s);

    #ifdef DEBUG
    debug_keygen (sk, pk);
    #endif
}

// ALL FUNCTIONS BELOW DERIVED FROM SIGNATURE
// TODO: merge

// Generate keypair as byte arrays
int keygen_bytes (unsigned char *sk, unsigned char *pk) {
    sk_t sk_obj;
    pk_t pk_obj;

    deserialize_secretkey (sk_obj, sk);
    keygen(sk_obj, pk_obj);

    serialize_secretkey (sk_obj, sk);
    serialize_publickey (pk_obj, pk);

    return 0;
}

void generate_A (polymatkl &A) {
    // generation of A accounts for about half the runtime of keygen and sign
    unsigned char seed[SEEDBYTES];

    // if DEBUG is defined then write the matrix to a file as it's generated
    #ifdef DEBUG
    FILE *mat_file;
    mat_file = fopen ("matrix_A.h", "w");

    fprintf (mat_file, "#ifndef MATRIX_A\n");
    fprintf (mat_file, "#define MATRIX_A\n\n");
    #endif

    poly p;
    uint32_t h, i, n;
    for (h=0; h<PQKEM_k; h++) {
        for (i=0; i<PQKEM_l; i++) {
            #ifdef DEBUG
            fprintf (mat_file, "static poly poly_%i_%i[PQS_n] = { ", h, i);
            #endif
            rej_sample (p, PQKEM_q-1, BUFLEN_A, seed);
            for (n=0; n<PQKEM_n; n++) {
                if (p.coeffs[n] < 0) {
                    p.coeffs[n] = -p.coeffs[n];
                }

                if (h==0 && i == 0) {
                    p.coeffs[0] |= 1;
                    if (n > 0) {
                        p.coeffs[n] &= 0xFFFFFFFE;
                    }
                }

                #ifdef DEBUG
                if (n != PQKEM_n-1) {
                    fprintf (mat_file, "%7i, ", p.coeffs[n]);
                } else {
                    fprintf (mat_file, "%7i", p.coeffs[n]);
                }
                #endif
            }

            A.elem[h][i] = p;

            #ifdef DEBUG
            fprintf (mat_file, " };\n");
            #endif
        }
        #ifdef DEBUG
        fprintf (mat_file, "\n");
        #endif
    }

    #ifdef DEBUG
    fprintf (mat_file, "static polymatkl matrix_A = {");
    for (h=0; h<PQKEM_k; h++) {
        for (i=0; i<PQKEM_l; i++) {
            if ((h != PQKEM_k-1) | (i != PQKEM_l-1)) {
                fprintf (mat_file, "*poly_%i_%i, ", h, i);
            } else {
                fprintf (mat_file, "*poly_%i_%i", h, i);
            }
        }
    }

    fprintf (mat_file, "};\n\n");
    fprintf (mat_file, "#endif");
    fclose (mat_file);
    #endif
}

void generate_s (polyvecl &s) {
    unsigned char seed[SEEDBYTES];
    randombytes (seed, SEEDBYTES);
    for (int l_i=0; l_i<PQKEM_l; l_i++) {
        poly p;
        cbd (p, PQKEM_nu, PQKEM_n, seed);
        s.polynomial[l_i] = p;
    }
}

void calculate_b (polyvecl &b, polymatkl &A, polyvecl &s) {
    // Multiply A and s.
    polyveck b_temp;
    mult_MatVecl(b_temp, A, s);

    // Add h1 and round each coefficient in each polynomial.
    uint32_t i, j;
    for (i=0; i<PQKEM_l; i++) {
        for (j=0; j<PQKEM_n; j++) {
            b.polynomial[i].coeffs[j] = round(b_temp.polynomial[i].coeffs[j] + PQKEM_h1_coeff);
        }
    }
}


void debug_keygen (sk_t &sk, pk_t &pk) {
    printf ("Debugging keygen...\n");
    unsigned char sk_bytes[SECRETKEYBYTES];
    unsigned char pk_bytes[PUBLICKEYBYTES];

    char s_str[] = "s = ";
    char b_str[] = "b = ";

    serialize_secretkey (sk, sk_bytes);
    serialize_publickey (pk, pk_bytes);

    printBstr (s_str, sk_bytes, SECRETKEYBYTES);
    printBstr (b_str, pk_bytes, PUBLICKEYBYTES);
}