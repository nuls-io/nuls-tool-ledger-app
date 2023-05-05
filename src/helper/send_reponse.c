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

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <string.h>  // memmove

#include "send_response.h"
#include "../constants.h"
#include "../globals.h"
#include "../sw.h"
#include "common/buffer.h"

int helper_send_response_pubkey() {
    uint8_t resp[1 + 33 + 1 + 37] = {0};
    size_t offset = 0;
	

    resp[offset++] = 33;
    memmove(resp + offset, G_context.pk_info.compressed_public_key, 33);
    offset += 33;
	

	resp[offset++] = 37;
	memmove(resp + offset, "NULSd", 5);
	offset += 5;
	memmove(resp + offset, G_context.pk_info.address_base_58, 32);
	offset += 32;
	

    return io_send_response(&(const buffer_t){.ptr = resp, .size = offset, .offset = 0}, SW_OK);
}

int helper_send_response_sig() {
    uint8_t resp[1 + 33 + 1 + MAX_DER_SIG_LEN] = {0};
    size_t offset = 0;

	resp[offset++] = 33;
    memmove(resp + offset, G_context.pk_info.compressed_public_key, 33);
    offset += 33;

    resp[offset++] = G_context.tx_info.signature_len;
    memmove(resp + offset, G_context.tx_info.signature, G_context.tx_info.signature_len);
    offset += G_context.tx_info.signature_len;
	
   // resp[offset++] = (uint8_t) G_context.tx_info.v;

    return io_send_response(&(const buffer_t){.ptr = resp, .size = offset, .offset = 0}, SW_OK);
}

int helper_send_response_sig_msg() {
    uint8_t resp[ MAX_DER_SIG_LEN] = {0};
    size_t offset = 0;

    memmove(resp + offset, G_context.tx_info.signature, G_context.tx_info.signature_len);
    offset += G_context.tx_info.signature_len;

	PRINTF("helper_send_response_sig_msg:%d,%d\n", sizeof(resp), G_context.tx_info.signature_len);

	
   // resp[offset++] = (uint8_t) G_context.tx_info.v;

    return io_send_response(&(const buffer_t){.ptr = resp, .size = offset, .offset = 0}, SW_OK);
}


