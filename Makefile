CC       = g++
CFLAGS   = -c -Wall -O3
LDLIBS   = -lcrypto

OBJS     = keygen.o encaps.o decaps.o
TEST_OBJ = test/cpucycles.o
KAT_OBJ  = KAT/rng.o KAT/api.o
UTIL_OBJ = utils/arith.o utils/core.o utils/fips202.o utils/poly_mul.o utils/randombytes.o utils/serialize_common.o #utils/serialize.o
UTIL_HDR = utils/params.h utils/matrix_A.h utils/utils.h

INSTALL_PATH?=/usr/local

# GENERAL

all: $(OBJS) $(UTIL_OBJ)

#kat: $(KAT_OBJ) KAT/PQCgenKAT_kem
#	cd KAT; ./PQCgenKAT_kem

test: $(TEST_OBJ) test/pqkem_test
	./test/pqs_test

# if library has been installed this version can be used
#test: $(TEST_OBJ) test/pqkem_test -lpqkem
#	./test/pqkem_test

install: $(OBJS)
	ar rcs libpqkem.a $(OBJS); 
	@mkdir -p $(INSTALL_PATH);
	@cp -p libpqkem.a $(INSTALL_PATH)/lib/;
	@mkdir -p $(INSTALL_PATH)/include/pqkem; 
	@cp -p pqkem.h $(INSTALL_PATH)/include/pqkem;
	@mkdir -p $(INSTALL_PATH)/include/pqkem/utils;
	@chmod 664 utils/*.h
	@cp -p utils/params.h $(INSTALL_PATH)/include/pqkem/utils/;
	@cp -p utils/randombytes.h $(INSTALL_PATH)/include/pqkem/utils/;

# MODULES

keygen.o: keygen.c keygen.h pqkem.h $(UTIL_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ 

encaps.o: encaps.c encaps.h pqkem.h $(UTIL_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

decaps.o: decaps.c decaps.h pqkem.h $(UTIL_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# UTILITIES

utils/%.o: utils/%.c utils/%.h $(UTIL_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

# TESTS

test/pqs_test: $(OBJS) $(UTIL_OBJ) $(TEST_OBJ) test/pqkem_test.c test/pqkem_test.h test/test_config.h
	$(CC) $(OBJS) $(UTIL_OBJ) $(TEST_OBJ) test/pqkem_test.c -o test/pqkem_test

# KAT

KAT/rng.o: KAT/rng.c KAT/rng.h 
	$(CC) $(CFLAGS) -c $< -o $@

KAT/api.o: KAT/api.c KAT/api.h
	$(CC) $(CFLAGS) -c $< -o $@

KAT/PQCgenKAT_kem: $(OBJS) $(UTIL_OBJ) $(KAT_OBJ) KAT/PQCgenKAT_kem.c
	$(CC) $(OBJS) $(UTIL_OBJ) $(KAT_OBJ) KAT/PQCgenKAT_kem.c -o KAT/PQCgenKAT_kem $(LDLIBS)


# Clean

clean:
	rm -rf *.o utils/*.o test/pqkem_test *.a

clean_kat:
	rm -rf KAT/rng.o KAT/PQCgenKAT_kem

clean_test:
	rm -rf test/*.o test/pqkem_test
