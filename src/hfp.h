/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at 
 * contact@bluekitchen-gmbh.com
 *
 */

// *****************************************************************************
//
//  HFP Hands-Free (HF) unit and Audio-Gateway Commons
//
// *****************************************************************************


#ifndef btstack_hfp_h
#define btstack_hfp_h

#include "hci.h"
#include "sdp_query_rfcomm.h"

#if defined __cplusplus
extern "C" {
#endif

#define HFP_Default_HF_Supported_Features 0x0000
#define HFP_Default_AG_Supported_Features 0x0009
#define HFP_MAX_NUM_CODECS 20

/* AT+BRSF Result: 
0: EC and/or NR function
1: Three-way calling
2: CLI presentation capability
3: Voice recognition activation
4: Remote volume control
5: Enhanced call status
6: Enhanced call control
7: Codec negotiation
8: HF Indicators
9: eSCO S4 (and T2) Settings Supported
10-31: Reserved for future definition
*/
/* +BRSF Result:
0: Three-way calling
1: EC and/or NR function
2: Voice recognition function
3: In-band ring tone capability
4: Attach a number to a voice tag
5: Ability to reject a call
6: Enhanced call status
7: Enhanced call control
8: Extended Error Result Codes
9: Codec negotiation
10: HF Indicators
11: eSCO S4 (and T2) Settings Supported
12-31: Reserved for future definition
*/
#define HFP_Supported_Features "+BRSF"
#define HFP_Available_Codecs "+BAC"
#define HFP_Codec_Indicator "+CIND"
#define HFP_Enable_Indicator_Status_Update "+CMER"
#define HFP_Support_Call_Hold_And_Multiparty_Services "+CHLD"
#define HFP_Generic_Status_Indicator "+BIND"
#define HFP_OK "OK"


typedef enum {
    HFP_IDLE,
    HFP_SDP_QUERY_RFCOMM_CHANNEL,
    HFP_W4_SDP_QUERY_COMPLETE,
    HFP_W4_RFCOMM_CONNECTED,
    HFP_W4_SUPPORTED_FEATURES_EXCHANGE,
    HFP_W4_CODEC_NEGOTIATION,
    HFP_W4_INDICATORS,
    HFP_W4_INDICATORS_STATUS,
    HFP_W4_INDICATORS_STATUS_UPDATE,
    HFP_W4_CAN_HOLD_CALL,
    HFP_W4_GENERIC_STATUS_INDICATORS,
    HFP_W4_HF_GENERIC_STATUS_INDICATORS,
    HFP_W4_AG_GENERIC_STATUS_INDICATORS,
    HFP_W4_INITITAL_STATE_GENERIC_STATUS_INDICATORS,

    HFP_CMD_SENT,
    
    HFP_ACTIVE,
    HFP_W2_DISCONNECT_RFCOMM,
    HFP_W4_RFCOMM_DISCONNECTED, 
    HFP_W4_CONNECTION_ESTABLISHED_TO_SHUTDOWN
} hfp_state_t;

typedef void (*hfp_callback_t)(uint8_t * event, uint16_t event_size);

typedef struct hfp_connection {
    linked_item_t    item;
    hfp_state_t state;

    bd_addr_t remote_addr;
    uint16_t con_handle;
    uint16_t rfcomm_channel_nr;
    uint16_t rfcomm_cid;

    uint8_t  wait_ok;
    uint8_t * codecs;

    hfp_callback_t callback;
} hfp_connection_t;

void hfp_create_service(uint8_t * service, uint16_t service_uuid, int rfcomm_channel_nr, const char * name, uint16_t supported_features);
void hfp_register_packet_handler(hfp_callback_t callback);
hfp_connection_t * hfp_handle_hci_event(uint8_t packet_type, uint8_t *packet, uint16_t size);
void hfp_init(uint16_t rfcomm_channel_nr);
void hfp_connect(bd_addr_t bd_addr, uint16_t service_uuid);

hfp_connection_t * get_hfp_connection_context_for_rfcomm_cid(uint16_t cid);

// TODO: move to utils
int send_str_over_rfcomm(uint16_t cid, char * command);
void join(char * buffer, int buffer_size, int buffer_offset, uint8_t * values, int values_nr);

#if defined __cplusplus
}
#endif

#endif