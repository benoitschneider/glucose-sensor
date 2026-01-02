#include "glucose_service.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

/**@brief Function for adding the Glucose Measurement characteristic.
 *
 * @param[in] p_gls        Glucose Service structure.
 * @param[in] p_gls_init   Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS on success.
 * @retval Other error code if the characteristic could not be added.
 */
static uint32_t glucose_measurement_char_add(ble_gls_t * p_gls, const ble_gls_init_t * p_gls_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_value_t   init_value;

    memset(&cccd_md, 0, sizeof(cccd_md));

    // Read/write security for CCCD. You can configure this to be more restrictive.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &p_gls_init->glucose_measurement_char_attr_md;
    attr_char_value.init_len  = 0; // Initial length will be dynamic
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_GLS_MAX_MEASUREMENT_LEN; // Define max length based on Glucose Measurement characteristic structure

    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_GLUCOSE_MEASUREMENT_CHAR);

    // Set read/write security for characteristic value.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_char_value.p_attr_md->read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_char_value.p_attr_md->write_perm);
    attr_char_value.p_attr_md->vloc = BLE_GATTS_VLOC_STACK; // Store in stack memory

    // Initialize with an empty value
    memset(&init_value, 0, sizeof(init_value));
    init_value.len = 0;
    init_value.p_value = NULL;

    return sd_ble_gatts_characteristic_add(p_gls->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_gls->glucose_measurement_handles);
}

uint32_t ble_gls_init(ble_gls_t * p_gls, const ble_gls_init_t * p_gls_init)
{
    if (p_gls == NULL || p_gls_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_gls->evt_handler = p_gls_init->evt_handler;
    p_gls->conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add custom base UUID (if not already added by other services)
    ble_uuid128_t base_uuid = {{0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_gls->uuid_type);
    APP_ERROR_CHECK(err_code);

    // Add service
    ble_uuid.type = p_gls->uuid_type;
    ble_uuid.uuid = BLE_UUID_GLUCOSE_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_gls->service_handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Glucose Service UUID: 0x%X", ble_uuid.uuid);
    NRF_LOG_INFO("Glucose Service handle: 0x%X", p_gls->service_handle);

    // Add Glucose Measurement characteristic
    err_code = glucose_measurement_char_add(p_gls, p_gls_init);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

uint32_t ble_gls_glucose_measurement_send(ble_gls_t * p_gls, uint16_t glucose_value, uint32_t timestamp)
{
    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    if (p_gls->conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    // Check if notifications are enabled for the characteristic
    uint16_t cccd_value = 0;
    ble_gatts_value_t cccd_gatts_value = {
        .len     = sizeof(uint16_t),
        .offset  = 0,
        .p_value = (uint8_t *)&cccd_value
    };

    err_code = sd_ble_gatts_value_get(p_gls->conn_handle,
                                      p_gls->glucose_measurement_handles.cccd_handle,
                                      &cccd_gatts_value);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    if (!(cccd_value & BLE_GATT_HVX_NOTIFICATION))
    {
        return NRF_ERROR_INVALID_STATE; // Notifications not enabled
    }

    // Construct the Glucose Measurement characteristic value payload
    // This is a simplified example. The actual Glucose Measurement characteristic
    // has a defined structure (flags, sequence number, base time, glucose concentration, etc.)
    // For this example, we'll send a simple structure.

    // Example payload structure (simplified for demonstration)
    // Flags (1 byte): 0x01 (Time offset present), 0x02 (Concentration type and location present)
    // Glucose Concentration (2 bytes, SFLOAT)
    // Unit (1 byte): 0x01 (mol/L), 0x02 (kg/L)
    // Time Stamp (7 bytes): year, month, day, hour, minute, second
    // Time Offset (2 bytes): minutes

    // For simplicity, let's just send glucose value and timestamp directly
    // A real implementation would follow the BLE Glucose Service specification (GATT_SPEC_V10.pdf)
    uint8_t data[5]; // 1 byte flags + 2 bytes glucose + 2 bytes timestamp (simplified)
    data[0] = 0x00; // Flags (no flags set for this simple example)
    data[1] = (uint8_t)(glucose_value & 0xFF);
    data[2] = (uint8_t)((glucose_value >> 8) & 0xFF);
    data[3] = (uint8_t)(timestamp & 0xFF);
    data[4] = (uint8_t)((timestamp >> 8) & 0xFF);

    memset(&gatts_value, 0, sizeof(gatts_value));
    gatts_value.len     = sizeof(data);
    gatts_value.offset  = 0;
    gatts_value.p_value = data;

    // Update the characteristic value in the SoftDevice
    err_code = sd_ble_gatts_value_set(p_gls->conn_handle,
                                     p_gls->glucose_measurement_handles.value_handle,
                                     &gatts_value);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Send notification
    ble_gatts_hvx_params_t hvx_params;
    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_gls->glucose_measurement_handles.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = 0;
    hvx_params.p_len  = &gatts_value.len;
    hvx_params.p_data = gatts_value.p_value;

    err_code = sd_ble_gatts_hvx(p_gls->conn_handle, &hvx_params);

    return err_code;
}
