

#include "deserialize.h"
#include <string.h> 
#include "types.h"
#include "../common/buffer.h"
//#include "../globals.h"

enum { 
	L_JC_DEP = 0,
	L_JC_ADDR,
	L_JC_AGE
};


parser_status_e transaction_join_consensus_deserialize(buffer_t *buf, transaction_tx_t *tx) {
	parser_status_e ret = PARSING_LENGTH_ERROR;
	transaction_tx_join_consensus_t *join_consensus = &tx->join_consensus;


	switch (tx->stack_index) {
		case L_JC_DEP:	
			PRINTF("*****************start transaction join consensus deserialize*************\n");
			tx->stack_index = L_JC_DEP;
			if(join_consensus->deposit == NULL && (join_consensus->deposit = buffer_malloc(32)) == NULL)  {
				PRINTF("buffer_malloc not enough space\n");

				return PARSING_ERROR;
			}
			if((ret = transaction_deserialize_spc_field(buf, "deposit", 32, join_consensus->deposit)) != PARSING_OK) {
				return ret;	
			}
		case L_JC_ADDR:
			tx->stack_index = L_JC_ADDR;
			if((ret = transaction_deserialize_spc_field(buf, "address", 23, NULL)) != PARSING_OK) {
					return ret;	
			}
		case L_JC_AGE:
			tx->stack_index = L_JC_AGE;
			if((ret = transaction_deserialize_spc_field(buf, "agentHash", 32, NULL)) != PARSING_OK) {
				return ret;	
			}
		default:
			PRINTF("*****************end transaction join consensus deserialize*************\n");
			return PARSING_OK;

		}		

}


