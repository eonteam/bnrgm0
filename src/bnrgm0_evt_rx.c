#include "bnrgm0_evt_rx.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"
#include "hci.h"
#include "hci_const.h"

// ===============================================================
// Weak functions
// ===============================================================

__weak void hci_le_connection_complete_event(uint8_t peer_addr[6], uint16_t conn_handle);
__weak void hci_disconnection_complete_event(uint8_t status, uint8_t conn_handle, uint8_t reason);
__weak void aci_gatt_attribute_modified_event(uint16_t conn_handle, uint16_t attr_handle, uint8_t data_length, uint8_t *attr_data);
__weak void aci_gatt_notification_event(uint16_t conn_handle, uint16_t attr_handle, uint8_t attr_len, uint8_t *attr_value);
__weak void aci_att_exchange_mtu_resp_event(uint16_t conn_handle, uint16_t server_rx_mtu);

// ===============================================================
// Main event
// ===============================================================

void bnrgm0_event_rx(void *pData) {
  hci_uart_pckt *hci_pckt = pData;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt *) hci_pckt->data;

  if (hci_pckt->type != HCI_EVENT_PKT)
    return;

  switch (event_pckt->evt) {

    case EVT_DISCONN_COMPLETE: {
      evt_disconn_complete *evt = (void *) event_pckt->data;
      hci_disconnection_complete_event(evt->status, evt->handle, evt->reason);
    } break;

    case EVT_LE_META_EVENT: {
      evt_le_meta_event *evt = (void *) event_pckt->data;

      switch (evt->subevent) {
        case EVT_LE_CONN_COMPLETE: {
          evt_le_connection_complete *cc = (void *) evt->data;
          hci_le_connection_complete_event(cc->peer_bdaddr, cc->handle);
        } break;
      }
    } break;

    case EVT_VENDOR: {
      evt_blue_aci *blue_evt = (void *) event_pckt->data;
      switch (blue_evt->ecode) {

        case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED: {
          evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1 *) blue_evt->data;
          aci_gatt_attribute_modified_event(evt->conn_handle, evt->attr_handle, evt->data_length, evt->att_data);
        } break;
        case EVT_BLUE_GATT_NOTIFICATION: { // when the device work as CLIENT mode
          evt_gatt_attr_notification *evt = (evt_gatt_attr_notification *) blue_evt->data;
          aci_gatt_notification_event(evt->conn_handle, evt->attr_handle, evt->event_data_length - 2, evt->attr_value);
        } break;
        case EVT_BLUE_ATT_EXCHANGE_MTU_RESP: {
          evt_att_exchange_mtu_resp *evt = (evt_att_exchange_mtu_resp *) blue_evt->data;
          aci_att_exchange_mtu_resp_event(evt->conn_handle, evt->server_rx_mtu);
        } break;
      }
      break;
    }
  }
}
