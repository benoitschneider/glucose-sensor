#include "ble_manager.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "fds.h"
#include "ble_bas.h"
#include "ble_dis.h"
#include "glucose_service.h" // Custom Glucose Service
#include "config.h"

#define DEAD_BEEF                       0xDEADBEEF                          /**< Value used as error code for stack dump, can be used to identify stack corruption. */

NRF_BLE_GATT_DEF(m_gatt);
NRF_BLE_QWR_DEF(m_qwr);
BLE_ADVERTISING_DEF(m_advertising);

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */
static uint16_t m_ble_adv_interval; // Advertising interval in units of 0.625 ms

static ble_bas_t m_bas;
static ble_dis_t m_dis;
static ble_gls_t m_gls; // Custom Glucose Service instance

static ble_uuid_t m_adv_uuids[] = /**< Universally unique service identifiers. */
{
    {BLE_UUID_GLUCOSE_SERVICE, BLE_UUID_TYPE_VENDOR_BEGIN}, // Custom Glucose Service UUID
    {BLE_UUID_BATTERY_SERVICE, BLE_UUID_TYPE_BLE}
};

/**@brief Function for assert in the SoftDevice.
 * @details This function will be called in case of an assert in the SoftDevice. 
 *          This handler is an example only and does not fit a final product. You need to analyze 
 *          how your product is supposed to react in case of Assert.
 * @param[in] line_num   Line number of the failing ASSERT call.
 * @param[in] file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for handling BLE events.
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Pointer to user defined context.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("BLE Connected.");
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("BLE Disconnected.");
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            // Restart advertising if not in deep sleep
            if (power_manager_get_state() == POWER_MODE_ACTIVE)
            {
                ble_manager_start_advertising();
            }
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys = {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
            break;
        }

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout.
            NRF_LOG_WARNING("GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout.
            NRF_LOG_WARNING("GATT Server Timeout.");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_ADV_SET_TERMINATED:
            NRF_LOG_INFO("Advertising terminated.");
            // If advertising terminated and not connected, and in active mode, restart advertising
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID && power_manager_get_state() == POWER_MODE_ACTIVE)
            {
                ble_manager_start_advertising();
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for the SoftDevice initialization.
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack to relevant number of connections, GATT queues, and advertising sets.
    // The configuration is defined in sdk_config.h
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER("BLE_APP_OBSERVER", APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

/**@brief Function for handling Peer Manager events.
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const * p_evt)
{
    ret_code_t err_code;

    switch (p_evt->evt_id)
    {
        case PM_EVT_BONDED_PEER_CONNECTED:
            NRF_LOG_INFO("PM_EVT_BONDED_PEER_CONNECTED");
            break;

        case PM_EVT_CONN_SEC_SUCCEEDED:
            NRF_LOG_INFO("PM_EVT_CONN_SEC_SUCCEEDED");
            break;

        case PM_EVT_CONN_SEC_FAILED:
            /** In some cases, an unsuccessful pairing/bonding process might be caused by an application
             *  error, for example, if the device attempts to establish a secure connection using bonding
             *  information from a previous bonding that is no longer valid. To avoid this situation,
             *  it is recommended to delete the bonding information and restart the bonding process.
             */
            switch (p_evt->params.conn_sec_failed.error)
            {
                case PM_CONN_SEC_ERROR_PIN_OR_KEY_MISSING:
                    // Rebond if keys are missing
                    err_code = pm_peers_delete();
                    APP_ERROR_CHECK(err_code);
                    break;

                default:
                    break;
            }
            break;

        case PM_EVT_CONN_SEC_START:
            NRF_LOG_INFO("PM_EVT_CONN_SEC_START");
            break;

        case PM_EVT_PEERS_DELETE_SUCCEEDED:
            ble_manager_start_advertising();
            break;

        case PM_EVT_LOCAL_DB_CACHE_APPLIED:
            NRF_LOG_INFO("PM_EVT_LOCAL_DB_CACHE_APPLIED");
            break;

        case PM_EVT_LOCAL_DB_CACHE_REESTABLISHED:
            NRF_LOG_INFO("PM_EVT_LOCAL_DB_CACHE_REESTABLISHED");
            break;

        case PM_EVT_AUTH_STATUS_IDLE:
            NRF_LOG_INFO("PM_EVT_AUTH_STATUS_IDLE");
            break;

        case PM_EVT_ERROR_UNEXPECTED:
            APP_ERROR_CHECK(p_evt->params.error_unexpected.error_src);
            break;

        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
            NRF_LOG_INFO("PM_EVT_PEER_DATA_UPDATE_SUCCEEDED");
            break;

        case PM_EVT_PEER_DATA_UPDATE_FAILED:
            NRF_LOG_INFO("PM_EVT_PEER_DATA_UPDATE_FAILED");
            break;

        case PM_EVT_STORAGE_FULL:
            // Handle storage full event, e.g., delete oldest peer
            err_code = fds_gc(); // Attempt garbage collection
            if (err_code != NRF_SUCCESS && err_code != FDS_ERR_NO_SPACE_IN_QUEUES)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            break;
    }
}

/**@brief Function for the Peer Manager initialization.
 */
static void peer_manager_init(void)
{
    ble_gap_sec_params_t sec_param;
    ret_code_t           err_code;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters. (Adjust as per requirements)
    sec_param.bond         = 1;
    sec_param.mitm         = 0;
    sec_param.lesc         = 0;
    sec_param.keypress     = 0;
    sec_param.io_caps      = BLE_GAP_IO_CAPS_NONE;
    sec_param.oob          = 0;
    sec_param.min_key_size = 7;
    sec_param.max_key_size = 16;
    sec_param.kdist_own.enc  = 1;
    sec_param.kdist_own.id   = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(pm_evt_handler);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the advertising module.
 */
static void advertising_init(void)
{
    ret_code_t             err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type               = BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME;
    init.advdata.include_appearance      = true;
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    init.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.advdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = MSEC_TO_UNITS(BLE_ADVERTISING_INTERVAL_MS, UNIT_0_625_MS); // Configurable advertising interval
    init.config.ble_adv_fast_timeout  = 0; // Advertise indefinitely in active mode

    init.evt_handler = NULL; // Handled by ble_evt_handler for now

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    ret_code_t         err_code;
    nrf_ble_qwr_init_t qwr_init = {0};
    ble_bas_init_t     bas_init;
    ble_dis_init_t     dis_init;
    ble_gls_init_t     gls_init;

    // Initialize NRF_BLE_QWR module
    qwr_init.error_handler = nrf_qwr_error_handler;
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Battery Service.
    memset(&bas_init, 0, sizeof(bas_init));

    // Here you would set initial battery level, security requirements, etc.
    bas_init.evt_handler          = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref         = NULL;
    bas_init.initial_batt_level   = 100; // Example: 100% battery

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_report_read_perm);

    err_code = ble_bas_init(&m_bas, &bas_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Device Information Service.
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)"Nordic Semiconductor");
    ble_srv_ascii_to_utf8(&dis_init.model_num_str,     (char *)"nRF52832_Sensor");
    ble_srv_ascii_to_utf8(&dis_init.serial_num_str,    (char *)"123456789");
    ble_srv_ascii_to_utf8(&dis_init.hw_rev_str,        (char *)"1.0");
    ble_srv_ascii_to_utf8(&dis_init.fw_rev_str,        (char *)"1.0.0");
    ble_srv_ascii_to_utf8(&dis_init.sw_rev_str,        (char *)"1.0.0");

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err_code = ble_dis_init(&m_dis, &dis_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Custom Glucose Service.
    memset(&gls_init, 0, sizeof(gls_init));
    gls_init.evt_handler = NULL; // No specific event handler needed for now
    err_code = ble_gls_init(&m_gls, &gls_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling a Connection Parameters event.
 * @details This function will be called for all events from the Connection Parameters module
 *          which are passed to the application.
 * @note    All parameters must be non NULL.
 * @param[in] p_evt  Event received from the Connection Parameters module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

/**@brief Function for handling a Connection Parameters error.
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = APP_TIMER_TICKS(5000); // 5 seconds
    cp_init.next_conn_params_update_delay  = APP_TIMER_TICKS(30000); // 30 seconds
    cp_init.max_conn_params_update_count   = 3;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

void ble_manager_init(void)
{
    ble_stack_init();
    gatt_init();
    advertising_init();
    services_init();
    conn_params_init();
    peer_manager_init();

    NRF_LOG_INFO("BLE Manager initialized.");
}

void ble_manager_start_advertising(void)
{
    ret_code_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("BLE Advertising started.");
}

void ble_manager_stop(void)
{
    ret_code_t err_code;

    // Stop advertising
    err_code = sd_ble_gap_adv_stop(m_advertising.adv_handle);
    if (err_code != NRF_SUCCESS && err_code != NRF_ERROR_INVALID_STATE)
    {
        APP_ERROR_CHECK(err_code);
    }

    // Disconnect if connected
    if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        if (err_code != NRF_SUCCESS && err_code != NRF_ERROR_INVALID_STATE)
        {
            APP_ERROR_CHECK(err_code);
        }
    }
    NRF_LOG_INFO("BLE stopped.");
}

void ble_manager_process(void)
{
    // This function can be used to handle any periodic BLE tasks if needed.
    // For example, updating battery level, checking connection status, etc.
    // Most BLE events are handled by the event handler.
}

uint33_t ble_manager_send_glucose_data(uint16_t glucose_value, uint32_t timestamp)
{
    if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        NRF_LOG_WARNING("Cannot send glucose data: not connected.");
        return NRF_ERROR_INVALID_STATE;
    }

    // Implement actual glucose data sending via custom service
    // This is a placeholder, replace with actual GLS characteristic update
    ret_code_t err_code = ble_gls_glucose_measurement_send(&m_gls, glucose_value, timestamp);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Failed to send glucose data: %d", err_code);
    }
    else
    {
        NRF_LOG_INFO("Glucose data sent: %d at %d", glucose_value, timestamp);
    }
    return err_code;
}
