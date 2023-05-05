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
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <string.h>   // memset, explicit_bzero

#include "os.h"
#include "cx.h"

#include "sign_tx.h"
#include "../sw.h"
#include "../globals.h"
#include "../crypto.h"
#include "../ui/display.h"
#include "../common/buffer.h"
#include "../transaction/types.h"
#include "../transaction/deserialize.h"


bool init_public_key(buffer_t *cdata, bool is_gen_address);
int varint_write(uint8_t *out, size_t offset, uint64_t value);
cx_err_t cx_sha256_update(cx_sha256_t *ctx, const uint8_t *data, size_t len);



int handler_sign_tx(buffer_t *cdata, uint8_t chunk, bool more) {
	size_t balance	= 0;
	uint8_t m_hash[32];  
	
	if (chunk == 0) { 
        explicit_bzero(&G_context, sizeof(G_context));
        G_context.req_type = CONFIRM_TRANSACTION;
        G_context.state = STATE_NONE;


		if(!init_public_key(cdata, true)) {
			return io_send_sw(SW_TX_PARSING_FAIL);
		}

		cx_sha256_init(&G_context.cx_sha.cx_sha256);

		balance  = cdata->size - cdata->offset;
		
    }  else {

	    if (G_context.req_type != CONFIRM_TRANSACTION) {
	        return io_send_sw(SW_BAD_STATE);
	    }
		
		balance =  cdata->size;

	}

	if(balance > 0) {
		if (G_context.tx_info.raw_tx_len + balance > sizeof(G_context.tx_info.raw_tx)) {
			return io_send_sw(SW_WRONG_TX_LENGTH);
		}


		G_context.tx_info.message_len+= balance;
		cx_sha256_update((cx_sha256_t *)&G_context.cx_sha.cx_sha256,  (uint8_t *)(cdata->ptr + cdata->offset), balance);
		   PRINTF("Hash-SOURCE-DATA: %.*H\n", balance, (cdata->ptr + cdata->offset));
		if (!buffer_spc_move(cdata,
						 G_context.tx_info.raw_tx + G_context.tx_info.raw_tx_len,
						 balance)) {
			return io_send_sw(SW_TX_PARSING_FAIL);
		}
		G_context.tx_info.raw_tx_len += balance;

	}
	


    buffer_t buf = {.ptr = G_context.tx_info.raw_tx,
                    .size = G_context.tx_info.raw_tx_len,
                    .offset = 0};

	

    parser_status_e status = transaction_deserialize(&buf, &G_context.tx_info.transaction);
    PRINTF("Parsing status: %d.\n", status);

	if(!more && status != PARSING_OK) {
		PRINTF("Parsing fail\n");
		return io_send_sw(SW_TX_PARSING_FAIL);

	}

	switch(status) {
		case PARSING_LENGTH_ERROR:
				memmove(G_context.tx_info.raw_tx, buf.ptr + buf.offset, buf.size - buf.offset);
				G_context.tx_info.raw_tx_len = buf.size - buf.offset;
				
				return io_send_sw(SW_OK);	
		case PARSING_VALUE_ERROR:
				G_context.tx_info.raw_tx_len = varint_write(G_context.tx_info.raw_tx,  0, buf.lack);
				return io_send_sw(SW_OK);
		case PARSING_OK:
			if(buf.size > buf.offset) {
				PRINTF("Parsing fail: Contains unanalyzed data\n");
				return io_send_sw(SW_TX_PARSING_FAIL);
			}
			break;
		default:
			return io_send_sw(SW_TX_PARSING_FAIL);
			
	}


    G_context.state = STATE_PARSED;


   	cx_hash_final((cx_hash_t *)&G_context.cx_sha.cx_sha256, m_hash);
    PRINTF("1-Hash: %.*H\n", sizeof(m_hash), m_hash);

	cx_hash_sha256(m_hash, sizeof(m_hash), G_context.tx_info.m_hash, sizeof(G_context.tx_info.m_hash));
    PRINTF("2-Hash: %.*H\n", sizeof(G_context.tx_info.m_hash), G_context.tx_info.m_hash);


   	ui_display_sign_tx();

	return 0;
}

