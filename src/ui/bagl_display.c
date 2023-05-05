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

#ifdef HAVE_BAGL

#pragma GCC diagnostic ignored "-Wformat-invalid-specifier"  // snprintf
#pragma GCC diagnostic ignored "-Wformat-extra-args"         // snprintf

#include <stdbool.h>  // bool
#include <string.h>   // memset

#include "os.h"
#include "ux.h"
#include "glyphs.h"

#include "display.h"
#include "constants.h"
#include "../globals.h"
#include "../io.h"
#include "../sw.h"
#include "../address.h"
#include "action/validate.h"
#include "../transaction/types.h"
#include "../common/bip32.h"
#include "../common/format.h"
#include "../menu.h"
#include "../common/uint256.h"
#include "../common/uint_common.h"


#define MAX_NUM_STEPS 10
#define __UX_STEP_NOCB(ui_var_name)  ux_flow_step_t ui_var_name = { NULL, &ux_display_params, NULL, NULL}
#define get_context_tx_info()  G_context.tx_info.transaction.tx_info
#define get_context_tx_type()  G_context.tx_info.transaction.transaction_type.transaction_type
#define get_context_coin_info() G_context.tx_info.transaction.coin_info


extern const internalStorage_t N_storage_real;

void ui_action_validate_not_blind_sign();

void display_next_state(bool is_upper_delimiter);
bool format_decimals(const char *src, size_t srcLength, char *target, size_t targetLength, uint8_t decimals);


enum e_state {
   STATIC_SCREEN,
   DYNAMIC_SCREEN,
};


//-------------------------------global var----------------------
action_validate_cb g_validate_callback;
step_flow_cb g_step_flow_callback;


enum e_state current_state;
int8_t step_flow;
char title[20];
char text[80];


//------------------------------UI STEP----------------------

ux_layout_paging_params_t ux_display_params = {.title = title,.text = text};
	
ux_flow_step_t* ux_display_transaction_flow[MAX_NUM_STEPS];




UX_STEP_NOCB(ux_display_review_transaction_step,
			 pnn,
			 {
			     &C_icon_eye,
			     "Review",
			     "Transaction",
			 });

UX_STEP_NOCB(ux_display_review_address_step,
			 pnn,
			 {
			     &C_icon_eye,
			     "Verify",
			     "address",
			 });



UX_STEP_CB(ux_display_approve_step,
			pb,
			(*g_validate_callback)(true),
			{
			   &C_icon_validate_14,
			   "Approve",
			});

UX_STEP_CB(ux_display_accept_step,
			pb,
			(*g_validate_callback)(true),
			{
			   &C_icon_validate_14,
			   "Accept and send",
			});


UX_STEP_CB(ux_display_reject_step,
			pb,
			(*g_validate_callback)(false),
			{
			   &C_icon_crossmark,
			   "Reject",
			});


UX_STEP_INIT(step_upper_delimiter,
			NULL,
			NULL,
			{
			   display_next_state(true);
			});

UX_STEP_NOCB(step_generic,
			bnnn_paging,
			{
			   .title = title,
			   .text = text,
			});

 
UX_STEP_INIT(
			step_lower_delimiter,
			NULL,
			NULL,
			{
			   display_next_state(false);
			});


//&C_icon_crossmark,
UX_STEP_CB(ux_display_not_blind_sign_step, 
				bnnn_paging, 
				ui_action_validate_not_blind_sign(),  
				{
			   		.title = "Error",
			   		.text = "Blind signing must be enabled in Settings",
				});

UX_FLOW(ux_display_not_blind_sign_flow, 
	&ux_display_not_blind_sign_step,
	FLOW_LOOP);


void ui_action_validate_not_blind_sign() {
    validate_deny();
    ui_menu_main();
}




void ui_action_validate_pubkey(bool choice) {
    validate_pubkey(choice);
    ui_menu_main();
}

void ui_action_validate_transaction(bool choice) {
    validate_transaction(choice);
    ui_menu_main();
}


ux_flow_step_t * __init_ux_flow_step(ux_flow_step_t *flow_step, ux_flow_step_init_t func) {
	
	flow_step->init = func;
	
	return flow_step;
}


 
void __conv_mount_1(uint8_t * amount, char *out, uint8_t decimals) 
{    
	char temp[80];
    uint256_t number;


	readu256LE(amount, &number);

	tostring256(&number, 10,  temp, 80);

	format_decimals(temp, strlen(temp),  out, 80, decimals);

}

void __conv_mount_2(uint256_t * number, char *out, uint8_t decimals) 
{
	char temp[80];

	tostring256(number, 10,  temp,  80);

	format_decimals(temp, strlen(temp), out, 80, decimals);

}


void bnnn_paging_edgecase() {

    G_ux.flow_stack[G_ux.stack_count - 1].prev_index = G_ux.flow_stack[G_ux.stack_count - 1].index - 2;
    G_ux.flow_stack[G_ux.stack_count - 1].index--;
    ux_flow_relayout();
}


// Main function that handles all the business logic for our new display architecture.
void display_next_state(bool is_upper_delimiter) {
	

   if (is_upper_delimiter) { 
      if (current_state == STATIC_SCREEN) {

         bool dynamic_data =  (*g_step_flow_callback)(true);

         if (dynamic_data) {
             current_state = DYNAMIC_SCREEN;
         }
         ux_flow_next();
      } else {
      
         bool dynamic_data =  (*g_step_flow_callback)(false);
		 
         if (dynamic_data) {
             ux_flow_next();
         } else {
             current_state = STATIC_SCREEN;
             ux_flow_prev();
         }
      }
   } else {
      if (current_state == STATIC_SCREEN) {
	  	
         bool dynamic_data =  (*g_step_flow_callback)( false);

         if (dynamic_data) {
           current_state = DYNAMIC_SCREEN;
         }
		 
         ux_flow_prev();
      } else {

         bool dynamic_data =  (*g_step_flow_callback)(true);
         if (dynamic_data) {
		 	
           bnnn_paging_edgecase();
		   
         } else {
           current_state = STATIC_SCREEN;

           ux_flow_next();
         }
      }
   }
}

bool is_built_in_assets() {
	char temp[40];

	nuls_dispaly_address_to_encoded_base58(get_context_tx_info().call_contract.contract_address, temp);

	PRINTF("contract_address:%s\n", temp);
	
	if(strcmp(temp, "NULSd6Hgv3Y49CHapt5qL4pCiEX8x3ZHrh6ie") == 0) {
		return true;
	} else if(strcmp(temp, "NULSd6HgrzcXdTuGvRF9DmkxoXM8XNmXZs95d") == 0) {
		return true;
	} else if(strcmp(temp, "NULSd6Hgr7kEY9dEJVEpjsWAgmGRqBiMe9vL7") == 0) {
		return true;
	} else if(strcmp(temp, "NULSd6HgvhaiirRcbg3QY7GWSU6RSFGaVovme") == 0) {
		return true;
	}else if(strcmp(temp, "NULSd6Hh1AW8WxfB64RW1EYxP4zXaMxkQi5xd") == 0) {
		return true;
	} else {
		return false;
	}
	

}


static int  __init() {
	switch(get_context_tx_type()) {
			case 2:
			case 5:
			case 6:
			case 10:
			case 16:
				return 0;
		}

		if(N_storage.dataAllowed) {
			return 0;
		}
	  
	  if (G_ux.stack_count == 0) {
		  ux_stack_push();
	  }
	  
	  ux_flow_init(0, ux_display_not_blind_sign_flow, NULL);

	  return -1;

		
}



static void __dsp_st_type_init() {
	strcpy(title, "Type");
	
	switch(get_context_tx_type()) {
			case 2:
				strcpy(text, "Transfer");
				break;
			case 5:
				strcpy(text, "Deposit");
				break;
			case 6:
				strcpy(text, "Withdraw");
				break;
			case 10:
				strcpy(text, "Cross Chain Transfer");
				break;
			case 16:
				if ((strcmp((char *)get_context_tx_info().call_contract.method_name,	"transfer") == 0 || strcmp((char *)get_context_tx_info().call_contract.method_name,  "transferFrom") == 0) && is_built_in_assets()) {
					strcpy(text, "NRC20 Transfer");

				} else {
					strcpy(text,"Call Contract");
				}
			
				break;
			default:
				strcpy(text, "Blind sign");
				break;
		}
}


static void __dsp_st_amount_deposit_init() {
	strcpy(title, "Amount");
	strcpy(text, "NULS "); __conv_mount_1(get_context_tx_info().join_consensus.deposit, text + 5, 8);
}


static void __dsp_st_amount_withdraw_init() {
	strcpy(title, "Amount");
	strcpy(text, "NULS "); __conv_mount_1(get_context_coin_info().to[0].amount, text + 5, 8);
}

static void __dsp_st_address_nrc20_init() {
	strcpy(title, "Address");
	strlcpy(text, (char *)get_context_tx_info().call_contract.element1, 80);	
}

static void __dsp_st_amount_nrc20_init() {
	char temp[40];
	uint8_t decimals;
	
	strcpy(title, "Amount");

	nuls_dispaly_address_to_encoded_base58(get_context_tx_info().call_contract.contract_address, temp);
	if(strcmp(temp, "NULSd6Hgv3Y49CHapt5qL4pCiEX8x3ZHrh6ie") == 0) {
		strcpy(text, "NABOX ");
		decimals = 18;
	
	} else if(strcmp(temp, "NULSd6HgrzcXdTuGvRF9DmkxoXM8XNmXZs95d") == 0) {
		strcpy(text, "NSWAP ");
		decimals = 8;
	
	} else if(strcmp(temp, "NULSd6Hgr7kEY9dEJVEpjsWAgmGRqBiMe9vL7") == 0) {
		strcpy(text, "WTF ");
		decimals = 8;
	
	} else if(strcmp(temp, "NULSd6HgvhaiirRcbg3QY7GWSU6RSFGaVovme") == 0) {
		strcpy(text, "BULS ");
		decimals = 8;
	
	}else if(strcmp(temp, "NULSd6Hh1AW8WxfB64RW1EYxP4zXaMxkQi5xd") == 0) {
		strcpy(text, "NFTC ");
		decimals = 2;
	
	} else {
		strcpy(text, "UKNOW ");
		decimals = 0;
	
	}
	
	format_decimals((char *)get_context_tx_info().call_contract.element2, strlen((char *)get_context_tx_info().call_contract.element2),  text + strlen(text), 80, decimals);
}


static void __dsp_st_address_contract_init() {
	strcpy(title, "Contract Address");

	
	nuls_dispaly_address_to_encoded_base58(get_context_tx_info().call_contract.contract_address, text);
}

static void __dsp_st_amount_contract_init() {
	strcpy(title, "Amount");
	strcpy(text, "NULS "); __conv_mount_1(get_context_tx_info().call_contract.value, text + 5, 8);
}

static void __dsp_st_method_contract_init() {
	strcpy(title, "Contract Method");
	strlcpy(text, (char *)get_context_tx_info().call_contract.method_name, 80);
}

static void __dsp_st_free_init() {
	uint256_t uint256;

	strcpy(title, "Max Fees");

	sub256(&get_context_coin_info().from_amount, &get_context_coin_info().to_amount, &uint256);
		
	strcpy(text, "NULS "); __conv_mount_2(&uint256, text + 5, 8);
}


 bool __ui_display_sign_tx_x(bool is_next)  {
	 uint16_t len = 0;
	 uint8_t decimals;
	 uint8_t sum  =  1 + (get_context_coin_info().to_len * 2) +  1;


	if(is_next) {
		
		step_flow++;

		if(step_flow > sum) {
			goto __F;
		}

	} else {
		step_flow--;

			
		if(step_flow == 0 ) {
			goto __F;

		}

	}

	if(step_flow == 1) {
		__dsp_st_type_init();
		goto __T;

	}
	
	if( step_flow == sum) {
		__dsp_st_free_init();
		goto __T;

	}

	 transaction_t * transaction_info = (transaction_t *)&G_context.tx_info.transaction;

	
	 if(step_flow%2 == 0) {
		  strcpy(title,  "Amount");
		  transaction_coin_to_t*  to = &(transaction_info->coin_info.to[(step_flow - 3)/2]);
		 
		  
		  if(to->assets_chain_id == 1 && to->assets_id == 1) {
			  strcpy(text, "NULS ");
			  decimals = 8;
			  
		  } else if(to->assets_chain_id == 9 && to->assets_id == 1) {
			  strcpy(text, "NVT ");
			  decimals = 8;
		 
		  } else if(to->assets_chain_id == 9 && to->assets_id == 220) {
			  strcpy(text, "USDTN ");
			  decimals = 18;
		 
		  } else if(to->assets_chain_id == 9 && to->assets_id == 388) {
			  strcpy(text, "USDCN ");
			  decimals = 18;
		 
		  } else {
			  len = 0;
			  format_u64(text + len,  100, to->assets_chain_id);
			  len = strlen(text);text[len] = '-';len++;
		 
			  format_u64(text + len,  100, to->assets_id);
			  len = strlen(text);text[len] = ' ';len++;
			  text[len] = '\0';
		 
		 
			  decimals = 0;
		 
		  }
		 
		 __conv_mount_1(to->amount, text + strlen(text), decimals);

	 } else {
	 	strcpy(title, "Address");
 		nuls_dispaly_address_to_encoded_base58(transaction_info->coin_info.to[(step_flow - 2)/2].address,text);
	 }
	
__T:
	return true;	
	
__F:
	PRINTF("Call get_next_data return false\n");
	return false;	

 }
 

 bool  __ui_display_sign_tx_5(bool is_next)  {
 	//TYPE + AMOUT + FEE
	uint8_t sum  =  1 + 1 + 1;
	 
	if(is_next) {
		
		step_flow++;

		if(step_flow > sum) {
			goto __F;
		}

	} else {
		step_flow--;

			
		if(step_flow == 0 ) {
			goto __F;

		}
	}

	
	if(step_flow == 1) {
		__dsp_st_type_init();
		goto __T;

	}

	if( step_flow == 2) {
		__dsp_st_amount_deposit_init();
		goto __T;

	}
	
	if( step_flow == 3) {
		__dsp_st_free_init();
		goto __T;

	}
	
__T:
	 return true;	 
		 
__F:
	PRINTF("Call get_next_data return false\n");
	return false;	 



 }

 bool  __ui_display_sign_tx_6(bool is_next)  {
	 //TYPE + AMOUT + FEE
	 uint8_t sum  =  1 + 1 + 1;
	  
	 if(is_next) {
		 
		 step_flow++;
 
		 if(step_flow > sum) {
			 goto __F;
		 }
 
	 } else {
		 step_flow--;
 
			 
		 if(step_flow == 0 ) {
			 goto __F;
 
		 }
	 }
 
	 
	 if(step_flow == 1) {
		 __dsp_st_type_init();
		 goto __T;
 
	 }
 
	 if( step_flow == 2) {
		 __dsp_st_amount_withdraw_init();
		 goto __T;
 
	 }
	 
	 if( step_flow == 3) {
		 __dsp_st_free_init();
		 goto __T;
 
	 }
	 
 __T:
	  return true;	  
		  
 __F:
	 PRINTF("Call get_next_data return false\n");
	 return false;	  
 
 
  
 }



bool  __ui_display_sign_tx_16_nrc20(bool is_next)  {
    //TYPE + ADDRESS + AMOUNT + FEE
	uint8_t sum  =  4;

	if(is_next) {
		step_flow++;
		
		if(step_flow > sum) {
			goto __F;
		}

	} else {
		step_flow--;
		
		if(step_flow == 0) {
		  goto __F;
		}
	}

	if(step_flow == 1) {
		 __dsp_st_type_init();
		 goto __T;
 
	 }
 
	 if( step_flow == 2) {
	 	__dsp_st_amount_nrc20_init();

		 goto __T;
 
	 }

	 if( step_flow == 3) {
		 	__dsp_st_address_nrc20_init();
		 goto __T;
 
	 }
	 
	 if( step_flow == 4) {
		 __dsp_st_free_init();
		 goto __T;
 
	 }

__T:
	return true;	   

__F:
	PRINTF("Call get_next_data return false\n");
	return false;

}


 bool  __ui_display_sign_tx_16_contract(bool is_next)	{
 	    //TYPE + ADDRESS + AMOUNT +METHOD + FEE
	 uint8_t sum  =  5;
 
	 if(is_next) {
		 step_flow++;
		 
		 if(step_flow > sum) {
			 goto __F;
		 }
 
	 } else {
		 step_flow--;
		 
		 if(step_flow == 0) {
		   goto __F;
		 }
	 }

	 if(step_flow == 1) {
		 __dsp_st_type_init();
		 goto __T;
 
	 }
 
	 if( step_flow == 2) {
	 	 __dsp_st_amount_contract_init();
		 goto __T;
 
	 }

	 if( step_flow == 3) {
		 __dsp_st_address_contract_init();
		 goto __T;
 
	 }

	 if( step_flow == 4) {
		 __dsp_st_method_contract_init();
		 goto __T;
 
	 }
	 
	 if( step_flow == 5) {
		 __dsp_st_free_init();
		 goto __T;
 
	 }
 
 __T:
	 return true;		
 
 __F:
	 PRINTF("Call get_next_data return false\n");
	 return false;
 

}



 int ui_display_sign_tx() {
	 uint8_t offset = 0;

	 
	 if (G_context.req_type != CONFIRM_TRANSACTION || G_context.state != STATE_PARSED) {
		G_context.state = STATE_NONE;		
		return io_send_sw(SW_BAD_STATE);	
	}	 

	 if( __init() != 0) {
		 return 0;
	 }

	 ux_display_transaction_flow[offset++] = (ux_flow_step_t *)&ux_display_review_transaction_step;
	 
	 ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&step_upper_delimiter;
	 ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&step_generic;
	 ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&step_lower_delimiter;
	 
	 
	 
	 ux_display_transaction_flow[offset++] = (ux_flow_step_t *) &ux_display_accept_step;
	 ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&ux_display_reject_step;
	 ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)FLOW_END_STEP;
	 
	 
	 step_flow = 0;
	 
	 g_validate_callback = &ui_action_validate_transaction;
	 
	 
	 switch(get_context_tx_type()) { 
		 case 5:
			g_step_flow_callback = __ui_display_sign_tx_5;
			break;
		 case 6:
		 	g_step_flow_callback = __ui_display_sign_tx_6;
			break;
		 case 16:
		 	if((strcmp((char *)get_context_tx_info().call_contract.method_name,  "transfer") == 0 || strcmp((char *)get_context_tx_info().call_contract.method_name,  "transferFrom") == 0) && is_built_in_assets()) {
				 g_step_flow_callback = __ui_display_sign_tx_16_nrc20;
			} else {
				g_step_flow_callback = __ui_display_sign_tx_16_contract;
			}
			 break;  
		 default:			 
			g_step_flow_callback = __ui_display_sign_tx_x;
	 }
	 
	 
	 ux_flow_init(0, (const ux_flow_step_t* const *)ux_display_transaction_flow, NULL);


 	return 0;
 }




 void __dsp_pk_addrss_init() {
	strcpy(title, "Address");

	strcpy(text, "NULSd"); strlcpy(text + 5, G_context.pk_info.address_base_58, 75);
}

bool  __ui_display_public_key(bool is_next) {
	uint8_t sum  =  1;

	if(is_next) {
		
		step_flow++;

		if(step_flow > sum) {
			goto __F;
		}

	} else {
		step_flow--;

			
		if(step_flow == 0 ) {
			goto __F;

		}

	}

	__dsp_pk_addrss_init();

	return true;	
	
__F:
	PRINTF("Call get_next_data return false\n");
	return false;

	
}

int ui_display_public_key() {
	uint8_t offset = 0;
		
    if (G_context.req_type != CONFIRM_ADDRESS || G_context.state != STATE_NONE) {
        G_context.state = STATE_NONE;
        return io_send_sw(SW_BAD_STATE);
	}
    

	ux_display_transaction_flow[offset++] = (ux_flow_step_t *)&ux_display_review_address_step;

	ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&step_upper_delimiter;
	ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&step_generic;
	ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&step_lower_delimiter;


	
	ux_display_transaction_flow[offset++] = (ux_flow_step_t *) &ux_display_approve_step;
	ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)&ux_display_reject_step;
	ux_display_transaction_flow[offset++] =  (ux_flow_step_t *)FLOW_END_STEP;


	step_flow = 0;

    g_validate_callback = &ui_action_validate_pubkey;

	g_step_flow_callback = __ui_display_public_key;


	ux_flow_init(0, (const ux_flow_step_t* const *)ux_display_transaction_flow, NULL);

    return 0;
}

#endif
