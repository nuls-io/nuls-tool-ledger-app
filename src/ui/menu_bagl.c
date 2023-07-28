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

#include "os.h"
#include "ux.h"
#include "glyphs.h"

#include "../globals.h"
#include "menu.h"

#define ENABLED_STR   "Enable transaction blind signing Enabled"
#define DISABLED_STR  "Enable transaction blind signing NOT Enabled"


char dis_buff[50];

extern const internalStorage_t N_storage_real;

void switch_settings_blind_signing(void);
void display_settings(const ux_flow_step_t* const start_step);

void ui_menu_main();



UX_STEP_NOCB(ux_menu_ready_step, pnn, {&C_boilerplate_logo, "NULS", "is ready"});
UX_STEP_NOCB(ux_menu_version_step, bn, {"Version", APPVERSION});
UX_STEP_CB(ux_menu_settings_step, pb, display_settings(NULL), {&C_icon_coggle, "Settings"});
UX_STEP_VALID(ux_menu_exit_step, pb, os_sched_exit(-1), {&C_icon_dashboard_x, "Quit"});


UX_FLOW(ux_menu_main_flow,
        &ux_menu_ready_step,
        &ux_menu_version_step,
        &ux_menu_settings_step,
        &ux_menu_exit_step,
        FLOW_LOOP);




UX_STEP_CB(ux_settings_flow_blind_signing_step, bnnn_paging, switch_settings_blind_signing(), { .title = "Blind Signing", .text = dis_buff});
UX_STEP_CB(ux_menu_back_step, pb, ui_menu_main(), {&C_icon_back, "Back"});

UX_FLOW(ux_settings_flow, 
	&ux_settings_flow_blind_signing_step, 
	&ux_menu_back_step,
	FLOW_LOOP);



void switch_settings_blind_signing(void) {
    uint8_t value = (N_storage.dataAllowed ? 0 : 1);
    nvm_write((void*) &N_storage.dataAllowed, (void*) &value, sizeof(uint8_t));

    display_settings(&ux_settings_flow_blind_signing_step);
	 
}



void display_settings(const ux_flow_step_t* const start_step) {

	strlcpy(dis_buff, N_storage.dataAllowed?ENABLED_STR:DISABLED_STR, 45);
	
    ux_flow_init(0, ux_settings_flow, start_step);
}

void ui_menu_main() {
    if (G_ux.stack_count == 0) {
        ux_stack_push();
    }

    ux_flow_init(0, ux_menu_main_flow, NULL);
}




#endif
