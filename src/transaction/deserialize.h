#pragma once

#include "types.h"
#include "../common/buffer.h"

/**
 * Deserialize raw transaction in structure.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialized transaction.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */


parser_status_e transaction_deserialize_spc_field(buffer_t *buf, char *field, uint64_t len,  uint8_t *ptr) ;


parser_status_e transaction_deserialize_var_field(buffer_t *buf, char *field, uint64_t *value,  uint8_t *ptr);


parser_status_e transaction_deserialize(buffer_t *buf, transaction_t *tx);

parser_status_e transaction_transfer_deserialize(buffer_t *buf, transaction_tx_t *tx);

parser_status_e transaction_join_consensus_deserialize(buffer_t *buf, transaction_tx_t *tx);

parser_status_e transaction_leave_consensus_deserialize(buffer_t *buf, transaction_tx_t *tx);

parser_status_e transaction_cross_chain_transfer_deserialize(buffer_t *buf, transaction_tx_t *tx);

parser_status_e transaction_call_contract_deserialize(buffer_t *buf, transaction_tx_t *tx);



