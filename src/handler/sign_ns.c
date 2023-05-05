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

#include "sign_ns.h"
#include "../sw.h"
#include "../globals.h"
#include "../crypto.h"
#include "../ui/display.h"
#include "../common/buffer.h"
#include "../transaction/types.h"
#include "../transaction/deserialize.h"


extern bool init_public_key(buffer_t *cdata, bool is_gen_address);

extern int varint_write(uint8_t *out, size_t offset, uint64_t value);

extern void validate_transaction_msg();
extern bool format_size64(char *out, size_t outLen, size_t in);


int handler_sign_ns(buffer_t *cdata, uint8_t chunk, bool more) {
	uint32_t len;
	unsigned char temp[10+1];
	size_t balance	= 0;
	char SIGN_MAGIC[] = "\x19NULS Signed Message:\n";

	if (chunk == 0) { 
        explicit_bzero(&G_context, sizeof(G_context));
        G_context.req_type = CONFIRM_TRANSACTION;
        G_context.state = STATE_NONE;


		if(!init_public_key(cdata, true)) {
			return io_send_sw(SW_TX_PARSING_FAIL);
		}

		cx_keccak_init(&G_context.cx_sha.cx_sha3, 256);
		cx_hash((cx_hash_t *) &G_context.cx_sha.cx_sha3, 0, (uint8_t *) SIGN_MAGIC, sizeof(SIGN_MAGIC) - 1, NULL, 0);

		PRINTF("Hash-SOURCE-DATA: %.*H\n", sizeof(SIGN_MAGIC) - 1, SIGN_MAGIC);


		buffer_read_u32(cdata, &len, BE);
		
		G_context.tx_info.message_len=len;

		format_size64((char *)temp, 10, G_context.tx_info.message_len);
		cx_hash((cx_hash_t *) &G_context.cx_sha.cx_sha3, 0, temp, strlen((char *)temp), NULL, 0);
		PRINTF("Hash-SOURCE-DATA: %.*H\n", strlen(temp), temp);
		

		balance  = cdata->size - cdata->offset;

    }  else {

	    if (G_context.req_type != CONFIRM_TRANSACTION) {
	        return io_send_sw(SW_BAD_STATE);
	    }
		
		balance =  cdata->size;

	}

	if(balance > 0) {
		cx_hash((cx_hash_t *) &G_context.cx_sha.cx_sha3, 0, (uint8_t *)(cdata->ptr + cdata->offset), balance, NULL, 0);
		PRINTF("Hash-SOURCE-DATA: %.*H\n", balance, (cdata->ptr + cdata->offset));

		
		G_context.tx_info.raw_tx_len += balance;

	}

	if(G_context.tx_info.message_len < G_context.tx_info.raw_tx_len) {
		PRINTF("read redundant message\n");
		return io_send_sw(SW_WRONG_TX_LENGTH);
	}

	if(more) {
		return io_send_sw(SW_OK);
	} else {
		if(G_context.tx_info.message_len  != G_context.tx_info.raw_tx_len) {
			PRINTF("Parsing fail:%d:%d\n", G_context.tx_info.message_len, G_context.tx_info.raw_tx_len);
			return io_send_sw(SW_TX_PARSING_FAIL);
		
		}
	}
	


    G_context.state = STATE_PARSED;

	 cx_hash((cx_hash_t *) &G_context.cx_sha.cx_sha3,
                CX_LAST,
                NULL,
                0,
                G_context.tx_info.m_hash,
                32);

    //cx_hash_final((cx_hash_t *)&G_context.cx_sha256, G_context.tx_info.m_hash);
    PRINTF("1-Hash: %.*H\n", sizeof(G_context.tx_info.m_hash), G_context.tx_info.m_hash);

    validate_transaction_msg();

    return 0;
}

