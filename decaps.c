#include "decaps.h"

void decaps(pk_t pk, sk_t sk, ctxt_t ctxt){
    // Calculate u.
    poly u;
    calculate_u (u, ctxt.b_p, sk.s);

    // Calculate m_1_p.
    poly m_1_p;
    calculate_m_1_p (m_1_p, u);

    // TODO
    // Hash m and hash of pk into K_1_p (r_1_p appears unused).
    //unsigned char* r_1_p[32];
    unsigned char m_cat_H_pk[64];
    //memcpy (m_cat_H_pk, m_1_p, 32);
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
    calculate_c_1_p (c_1_p, u_1_p, m_1_p);

    // Validate and calculate K.
    // TODO
    /*
    char* K[32];
    for (int i=0; i<PQKEM_n; i++) {
        if (c_1_p.coeffs[i] != c_p.coeffs[i]) {
            K = shake256();
            return K;
        }
    } else {
        return ;
    }
    */
}


void calculate_u (poly u, polyvecl b_p, polyvecl s) {
    poly acc;
    for (int l=0; l<PQKEM_l; l++){
        MULT_POLYPOLY(b_p.polynomial[l], s.polynomial[l], acc, PQKEM_n, PQKEM_p);
        add_PolyPoly (u, u, acc);
    }
}

void calculate_m_1_p (poly m_1_p, poly u){
    // TODO
}

void generate_s_1_p (polyvecl s_1_p){
    unsigned char seed[SEEDBYTES];
    randombytes (seed, SEEDBYTES);
    for (int l_i=0; l_i<PQKEM_l; l_i++) {
        poly p;
        cbd (p, PQKEM_nu, PQKEM_n, seed);
        s_1_p.polynomial[l_i] = p;
    }
}

void calculate_b_1_p (polyveck &b_1_p, polymatkl &A, polyvecl &s_1_p){
    // Multiply AT and s.
    polymatkl AT;
    transpose_polymatll(AT, A);
    mult_MatVecl(b_1_p, AT, s_1_p);

    // Add h1 and round each coefficient in each polynomial.
    uint32_t i, j;
    for (i=0; i<PQKEM_l; i++) {
        for (j=0; j<PQKEM_n; j++) {
            b_1_p.polynomial[i].coeffs[j] = round(b_1_p.polynomial[i].coeffs[j] + PQKEM_h1_coeff);
        }
    }
}

void calculate_u_1_p (poly u_1_p, polyvecl b, polyvecl s_1_p){

}

void calculate_c_1_p (poly u_1_p){

}