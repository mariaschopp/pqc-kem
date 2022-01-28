#include <cstdio>
#include <cstdlib>

#include "pqkem_test.h"
//#include <pqkem/pqkem.h>    // can be used once lib has been installed
#include "../pqkem.h"         // can be used if lib is not installed

#include "cpucycles.h"
#include "getCPUTime.c"

#define TEST 1

int main (int argc, char **argv) {
    // Detailed output for each test, small number of tests.
    #if TEST_MODE == 1
    mode_1();

    // For large number of tests.
    #elif TEST_MODE == 2
    mode_2();

    // Test PQKem performance as Kyber does.
    #elif TEST_MODE == 3
    mode_3();

    #endif

    return 0;
}

void mode_1() {
    double tm;

    tm = getCPUTime();
    for(int loop=0; loop<NTESTS; ++loop){

        pk_t pk;                 // Public key
        sk_t sk;                 // Secret key
        ctxt_t ctxt;             // Ciphertext
        unsigned char K_e[32];  // Shared Key from encaps
        unsigned char K_d[32];  // Shared Key from decaps
        memset(&sk, 0, sizeof(sk_t));

        fprintf (stderr, "Generating keypair ... ");
        tm = getCPUTime();
        keygen (sk, pk);
        fprintf (stderr, "Ok\n");
        fprintf (stderr, "keypair is generated in %f sec.\n", getCPUTime() - tm);

        tm = getCPUTime();
        encaps (pk, ctxt, K_e);
        fprintf (stderr, "Encapsulation Ok\n");
        fprintf (stderr, "Encapsulation is complete in %f sec.\n", getCPUTime() - tm);

        tm = getCPUTime();
        decaps (sk, pk, ctxt, K_d);
        fprintf (stderr, "Decapsulation Ok\n");
        fprintf (stderr, "Decapsulation is complete in %f sec.\n", getCPUTime() - tm);
    }
}

void mode_2() {

}

void mode_3() {

}