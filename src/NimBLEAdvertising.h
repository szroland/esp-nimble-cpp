/*
 * NimBLEAdvertising.h
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEAdvertising.h
 *
 *  Created on: Jun 21, 2017
 *      Author: kolban
 */

#ifndef MAIN_BLEADVERTISING_H_
#define MAIN_BLEADVERTISING_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)

#include "host/ble_gap.h"
/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

#include "NimBLEUUID.h"

#include <vector>

/* COMPATIBILITY - DO NOT USE */
#define ESP_BLE_ADV_FLAG_LIMIT_DISC         (0x01 << 0)
#define ESP_BLE_ADV_FLAG_GEN_DISC           (0x01 << 1)
#define ESP_BLE_ADV_FLAG_BREDR_NOT_SPT      (0x01 << 2)
#define ESP_BLE_ADV_FLAG_DMT_CONTROLLER_SPT (0x01 << 3)
#define ESP_BLE_ADV_FLAG_DMT_HOST_SPT       (0x01 << 4)
#define ESP_BLE_ADV_FLAG_NON_LIMIT_DISC     (0x00 )
 /* ************************* */


//TODO: review this against ble_hs_adv_fields
struct ble_hs_adv_fields_rw {
    /*** 0x01 - Flags. */
    uint8_t flags;

    /*** 0x02,0x03 - 16-bit service class UUIDs. */
    ble_uuid16_t *uuids16;
    uint8_t num_uuids16;
    unsigned uuids16_is_complete:1;

    /*** 0x04,0x05 - 32-bit service class UUIDs. */
    ble_uuid32_t *uuids32;
    uint8_t num_uuids32;
    unsigned uuids32_is_complete:1;

    /*** 0x06,0x07 - 128-bit service class UUIDs. */
    ble_uuid128_t *uuids128;
    uint8_t num_uuids128;
    unsigned uuids128_is_complete:1;

    /*** 0x08,0x09 - Local name. */
    const uint8_t *name;
    uint8_t name_len;
    unsigned name_is_complete:1;

    /*** 0x0a - Tx power level. */
    int8_t tx_pwr_lvl;
    unsigned tx_pwr_lvl_is_present:1;

    /*** 0x0d - Slave connection interval range. */
    const uint8_t *slave_itvl_range;

    /*** 0x16 - Service data - 16-bit UUID. */
    const uint8_t *svc_data_uuid16;
    uint8_t svc_data_uuid16_len;

    /*** 0x17 - Public target address. */
    const uint8_t *public_tgt_addr;
    uint8_t num_public_tgt_addrs;

    /*** 0x19 - Appearance. */
    uint16_t appearance;
    unsigned appearance_is_present:1;

    /*** 0x1a - Advertising interval. */
    uint16_t adv_itvl;
    unsigned adv_itvl_is_present:1;

    /*** 0x20 - Service data - 32-bit UUID. */
    const uint8_t *svc_data_uuid32;
    uint8_t svc_data_uuid32_len;

    /*** 0x21 - Service data - 128-bit UUID. */
    const uint8_t *svc_data_uuid128;
    uint8_t svc_data_uuid128_len;

    /*** 0x24 - URI. */
    const uint8_t *uri;
    uint8_t uri_len;

    /*** 0xff - Manufacturer specific data. */
    const uint8_t *mfg_data;
    uint8_t mfg_data_len;
};

/**
 * @brief Advertisement data set by the programmer to be published by the %BLE server.
 */
class NimBLEAdvertisementData {
    // Only a subset of the possible BLE architected advertisement fields are currently exposed.  Others will
    // be exposed on demand/request or as time permits.
    //
public:
    void setAppearance(uint16_t appearance);
    void setCompleteServices(const NimBLEUUID &uuid);
    void setFlags(uint8_t);
    void setManufacturerData(const std::string &data);
    void setName(const std::string &name);
    void setPartialServices(const NimBLEUUID &uuid);
    void setServiceData(const NimBLEUUID &uuid, const std::string &data);
    void setShortName(const std::string &name);
    void addData(const std::string &data);  // Add data to the payload.
    void addData(char * data, size_t length);
    std::string getPayload();               // Retrieve the current advert payload.

private:
    friend class NimBLEAdvertising;
    std::string m_payload;   // The payload of the advertisement.
};   // NimBLEAdvertisementData


/**
 * @brief Perform and manage %BLE advertising.
 *
 * A %BLE server will want to perform advertising in order to make itself known to %BLE clients.
 */
class NimBLEAdvertising {
public:
    NimBLEAdvertising();
    void addServiceUUID(const NimBLEUUID &serviceUUID);
    void addServiceUUID(const char* serviceUUID);
    void removeServiceUUID(const NimBLEUUID &serviceUUID);
    void start(uint32_t duration = 0, void (*advCompleteCB)(NimBLEAdvertising *pAdv) = nullptr);
    void stop();
    void setAppearance(uint16_t appearance);
    void setAdvertisementType(uint8_t adv_type);
    void setMaxInterval(uint16_t maxinterval);
    void setMinInterval(uint16_t mininterval);
    void setAdvertisementData(NimBLEAdvertisementData& advertisementData);
    void setScanFilter(bool scanRequestWhitelistOnly, bool connectWhitelistOnly);
    void setScanResponseData(NimBLEAdvertisementData& advertisementData);
    void setScanResponse(bool);
    void advCompleteCB();
    bool isAdvertising();

private:
    friend class NimBLEDevice;

    void                    onHostReset();
    static int              handleGapEvent(struct ble_gap_event *event, void *arg);

    ble_hs_adv_fields_rw       m_advData;
    ble_hs_adv_fields_rw       m_scanData;
    ble_gap_adv_params      m_advParams;
    std::vector<NimBLEUUID> m_serviceUUIDs;
    bool                    m_customAdvData;
    bool                    m_customScanResponseData;
    bool                    m_scanResp;
    bool                    m_advDataSet;
    void                   (*m_advCompCB)(NimBLEAdvertising *pAdv);

};

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_BLEADVERTISING_H_ */
