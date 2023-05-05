

#include "deserialize.h"
#include <string.h> 
#include "types.h"
#include "../common/buffer.h"
//#include "../globals.h"


enum { 
	L_D_LEN = 0,
	L_D_VALUE_L,	
	L_D_VALUE_V,
};

parser_status_e dimensional_transfer_deserialize(buffer_t *buf, uint8_t index, transaction_tx_call_contract_t *call_contract ) {
	parser_status_e ret;
	//uint64_t val_64_len;

	uint8_t  **ptr ;

	switch(index) {
		case 0:
			ptr = &call_contract->element1;
			break;
		case 1:
			ptr = &call_contract->element2;
			break;	
		default:
			ptr = &call_contract->element3;
			break;		
	}

	switch(call_contract->stack_index) {
		case L_D_LEN: 
			call_contract->stack_index = L_D_LEN;
			if(!buffer_read_u8(buf, &call_contract->one_args_len)) {
				PRINTF("deserialize one_dimensional_args_len  error\n");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.one_args_len  =%d\n", call_contract->one_args_len);
			call_contract->one_array_index  = 0; 
			if(call_contract->one_args_len != 1) {
				PRINTF("deserialize one_dimensional_args_len not equire 1\n");
				return PARSING_ERROR;
			}
		case L_D_VALUE_L: 
			call_contract->stack_index = L_D_VALUE_L;
			if (!buffer_read_varint(buf, &call_contract->element_len)) {
				PRINTF("deserialize element value len error\n");
				return PARSING_LENGTH_ERROR;
			}
			if((*ptr = buffer_malloc(call_contract->element_len + 1)) == NULL) {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
		case L_D_VALUE_V: 
			call_contract->stack_index = L_D_VALUE_V;
			if((ret = transaction_deserialize_spc_field(buf, "element.value", call_contract->element_len, *ptr)) != PARSING_OK) {
				return ret;
			}
			(*ptr)[call_contract->element_len]='\0';

				
		default:
			return PARSING_OK;
				
	}

}


parser_status_e dimensional_transferFrom_deserialize(buffer_t *buf, uint8_t index, transaction_tx_call_contract_t *call_contract ) {
	parser_status_e ret;
	//uint64_t val_64_len;

	uint8_t  **ptr ;

	switch(index) {
		case 1:
			ptr = &call_contract->element1;
			break;
		case 2:
			ptr = &call_contract->element2;
			break;	
		default:
			ptr = &call_contract->element3;
			break;		
	}

	switch(call_contract->stack_index) {
		case L_D_LEN: 
			call_contract->stack_index = L_D_LEN;
			if(!buffer_read_u8(buf, &call_contract->one_args_len)) {
				PRINTF("deserialize one_dimensional_args_len  error\n");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.one_args_len  =%d\n", call_contract->one_args_len);
			call_contract->one_array_index  = 0; 
			if(call_contract->one_args_len != 1) {
				PRINTF("deserialize one_dimensional_args_len not equire 1\n");
				return PARSING_ERROR;
			}
		case L_D_VALUE_L: 
			call_contract->stack_index = L_D_VALUE_L;
			if (!buffer_read_varint(buf, &call_contract->element_len)) {
				PRINTF("deserialize element value len error\n");
				return PARSING_LENGTH_ERROR;
			}
			if((*ptr = buffer_malloc(call_contract->element_len + 1)) == NULL) {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
		case L_D_VALUE_V: 
			call_contract->stack_index = L_D_VALUE_V;
			if((ret = transaction_deserialize_spc_field(buf, "element.value", call_contract->element_len, *ptr)) != PARSING_OK) {
				return ret;
			}
			(*ptr)[call_contract->element_len]='\0';

				
		default:
			return PARSING_OK;
				
	}

}


parser_status_e dimensional_deserialize(buffer_t *buf, uint8_t index, transaction_tx_call_contract_t *call_contract ) {
	parser_status_e ret;
	uint64_t val_64_len;(void)index;

	switch(call_contract->stack_index) {
		case L_D_LEN: 
			call_contract->stack_index = L_D_LEN;
			if(!buffer_read_u8(buf, &call_contract->one_args_len)) {
				PRINTF("deserialize one_dimensional_args_len  error\n");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.one_args_len  =%d\n", call_contract->one_args_len);
			call_contract->one_array_index  = 0; 
		case L_D_VALUE_L: 
			call_contract->stack_index = L_D_VALUE_L;

			while(call_contract->one_array_index <  call_contract->one_args_len) {
				
				if((ret = transaction_deserialize_var_field(buf, "element", &val_64_len, NULL)) != PARSING_OK) {
					return ret;	
				}
				call_contract->one_array_index = call_contract->one_array_index  + 1;
			}
		default:
			return PARSING_OK;
				
	}

}


enum { 
	L_CC_SENDER = 0,
	L_CC_CADDR,
	L_CC_VALUE,
	L_CC_GMSL,
	L_CC_GMSP,
	L_CC_MNAME,
	L_CC_MDES,
	L_CC_TDAL,
	L_CC_TDAV,
	L_CC_MEM
};

parser_status_e transaction_call_contract_deserialize(buffer_t *buf, transaction_tx_t *tx) {
	parser_status_e ret;
	uint64_t val_64_len;
	transaction_tx_call_contract_t * call_contract =  &tx->call_contract;


	switch (tx->stack_index) {
		case L_CC_SENDER:	
			PRINTF("*****************start transaction call contract deserialize*************\n");
			tx->stack_index = L_CC_SENDER;
			if((ret = transaction_deserialize_spc_field(buf, "sender", 23, NULL)) != PARSING_OK) {
				return ret;	
			}
		case L_CC_CADDR:
			tx->stack_index = L_CC_CADDR;
			if(call_contract->contract_address == NULL && (call_contract->contract_address = buffer_malloc(23)) == NULL)  {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
			if((ret = transaction_deserialize_spc_field(buf, "contractAddress", 23, call_contract->contract_address)) != PARSING_OK) {
				return ret;	
			}
		case L_CC_VALUE:
			tx->stack_index = L_CC_VALUE;
			if(call_contract->value == NULL && (call_contract->value = buffer_malloc(32)) == NULL)  {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
			if((ret = transaction_deserialize_spc_field(buf, "value", 32, call_contract->value)) != PARSING_OK) {
				return ret;	
			}
		case L_CC_GMSL:	
			tx->stack_index = L_CC_GMSL;
			if((ret = transaction_deserialize_spc_field(buf, "gasLimit", 8, NULL)) != PARSING_OK) {
				return ret;	
			}	
		case L_CC_GMSP:	
			tx->stack_index = L_CC_GMSP;
			if((ret = transaction_deserialize_spc_field(buf, "gasPrice", 8, NULL)) != PARSING_OK) {
				return ret;	
			}	
		case L_CC_MNAME:	
			tx->stack_index = L_CC_MNAME;
			if(call_contract->method_name == NULL && (call_contract->method_name = buffer_malloc(40)) == NULL) {
				PRINTF("buffer_malloc not enough space\n");
				return PARSING_ERROR;
			}
			
			if((ret = transaction_deserialize_var_field(buf, "methodName", &call_contract->method_len, call_contract->method_name + call_contract->method_len)) != PARSING_OK) {
				return ret;	
			}
			
		case L_CC_MDES:	
			tx->stack_index = L_CC_MDES;
			if((ret = transaction_deserialize_var_field(buf, "methodDesc", &val_64_len, NULL)) != PARSING_OK) {
				return ret;	
			}	
		case L_CC_TDAL:	
			tx->stack_index = L_CC_TDAL;
			if(!buffer_read_u8(buf, &call_contract->two_args_len)) {
				PRINTF("deserialize two_args_len error\n");
				return PARSING_LENGTH_ERROR;
			}
			PRINTF("=> transaction.two_args_len =%d\n", call_contract->two_args_len);
			call_contract->two_array_index  = 0;	
		case L_CC_TDAV:	
			tx->stack_index = L_CC_TDAV;	

				while(call_contract->two_array_index <  call_contract->two_args_len) {
					
					if(strcmp((char *)call_contract->method_name, "transfer") == 0) {
						if((ret = dimensional_transfer_deserialize(buf, call_contract->two_array_index, call_contract)) != PARSING_OK) {
							return ret;
						}
					} else if(strcmp((char *)call_contract->method_name, "transferFrom") == 0) {
						if((ret = dimensional_transferFrom_deserialize(buf, call_contract->two_array_index, call_contract)) != PARSING_OK) {
							return ret;
						}

					} else {
						if((ret = dimensional_deserialize(buf, call_contract->two_array_index, call_contract)) != PARSING_OK) {
							return ret; 
						}
					}
					
					call_contract->two_array_index = call_contract->two_array_index  + 1;
					call_contract->stack_index = 0;
				}
		
		default:
			PRINTF("*****************end transaction call contract deserialize*************\n");
			return PARSING_OK;

		}		

}




