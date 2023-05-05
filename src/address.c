/*****************************************************************************
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "os.h"
#include "cx.h"

#include "address.h"
#include "base58.h"
#include "read.h"
#include "transaction/types.h"

/*
bool __address_from_pubkey(const uint8_t public_key[static 64], uint8_t *out, size_t out_len) {
    uint8_t address[32] = {0};
    cx_sha3_t keccak256;

    if (out_len < ADDRESS_LEN) {
        return false;
    }

    cx_keccak_init(&keccak256, 256);
    cx_hash((cx_hash_t *) &keccak256, CX_LAST, public_key, 64, address, sizeof(address));

    memmove(out, address + sizeof(address) - ADDRESS_LEN, ADDRESS_LEN);

    return true;
}
*/

static  void __address_from_pubkey(unsigned char *in, unsigned short inlen, unsigned char *out) {
  union {
      cx_sha256_t shasha;
      cx_ripemd160_t riprip;
  } u;
  unsigned char buffer[32];

  cx_sha256_init(&u.shasha);
  cx_hash(&u.shasha.header, CX_LAST, in, inlen, buffer, 32);
  cx_ripemd160_init(&u.riprip);
  cx_hash(&u.riprip.header, CX_LAST, buffer, 32, out, 20);
}


static uint8_t __xor(uint8_t *buffer, uint8_t length) {
  uint8_t xor = 0;
  for (int i = 0; i < length; i++) {
    xor = xor ^ buffer[i];
  }
  return xor;
}





unsigned short nuls_address_to_encoded_base58(
        uint8_t WIDE *nulsRipemid160, //chainId + addresstype + ripemid160
        char *out_addressBase58) {
  unsigned char tmpBuffer[24];
   size_t outputLen = 32;
   
  memmove(tmpBuffer, nulsRipemid160, 23);
  
  tmpBuffer[23] = __xor(tmpBuffer, 23);
  

  if (base58_encode(tmpBuffer, 24, out_addressBase58, outputLen) < 0) {
    THROW(EXCEPTION);
  }
  
  return outputLen;
}

unsigned short nuls_dispaly_address_to_encoded_base58(
        uint8_t WIDE *nulsRipemid160, //chainId + addresstype + ripemid160
        char *out_addressBase58) {
  unsigned char tmpBuffer[24];
  size_t outputLen = 32;

  uint16_t type = read_u16_le(nulsRipemid160, 0);
  uint8_t type_len;
   
  memmove(tmpBuffer, nulsRipemid160, 23);
  
  tmpBuffer[23] = __xor(tmpBuffer, 23);

  if(type == 9) {
	 	type_len = 6;
	  	strlcpy(out_addressBase58, "NERVEe", type_len + 1);
  } else {
		type_len = 5;
		strlcpy(out_addressBase58, "NULSd", type_len + 1);
  }


	
  if (base58_encode(tmpBuffer, 24, out_addressBase58 + type_len, outputLen) < 0) {
    THROW(EXCEPTION);
  }

  out_addressBase58[outputLen + type_len] = '\0';
  return outputLen;
}


bool address_from_pubkey (
        uint8_t *compressed_public_key,
        uint16_t chain_id, 
        uint8_t address_type,
        uint8_t *out_address,
        char *out_addressBase58) {
        
  out_address[0] = chain_id;
  out_address[1] = (chain_id >> 8);
  out_address[2] = address_type;
  
  __address_from_pubkey(compressed_public_key, 33, out_address + 3);
  
  nuls_address_to_encoded_base58(out_address, out_addressBase58);

  out_addressBase58[32] = '\0';

	return true;
}



