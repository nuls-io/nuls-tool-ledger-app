

#include "deserialize.h"
#include <string.h> 
#include "types.h"
#include "../common/buffer.h"
//#include "../globals.h"


parser_status_e transaction_cross_chain_transfer_deserialize(buffer_t *buf, transaction_tx_t *tx) {
	(void)buf, (void)tx;

	return PARSING_OK;
}



