#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <lcx_sha256.h>
#include <lcx_sha3.h>

#include "constants.h"
#include "common/bip32.h"
#include "../transaction/types.h"


#define N_storage (*(volatile internalStorage_t *) PIC(&N_storage_real))



typedef struct internalStorage_t {
    unsigned char dataAllowed;
    unsigned char contractDetails;
    unsigned char displayNonce;
    uint8_t initialized;
} internalStorage_t;



/**
 * Enumeration for the status of IO.
 */
typedef enum {
    READY,     /// ready for new event
    RECEIVED,  /// data received
    WAITING    /// waiting
} io_state_e;


/**
 * Enumeration with parsing state.
 */
typedef enum {
    STATE_NONE,     /// No state
    STATE_PARSED,   /// Transaction data parsed
    STATE_APPROVED  /// Transaction data approved
} state_e;

/**
 * Enumeration with user request type.
 */
typedef enum {
    CONFIRM_ADDRESS,     /// confirm address derived from public key
    CONFIRM_TRANSACTION  /// confirm transaction information
} request_type_e;

/**
 * Structure for public key context information.
 */
typedef struct {
    uint8_t raw_public_key[65];  /// x-coordinate (32), y-coodinate (32)
    uint8_t chain_code[32];      /// for public key derivation
    uint8_t type;
	uint16_t chain_id;
    uint8_t address[23];
	uint8_t address_with_prefix[40];

	char address_base_58[33];

	uint8_t compressed_public_key[33];
} pubkey_ctx_t;

/**
 * Structure for transaction information context.
 */
typedef struct {
    uint8_t raw_tx[MAX_TRANSACTION_LEN];  /// raw transaction serialized
    size_t raw_tx_len;                    /// length of raw transaction
	

	transaction_t transaction;            /// structured transaction

	size_t message_len;

	uint8_t m_hash[32];                   /// message hash digest


	uint8_t signature[MAX_DER_SIG_LEN];   /// transaction signature encoded in DER
    uint8_t signature_len;                /// length of transaction signature
    uint8_t v;                            /// parity of y-coordinate of R in ECDSA signature
} transaction_ctx_t;

/**
 * Structure for global context.
 */

typedef struct {
    state_e state; 			 				/// state of the context
    request_type_e req_type; 				/// user request

	uint32_t bip32_path[MAX_BIP32_PATH]; 	/// BIP32 path
    uint8_t bip32_path_len;               	/// length of BIP32 path
    
	pubkey_ctx_t pk_info;					/// public key context
	transaction_ctx_t tx_info; 				 /// transaction context

	uint8_t display_step;

	union {
		cx_sha256_t cx_sha256;	
		cx_sha3_t cx_sha3;
	} cx_sha;

} global_ctx_t;


