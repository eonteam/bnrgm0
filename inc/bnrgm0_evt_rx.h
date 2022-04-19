#ifndef __BNRGM0_EVT_RX_H_
#define __BNRGM0_EVT_RX_H_

#include "eonOS.h"

// ===============================================================
// Event callbacks
// ===============================================================

// This function is called when there is a LE Connection Complete event.
void hci_le_connection_complete_event(uint8_t peer_addr[6], uint16_t conn_handle);
// This function is called when the peer device get disconnected.
void hci_disconnection_complete_event(uint8_t status, uint8_t conn_handle, uint8_t reason);
// This function is called when an attribute gets modified
void aci_gatt_attribute_modified_event(uint16_t attr_handle, uint8_t data_length, uint8_t *attr_data);
// This function is called when there is a notification from the sever.
void aci_gatt_notification_event(uint16_t attr_handle, uint8_t attr_len, uint8_t *attr_value);
// This event is generated in response to an Exchange MTU request (local or from the peer).
void aci_att_exchange_mtu_resp_event(uint16_t conn_handle, uint16_t server_rx_mtu);

// ===============================================================
// Event process
// ===============================================================
void bnrgm0_event_rx(void *pData);

#endif