#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

/**
 * Convert public key to address.
 *
 * address = Keccak256(public_key)[12:32] (20 bytes)
 *
 * @param[in]  public_key
 *   Pointer to byte buffer with public key.
 *   The public key is represented as 64 bytes with 32 bytes for
 *   each coordinate.
 * @param[out] out
 *   Pointer to output byte buffer for address.
 * @param[in]  out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
//bool address_from_pubkey(const uint8_t public_key[static 64], uint8_t *out, size_t out_len);

unsigned short nuls_dispaly_address_to_encoded_base58(
        uint8_t WIDE *nulsRipemid160, //chainId + addresstype + ripemid160
        char *out_addressBase58);

unsigned short nuls_address_to_encoded_base58(
        uint8_t WIDE *nulsRipemid160, //chainId + addresstype + ripemid160
        char *out_addressBase58);

bool  address_from_pubkey (
        uint8_t *compressed_public_key,
        uint16_t chain_id, 
        uint8_t address_type,
        uint8_t *out_address,
        char *out_addressBase58);

