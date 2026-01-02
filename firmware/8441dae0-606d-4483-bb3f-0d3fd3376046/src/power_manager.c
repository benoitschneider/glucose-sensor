#include "power_manager.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_rtc.h"
#include "nrf_gpio.h"
#include "ble_manager.h"
#include "sensor_manager.h"
#include "config.h"

// External RTC instance from main.c
extern const nrf_drv_rtc_t rtc;

static power_mode_t m_current_power_mode = POWER_MODE_ACTIVE;

/**@brief Function to disable all unnecessary peripherals.
 */
static void peripherals_power_down(void)
{
    // Stop BLE advertising/connection if active
    ble_manager_stop();

    // Deinitialize sensor manager (ADC, I2C, etc.)
    sensor_manager_deinit();

    // Disable other peripherals as needed (e.g., SPI, UART, etc.)
    // For example, if UART is used for logging, it should be deinitialized
    // nrf_drv_uart_uninit(&m_uart);

    // Set GPIOs to low power state (e.g., input with pull-up/down or disconnected)
    // This is highly dependent on hardware design. Example:
    // nrf_gpio_cfg_default(LED_RED);
    // nrf_gpio_cfg_default(LED_GREEN);
    // nrf_gpio_pin_clear(LED_RED);
    // nrf_gpio_pin_clear(LED_GREEN);

    NRF_LOG_INFO("Peripherals powered down.");
}

/**@brief Function to enable necessary peripherals.
 */
static void peripherals_power_up(void)
{
    // Reinitialize sensor manager (ADC, I2C, etc.)
    sensor_manager_init();

    // Reinitialize other peripherals if they were powered down
    // nrf_drv_uart_init(&m_uart, &uart_config, uart_event_handle);

    // Start BLE advertising
    ble_manager_start_advertising();

    NRF_LOG_INFO("Peripherals powered up.");
}

/**@brief Function to set the next RTC wake-up time.
 * @param[in] seconds_from_now The number of seconds from now to wake up.
 */
void power_manager_set_next_wakeup_time(uint32_t seconds_from_now)
{
    ret_code_t err_code;
    uint32_t current_counter = nrf_drv_rtc_counter_get(&rtc);
    uint32_t compare_value = (current_counter + SECONDS_TO_TICKS(seconds_from_now)) % (NRF_RTC_COUNTER_MAX + 1);

    err_code = nrf_drv_rtc_compare(&rtc, NRF_RTC_CC_0, compare_value, true);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Next wakeup in %d seconds (RTC tick: %d).", seconds_from_now, compare_value);
}

/**@brief Function to transition to deep sleep mode.
 */
static void enter_deep_sleep(void)
{
    NRF_LOG_INFO("Entering Deep Sleep mode...");

    // Power down peripherals
    peripherals_power_down();

    // Set next wake-up time
    power_manager_set_next_wakeup_time(DEEP_SLEEP_INTERVAL_SEC);

    // Flush logs before deep sleep
    NRF_LOG_FINAL_FLUSH();

    // Enter System OFF mode (deepest sleep on nRF52)
    // This will reset the MCU upon wake-up, so all context must be saved if needed.
    // For this design, we re-initialize after wake-up.
    nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
}

/**@brief Function to transition to active mode.
 */
static void enter_active_mode(void)
{
    NRF_LOG_INFO("Entering Active mode...");

    // Power up peripherals
    peripherals_power_up();

    // Set next transition to deep sleep
    power_manager_set_next_wakeup_time(ACTIVE_MODE_INTERVAL_SEC);
}

void power_manager_set_state(power_mode_t new_state)
{
    if (m_current_power_mode == new_state)
    {
        return; // No state change needed
    }

    NRF_LOG_INFO("Power state transition: %s -> %s",
                 (m_current_power_mode == POWER_MODE_ACTIVE) ? "ACTIVE" : "DEEP_SLEEP",
                 (new_state == POWER_MODE_ACTIVE) ? "ACTIVE" : "DEEP_SLEEP");

    m_current_power_mode = new_state;

    switch (m_current_power_mode)
    {
        case POWER_MODE_ACTIVE:
            enter_active_mode();
            break;
        case POWER_MODE_DEEP_SLEEP:
            enter_deep_sleep();
            break;
        default:
            // Should not happen
            NRF_LOG_ERROR("Unknown power mode: %d", m_current_power_mode);
            break;
    }
}

power_mode_t power_manager_get_state(void)
{
    return m_current_power_mode;
}

void power_manager_run(void)
{
    // In active mode, perform tasks. When done, transition to deep sleep.
    if (m_current_power_mode == POWER_MODE_ACTIVE)
    {
        // Perform sensor measurements
        sensor_manager_perform_measurement();

        // Handle BLE events (advertising, connection, data transmission)
        ble_manager_process();

        // Check if active period is over
        if (nrf_drv_rtc_counter_get(&rtc) >= nrf_drv_rtc_compare_get(&rtc, NRF_RTC_CC_0))
        {
            power_manager_set_state(POWER_MODE_DEEP_SLEEP);
        }
    }
    // In deep sleep, the system is mostly idle, waiting for RTC interrupt.
    // The nrf_pwr_mgmt_run() in main will handle putting CPU to sleep.
}
