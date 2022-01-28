#include <cstdio>

#include "encaps.h"

void encaps (pk_t &pk, ctxt_t &ctxt, unsigned char* K){
    // Generate random message m.
    unsigned char m[32];
    randombytes (m, 32);

    // Arrange message stem into message.
    // Skipping error correction for now -
    // not necessary in pure software implementation?

    // Hash message and key hash into K'.
    unsigned char m_cat_H_pk[64];
    memcpy (m_cat_H_pk, m, 32);
    unsigned char pk_bytes[PUBLICKEYBYTES];
    serialize_publickey (pk, pk_bytes);
    shake256 (m_cat_H_pk+32, 32, pk_bytes, PUBLICKEYBYTES);
    unsigned char K_p[32];
    shake256 (K_p, 32, m_cat_H_pk, 64);

    // Generate r.
    // This doesn't appear to ever be used?
    // randombytes (sk.r, 32);

    // Generate s'. (Centered Binomial Distribution)
    polyvecl s_p;
    generate_s_p (s_p);

    // Calculate b'.
    calculate_b_p (ctxt.b_p, pk.A, s_p);

    // Generate u'.
    poly u_p;
    memset(&u_p, 0, sizeof(poly));
    calculate_u_p (u_p, pk.b, s_p);

    // Calculate c'.
    calculate_c_p (ctxt.c_p, u_p, m);

    // Hash K' and hash of c' into K.
    unsigned char K_p_cat_H_c_p[64];
    memcpy(K_p_cat_H_c_p, K_p, 32);
    unsigned char c_p_bytes[sizeof(poly)];
    serialize_poly(ctxt.c_p, c_p_bytes, 32);
    shake256 (K_p_cat_H_c_p+32, 32, c_p_bytes, sizeof(poly));
    shake256 (K, 32, K_p_cat_H_c_p, 64);

    #ifdef DEBUG
    debug_encaps (pk);
    #endif
}


void generate_s_p (polyvecl &s_p) {
    unsigned char seed[32];
    randombytes (seed, SEEDBYTES);
    for (int l_i=0; l_i<PQKEM_l; l_i++) {
        poly p;
        cbd (p, PQKEM_nu, PQKEM_nu*PQKEM_n, seed);
        s_p.polynomial[l_i] = p;
    }
}


void calculate_b_p (polyvecl &b_p, polymatkl &A, polyvecl &s) {
    // Multiply AT and s.
    polymatkl AT;
    polyveck b_p_temp;
    transpose_polymatll(AT, A);
    mult_MatVecl(b_p_temp, AT, s);

    // Copy polynomials from b_p_temp to b_p.
    for (int i=0; i<PQKEM_k; i++){
        for (int j=0; j<PQKEM_n; j++){
            b_p.polynomial[i].coeffs[j] = b_p_temp.polynomial[i].coeffs[j];
        }
    }

    // Add h1 and round each coefficient in each polynomial.
    uint32_t i, j;
    for (i=0; i<PQKEM_k; i++) {
        for (j=0; j<PQKEM_n; j++) {
            b_p.polynomial[i].coeffs[j] = round(b_p.polynomial[i].coeffs[j] + PQKEM_h1_coeff);
        }
    }
}

void calculate_u_p (poly &u_p, polyvecl &b, polyvecl &s_p) {
    poly acc;
    for (int l=0; l<PQKEM_l; l++){
        MULT_POLYPOLY(b.polynomial[l], s_p.polynomial[l], acc, PQKEM_n, PQKEM_p);
        add_PolyPoly (u_p, u_p, acc);
    }
}


// TODO: How they define multiplication by m?     
void calculate_c_p (poly &c_p, poly &u_p, unsigned char* m) {
    // Calculate the m_term: 2^(e_p - B)*m.
    for (int b=0; b<32; b++) {
        //int m_term = (1 << (PQKEM_mu)) * m;
    }

    // Add h3, subtract the m_term, and round
    // each coefficient in the polynomial.
    uint32_t j;
    for (j=0; j<PQKEM_n; j++) {
        //c_p.coeffs[j] = round(c_p.coeffs[j] + PQKEM_h3_coeff - m_term, );
    }
}

// TODO
void debug_encaps (pk_t &pk) {
    printf ("Debugging encapsulation...\n");
    unsigned char pk_bytes[PUBLICKEYBYTES];

    char t_str[] = "t = ";

    serialize_publickey (pk, pk_bytes);

    printBstr (t_str, pk_bytes, PUBLICKEYBYTES);
}