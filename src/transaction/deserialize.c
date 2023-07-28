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

#include "deserialize.h"
#include <string.h> 
#include "types.h"
#include "../common/buffer.h"
//#include "../globals.h"





void __print(int x, unsigned char *ptr) {
(void)x;
(void)ptr;

	PRINTF("%.*H", x, ptr);
}


parser_status_e transaction_deserialize_var_buffer(buffer_t *buf, char *field, buffer_t *dst) {
	uint64_t value;
	(void)field;

	if (!buffer_read_varint(buf, &value)) {
		PRINTF("deserialize buffer %s len error\n", field);
		return PARSING_LENGTH_ERROR;
	}

		
	dst->ptr = (uint8_t *) (buf->ptr + buf->offset);
	dst->size = value;
	dst->offset = 0;

	
	if (!buffer_seek_cur(buf, value)) {
		PRINTF("deserialize buffer %s erro\nr", field);
		return PARSING_LENGTH_ERROR;
	}


	return PARSING_OK;
}

parser_status_e transaction_deserialize_spc_field(buffer_t *buf, char *field,  uint64_t len,  uint8_t *ptr) {
	parser_status_e ret = PARSING_OK;
	uint8_t *vptr;
	(void)field;


	vptr = (uint8_t *)(buf->ptr  +  buf->offset);

	if(ptr != NULL) {
		if (!buffer_spc_move(buf, ptr, len)) {
			PRINTF("deserialize %s error", field);
		
			len = buf->size - buf->offset;
		
			ret =  PARSING_LENGTH_ERROR;
		}
	} else {
		if (!buffer_seek_cur(buf, len)) {
			PRINTF("deserialize %s error", field);
		
			len = buf->size - buf->offset;
		
			ret =  PARSING_LENGTH_ERROR;
		}
	}


	if(len > 0) {
		uint8_t xlen = len;
		
		PRINTF("=> transaction.%s:%s=%d, CData:",field, (ret != PARSING_OK)?"partLc":"Lc", xlen);
		__print(xlen, vptr);
		PRINTF("\n");


	} else {
		PRINTF("=> transaction.%s:Lc=0, CData:\n",field);


	}

	

	return ret;
}


parser_status_e transaction_deserialize_var_field(buffer_t *buf, char *field, uint64_t *value,  uint8_t *ptr) {
	parser_status_e ret = PARSING_OK;
	uint8_t *vptr;(void)field;

	if (!buffer_read_varint(buf, value)) {
		PRINTF("deserialize %s len error", field);
		return PARSING_LENGTH_ERROR;
	}

 	vptr = (uint8_t *) (buf->ptr + buf->offset);

		
	if(ptr != NULL) {
		if(!buffer_can_read(buf, *value)) {

			buf->lack = *value - (buf->size - buf->offset);
				
			*value = buf->size - buf->offset;

			ret =  PARSING_VALUE_ERROR;

		} 
		
		buffer_spc_move(buf, ptr, *value);


			
	} else {
		if (!buffer_seek_cur(buf, *value)) {
			buf->lack = *value - (buf->size - buf->offset);

			*value = buf->size - buf->offset;

			buf->offset = buf->size;

			
			PRINTF("deserialize %s error\n", field);
			ret =  PARSING_VALUE_ERROR;

		}
	}


	if(*value > 0) {
		uint8_t xlen = *value;

		PRINTF("=> transaction.%s:%s=%d, CData:",field,(ret != PARSING_OK)?"partLc":"Lc",xlen);
		__print(xlen, vptr);
		PRINTF("\n");

	} else {
		PRINTF("=> transaction.%s:Lc=0, CData:\n",field);
	}

	

	return ret;
}




enum { 
	L_C_FROM_ADDRESS = 0,
	L_C_FROM_ASSCID,
	L_C_FROM_ASSID,
	L_C_FROM_AMOUNT,
	L_C_FROM_NONCE,
	L_C_FROM_LOCKED
};


parser_status_e transaction_deserialize_coin_from(buffer_t *buf, transaction_coin_from_t *tx, uint256_t *amount) {
	parser_status_e ret = PARSING_LENGTH_ERROR;
	uint64_t var_64_len;
	//uint16_t var_16_len;
	uint8_t  str_buff[32]; 


	
	switch(tx->stack_index) {
		case L_C_FROM_ADDRESS:
			PRINTF("*****************start transaction.coinFrom  deserialize*************\n");
			tx->stack_index = L_C_FROM_ADDRESS;
			if((ret = transaction_deserialize_var_field(buf, "address", &var_64_len, NULL)) != PARSING_OK) {
				return ret;	
			}

		case L_C_FROM_ASSCID:
			tx->stack_index = L_C_FROM_ASSCID;
			if (!buffer_read_u16(buf, &tx->assets_chain_id, LE)) {
				PRINTF("deserialize assetsChainId error\n");
		        return PARSING_LENGTH_ERROR;
		    }
			PRINTF("=> transaction.coin.from.assetsChainId =%d\n", tx->assets_chain_id);
		

		case L_C_FROM_ASSID:
			tx->stack_index = L_C_FROM_ASSID;
			if (!buffer_read_u16(buf, &tx->assets_id, LE)) {
				PRINTF("deserialize assetsId error\n");
		        return PARSING_LENGTH_ERROR;
		    }
			PRINTF("=> transaction.coin.from.assetsId =%d\n", tx->assets_id);

		case L_C_FROM_AMOUNT:
			tx->stack_index = L_C_FROM_AMOUNT;
		    if ((ret = transaction_deserialize_spc_field(buf, "amount", 32, str_buff)) != PARSING_OK) {
		        return ret;
		    }

			if(tx->assets_chain_id == 1 && tx->assets_id == 1) {
				uint256_t temp1,temp2; 
				
				readu256LE(str_buff, &temp1);
					
				add256(&temp1, amount, &temp2);

				memmove((char *)amount, (char *)&temp2, sizeof(uint256_t));

			}
	
		case L_C_FROM_NONCE:
			tx->stack_index = L_C_FROM_NONCE;
			if((ret = transaction_deserialize_var_field(buf, "nonce", &var_64_len, NULL)) != PARSING_OK) {
				return ret;	
			}
		case L_C_FROM_LOCKED:
			tx->stack_index = L_C_FROM_LOCKED;
			if ((ret = transaction_deserialize_spc_field(buf, "locked", 1, str_buff)) != PARSING_OK) {
		        return ret;
		    }

		default:
			tx->stack_index = L_C_FROM_ADDRESS;
			PRINTF("*****************end transaction.coinFrom  deserialize*************\n");
			return PARSING_OK;
	}


}


enum { 
	L_C_TO_ADDRESS = 0,
	L_C_TO_ASSCID,
	L_C_TO_ASSID,
	L_C_TO_AMOUNT,
	L_C_TOLOCKTIME
};

parser_status_e transaction_deserialize_coin_to(buffer_t *buf, transaction_coin_to_t *tx, uint256_t *amount) {
	parser_status_e ret = PARSING_LENGTH_ERROR;
	uint8_t  str_buff[32];
	

	switch(tx->stack_index) {
		case L_C_TO_ADDRESS:
			PRINTF("*****************start transaction.coinTo deserialize*************\n");
			tx->stack_index = L_C_TO_ADDRESS;
			if(tx->address == NULL && (tx->address = buffer_malloc(40)) == NULL) {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
			
			if((ret = transaction_deserialize_var_field(buf, "address", &tx->address_len, tx->address + tx->address_len)) != PARSING_OK) {
				return ret;	
			}
	
		case L_C_TO_ASSCID:
			tx->stack_index = L_C_TO_ASSCID;
			if (!buffer_read_u16(buf, &tx->assets_chain_id, LE)) {
				PRINTF("deserialize assetsChainId error");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.coin.to.assetsChainId =%d\n", tx->assets_chain_id);

		case L_C_TO_ASSID:
			tx->stack_index = L_C_TO_ASSID;
			if (!buffer_read_u16(buf,  &tx->assets_id, LE)) {
				PRINTF("deserialize assetsId error");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.coin.to.assetsId =%d\n", tx->assets_id);

		case L_C_TO_AMOUNT:
			tx->stack_index = L_C_TO_AMOUNT;
			if(tx->amount == NULL &&  (tx->amount = buffer_malloc(32)) == NULL) {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
		    if ((ret = transaction_deserialize_spc_field(buf, "amount", 32, tx->amount)) != PARSING_OK) {
		        return ret;
		    }

			if(tx->assets_chain_id == 1 && tx->assets_id == 1) {
				uint256_t temp1,temp2; 
				
				readu256LE(tx->amount, &temp1);
					
				add256(&temp1, amount, &temp2);

				memmove((char *)amount,(char *) &temp2, sizeof(uint256_t));

			}
	
		case L_C_TOLOCKTIME:
			tx->stack_index = L_C_TOLOCKTIME;
		    if ((ret = transaction_deserialize_spc_field(buf, "lockTime", 8, str_buff)) != PARSING_OK) {
		        return ret;
		    }
			
		default:
			tx->stack_index=L_C_TO_ADDRESS;
			tx->address_len=0;
			PRINTF("*****************end transaction.coinTo deserialize*************\n");
			return PARSING_OK;
	}
}



enum { 
	L_C_FROM = 0,
	L_C_FROM_I,	
	L_C_TO,
	L_C_TO_I
};

parser_status_e transaction_deserialize_coin(buffer_t *buf, transaction_coin_t *tx) {
	parser_status_e ret = PARSING_LENGTH_ERROR;
	transaction_coin_to_t *coin_to_t;
	transaction_coin_from_t *coin_from_t;


	switch(tx->stack_index) {
		case L_C_FROM:
			PRINTF("*****************start transaction.coin  deserialize*************\n");
			tx->stack_index = L_C_FROM;
			if(!buffer_read_u8(buf, &tx->from_len)) {
				PRINTF("deserialize coin.from.items error\n");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.coin.from.items =%d\n", tx->from_len);
			tx->array_index  = 0;
			
		case L_C_FROM_I:
			tx->stack_index = L_C_FROM_I;	
			while(tx->array_index < tx->from_len) {
				if(tx->array_index >= MAX_COIN_LEN) {
					coin_from_t =&tx->from[MAX_COIN_LEN];
				} else {
					coin_from_t =&tx->from[tx->array_index];
				}
				
				if((ret = transaction_deserialize_coin_from(buf, coin_from_t, &tx->from_amount)) != PARSING_OK) {
					return ret; 
				}
				tx->array_index = tx->array_index  + 1;
			}
			if(tx->from_len > MAX_COIN_LEN) {
				tx->from_len = MAX_COIN_LEN;
			}
				
		case L_C_TO:
			tx->stack_index = L_C_TO;
			if(!buffer_read_u8(buf, &tx->to_len)) {
				PRINTF("deserialize coin.toitems error\n");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.coin.toitems =%d\n", tx->to_len);
			tx->array_index = 0;

		case L_C_TO_I:
			tx->stack_index = L_C_TO_I;
			while(tx->array_index < tx->to_len) {
		
				if(tx->array_index >= MAX_COIN_LEN) {
					coin_to_t =&tx->to[MAX_COIN_LEN];
				} else {
					coin_to_t =&tx->to[tx->array_index];
				}
		
				if((ret = transaction_deserialize_coin_to(buf, coin_to_t, &tx->to_amount)) != PARSING_OK) {
					return ret; 
				}
				tx->array_index = tx->array_index  + 1;
			}

			if(tx->to_len > MAX_COIN_LEN) {
				tx->to_len = MAX_COIN_LEN;

			}
		default:
			
			PRINTF("*****************end transaction.coin  deserialize*************\n");
			return PARSING_OK;
	}


}


enum { 
	L_TYP = 0,
	L_TSP,
	L_RMK,
	L_TXD_L,
	L_TXD_V,
	L_COD_L,
	L_COD_V,
};




bool init_transaction_type( transaction_t *tx, uint16_t type) {
	tx->transaction_type.transaction_type = type;
				
	switch(tx->transaction_type.transaction_type) {
		case 2:
			tx->transaction_type.func= transaction_transfer_deserialize;
			return true;
		//case 5:
		//	tx->transaction_type.func= transaction_join_consensus_deserialize;
		//	return true;
		//case 6:
		//	tx->transaction_type.func= transaction_leave_consensus_deserialize;
		//	return true;	
		case 10:
			tx->transaction_type.func= transaction_cross_chain_transfer_deserialize;
			return true;
		case 16:
			tx->transaction_type.func= transaction_call_contract_deserialize;
			return true;	
		default:
			tx->transaction_type.func= NULL;
			return true;
			//PRINTF("=> not support transaction.type....\n");
			//return false;
	}

}


parser_status_e transaction_deserialize(buffer_t *buf, transaction_t *tx) {
	parser_status_e ret;
	uint64_t var_64_len;
	uint32_t var_32_len;
	uint16_t var_16_len;


	switch (tx->stack_index) {
		case L_TYP:	
			PRINTF("*****************start transaction deserialize*************\n");
			PRINTF("Transaction start message: %.*H\n", 6, buf->ptr + buf->offset);
			tx->stack_index = L_TYP;
			PRINTF("=> parse transaction.type....\n");
			if (!buffer_read_u16(buf, &var_16_len, LE)) {
		        return PARSING_LENGTH_ERROR;
		    }
			PRINTF("=> transaction.type=%d\n", var_16_len);
			if(!init_transaction_type(tx, var_16_len)) {
		        return PARSING_ERROR;
			}
			buffer_init();
			
		case L_TSP: 
			tx->stack_index = L_TSP;
			PRINTF("=> parse transaction.timestamp....\n");
		    if (!buffer_read_u32(buf, &var_32_len, LE)) {
		        return PARSING_LENGTH_ERROR;
		    }
			PRINTF("=> transaction.timestamp=%d\n", var_32_len);


		case L_RMK: 
			tx->stack_index = L_RMK;
			PRINTF("=> parse transaction.remark....\n");
			if((ret = transaction_deserialize_var_field(buf, "remark", &var_64_len, NULL)) != PARSING_OK) {
				return ret;	
			}


		case L_TXD_L:
			tx->stack_index = L_TXD_L;
			if(tx->transaction_type.func == NULL) {
				if((ret = transaction_deserialize_var_field(buf, "txData", &var_64_len, NULL)) != PARSING_OK) {
							return ret; 
				}

			} else {

				PRINTF("=> parse transaction.txData.len....\n");
				if (!buffer_read_varint(buf, &var_64_len)) {
					PRINTF("deserialize txData len error\n");
					return PARSING_LENGTH_ERROR;
				}
				tx->tx_info.stack_index = 0;
			}

		case L_TXD_V:
			tx->stack_index = L_TXD_V;
			if(tx->transaction_type.func != NULL) {
				PRINTF("=> parse transaction.txData.value....\n");
				if((ret = tx->transaction_type.func(buf,  &tx->tx_info)) != PARSING_OK) {
					return ret; 
				}
			}
		case L_COD_L: 
			tx->stack_index = L_COD_L;
			PRINTF("=> parse transaction.coinData.len....\n");
			if (!buffer_read_varint(buf, &var_64_len)) {
				PRINTF("deserialize coinData len error\n");
				return PARSING_LENGTH_ERROR;
			}
			tx->coin_info.stack_index=0;
		case L_COD_V: 
			tx->stack_index = L_COD_V;
				PRINTF("=> parse transaction.coinData.value....\n");
			if((ret = transaction_deserialize_coin(buf, &tx->coin_info)) != PARSING_OK) {
				return ret; 
			}
		default:
			PRINTF("*****************end transaction deserialize*************\n");
			return PARSING_OK;

	}
	
}

