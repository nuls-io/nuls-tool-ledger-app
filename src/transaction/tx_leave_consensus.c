#include "deserialize.h"
#include <string.h> 
#include "types.h"
#include "../common/buffer.h"
//#include "../globals.h"

enum { 
	L_LC_JT = 0
};

parser_status_e transaction_leave_consensus_deserialize(buffer_t *buf, transaction_tx_t *tx) {
	parser_status_e ret;


	switch (tx->stack_index) {
		case L_LC_JT:	
			PRINTF("*****************start transaction leave consensus deserialize*************\n");
			tx->stack_index = L_LC_JT;
			if((ret = transaction_deserialize_spc_field(buf, "joinTxHash", 32, NULL)) != PARSING_OK) {
				return ret;	
			}
		default:
			PRINTF("*****************end transaction join consensus deserialize*************\n");
			return PARSING_OK;

		}		
}

