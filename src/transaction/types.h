#pragma once

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include "../common/buffer.h"
#include "../common/uint256.h"

#ifdef UNIT_TEST
#define PRINTF(...)
#endif



#define MAX_COIN_LEN 5


typedef enum {
	PARSING_OK = 1,
	PARSING_ERROR = -1,
   	PARSING_LENGTH_ERROR = -2,
   	PARSING_VALUE_ERROR = -3,
} parser_status_e;


typedef struct  {
	uint8_t *deposit;
} transaction_tx_join_consensus_t  ;



typedef struct {
	uint64_t method_len;

	uint8_t *method_name;
	uint8_t *contract_address;
	uint8_t *value;

	uint64_t element_len;
	
	uint8_t *element1;
	uint8_t *element2;
	uint8_t *element3;

	uint8_t two_args_len;
	uint8_t two_array_index;

	uint8_t one_args_len;
	uint8_t one_array_index;
	uint8_t stack_index;
} transaction_tx_call_contract_t;


typedef struct {
	uint8_t stack_index;

	transaction_tx_join_consensus_t join_consensus;
	
	transaction_tx_call_contract_t call_contract;

} transaction_tx_t;


typedef parser_status_e (*transaction_parse_func)(buffer_t *buf, transaction_tx_t *tx);

typedef struct {  
	uint16_t transaction_type; 
	transaction_parse_func func;

} transaction_type_t;


typedef struct {
	uint8_t stack_index; 
	uint16_t assets_chain_id;
	uint16_t assets_id;
} transaction_coin_from_t;


typedef struct {
	uint8_t stack_index;
		
	uint64_t address_len;
	uint8_t  *address;

	uint16_t assets_chain_id;
	uint16_t assets_id;

	uint8_t  *amount;  
} transaction_coin_to_t;


typedef struct {
	uint8_t stack_index;	
	uint8_t array_index;

	uint256_t from_amount;

	uint8_t from_len;
	transaction_coin_from_t from[MAX_COIN_LEN + 1]; 

	uint256_t to_amount;

	uint8_t to_len;
	transaction_coin_to_t to[MAX_COIN_LEN + 1]; 
} transaction_coin_t;


typedef struct {
	uint8_t stack_index;					///parase transaction context stack

	transaction_type_t transaction_type; 	///transaction type

	transaction_tx_t tx_info; 
	
	transaction_coin_t coin_info; 
} transaction_t;


