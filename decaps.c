#include "decaps.h"

void decaps(pk_t pk, sk_t sk, ctxt_t ctxt, unsigned char *K){
    // Calculate u.
    poly u;
    calculate_u (u, ctxt.b_p, sk.s);

    // Calculate m_1_p.
    poly m_1_p;
    calculate_m_1_p (m_1_p, u);

    // Hash m and hash of pk into K_1_p (r_1_p appears unused).
    //unsigned char* r_1_p[32];
    unsigned char m_cat_H_pk[64];
    unsigned char m_1_p_bytes[sizeof(poly)];
    serialize_poly (m_1_p, m_1_p_bytes, sizeof(poly));
    memcpy (m_cat_H_pk, m_1_p_bytes, 32);
    unsigned char pk_bytes[PUBLICKEYBYTES];
    serialize_publickey (pk, pk_bytes);
    shake256 (m_cat_H_pk+32, 32, pk_bytes, PUBLICKEYBYTES);
    unsigned char K_1_p[32];
    shake256 (K_1_p, 32, m_cat_H_pk, 64);

    // Generate s_1_p.
    polyvecl s_1_p;
    generate_s_1_p (s_1_p);

    // Calculate b_1_p.
    polyvecl b_1_p;
    calculate_b_1_p (b_1_p, pk.A, s_1_p);

    // Calculate u_1_p.
    poly u_1_p;
    calculate_u_1_p (u_1_p, pk.b, s_1_p);

    // Calculate c_1_p.
    poly c_1_p;
    memset (&c_1_p, 0, sizeof(poly));
    calculate_c_1_p (c_1_p, u_1_p, m_1_p);

    // Validate and calculate K.
    unsigned char c_p_bytes[32];
    serialize_poly(ctxt.c_p, c_p_bytes, 32);
    unsigned char Kvr_cat_H_c_p[64];
    shake256 (Kvr_cat_H_c_p+32, 32, c_p_bytes, sizeof(poly));
    
    // As a default, write K_1_p to the buffer.
    memcpy (Kvr_cat_H_c_p, K_1_p, 32);

    // If validation fails, overwrite K_1_p with r.
    for (int i=0; i<PQKEM_n; i++) {
        if (c_1_p.coeffs[i] != ctxt.c_p.coeffs[i]) {
            memcpy (Kvr_cat_H_c_p, sk.r, 32);
        }
    }

    // Hash the buffer to K.
    shake256 (K, 32, Kvr_cat_H_c_p, 64);
}


void calculate_u (poly &u, polyvecl &b_p, polyvecl &s) {
    poly acc;
    for (int l=0; l<PQKEM_l; l++){
        MULT_POLYPOLY(b_p.polynomial[l], s.polynomial[l], acc, PQKEM_n, PQKEM_p);
        add_PolyPoly (u, u, acc);
    }
}

void calculate_m_1_p (poly &m_1_p, poly &u){
    // TODO (similar operations to calculate c_p in encaps)
}

void generate_s_1_p (polyvecl &s_1_p){
    unsigned char seed[SEEDBYTES];
    randombytes (seed, SEEDBYTES);
    for (int l_i=0; l_i<PQKEM_l; l_i++) {
        poly p;
        cbd (p, PQKEM_nu, PQKEM_n, seed);
        s_1_p.polynomial[l_i] = p;
    }
}

void calculate_b_1_p (polyvecl &b_1_p, polymatkl &A, polyvecl &s_1_p){
    // Multiply AT and s_1_p.
    polymatkl AT;
    polyveck b_1_p_temp;
    transpose_polymatll(AT, A);
    mult_MatVecl(b_1_p_temp, AT, s_1_p);

    // Add h1 and round each coefficient in each polynomial.
    uint32_t i, j;
    for (i=0; i<PQKEM_l; i++) {
        for (j=0; j<PQKEM_n; j++) {
            b_1_p.polynomial[i].coeffs[j] = round(b_1_p.polynomial[i].coeffs[j] + PQKEM_h1_coeff);
        }
    }
}

void calculate_u_1_p (poly &u_1_p, polyvecl &b, polyvecl &s_1_p){
    poly acc;
    for (int l=0; l<PQKEM_l; l++){
        MULT_POLYPOLY(b.polynomial[l], s_1_p.polynomial[l], acc, PQKEM_n, PQKEM_p);
        add_PolyPoly (u_1_p, u_1_p, acc);
    }
}

void calculate_c_1_p (poly &c_1_p, poly &u_1_p, poly &m_1_p){
    // See calculate_c_p in encaps.
}