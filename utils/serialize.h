#ifndef PQKEM_SERIALIZE_H
#define PQKEM_SERIALIZE_H

#include "params.h"
#include "serialize_common.h"

void serialize_secretkey (sk_t &sk, unsigned char *sk_bytes);
void deserialize_secretkey (sk_t &sk, unsigned char *sk_bytes);

void serialize_publickey (pk_t &vk, unsigned char *pk_bytes);
void deserialize_publickey (pk_t &vk, unsigned char *pk_bytes);

#endif
