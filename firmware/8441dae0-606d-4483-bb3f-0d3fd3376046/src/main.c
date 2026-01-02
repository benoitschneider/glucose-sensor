#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_rtc.h"
#include "app_timer.h"
#include "ble_manager.h"
#include "sensor_manager.h"
#include "power_manager.h"
#include "config.h"

// RTC instance for timed wake-ups
static const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(RTC_INSTANCE_ID);

/**@brief Function for initializing the nRF log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for initializing the Power Management module.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Callback function for RTC events.
 */
static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    if (int_type == NRF_DRV_RTC_INT_COMPARE0)
    {
        // RTC compare event occurred, wake up for measurement/BLE
        NRF_LOG_INFO("RTC Wakeup: Time for action!");
        power_manager_set_state(POWER_MODE_ACTIVE);
    }
}

/**@brief Function for initializing the RTC.
 */
static void rtc_config(void)
{
    ret_code_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = RTC_PRESCALER; // Set prescaler to achieve 1-second tick
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable the RTC instance
    nrf_drv_rtc_enable(&rtc);

    // Set compare value for initial wake-up
    // This will be updated by power_manager_set_next_wakeup_time in deep sleep
    err_code = nrf_drv_rtc_compare(
        &rtc, NRF_RTC_CC_0, 
        nrf_drv_rtc_counter_get(&rtc) + SECONDS_TO_TICKS(ACTIVE_MODE_INTERVAL_SEC), 
        true
    );
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("RTC configured.");
}

/**@brief Function for initializing the Low Frequency clock.
 */
static void lfclk_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request_start();
    while(!nrf_drv_clock_lfclk_is_running())
    {
        // Wait for LFCLK to start
    }
    NRF_LOG_INFO("LFCLK started.");
}

/**@brief Function for application main entry.
 */
int main(void)
{
    // Initialize modules.
    log_init();
    lfclk_init();
    app_timer_init(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    power_management_init();
    rtc_config();
    ble_manager_init();
    sensor_manager_init();

    NRF_LOG_INFO("Device started.");

    // Set initial power mode
    power_manager_set_state(POWER_MODE_ACTIVE);

    // Enter main loop.
    for (;;)
    {
        power_manager_run();
        // Process any pending events from the SoftDevice or other modules
        // This will put the CPU to sleep if no events are pending
        nrf_pwr_mgmt_run();
    }
}
