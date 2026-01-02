#include "sensor_manager.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "nrf_drv_saadc.h"
#include "app_timer.h"
#include "ble_manager.h"
#include "config.h"

// TWI instance for I2C communication (e.g., with ADS1115)
#define TWI_INSTANCE_ID     0
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// SAADC instance for direct analog measurements if needed
#define SAMPLES_IN_BUFFER 1
static nrf_saadc_value_t m_buffer_pool[2][SAMPLES_IN_BUFFER];

APP_TIMER_DEF(m_sensor_timer_id);

/**@brief TWI event handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            // TWI transfer complete
            break;
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            NRF_LOG_ERROR("TWI Address NACK");
            break;
        case NRF_DRV_TWI_EVT_DATA_NACK:
            NRF_LOG_ERROR("TWI Data NACK");
            break;
        default:
            break;
    }
}

/**@brief Function for initializing TWI (I2C) peripheral.
 */
static void twi_init(void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
        .scl                = NRF_GPIO_PIN_MAP(0, 27), // Example SCL pin
        .sda                = NRF_GPIO_PIN_MAP(0, 26), // Example SDA pin
        .frequency          = NRF_DRV_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
        .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
    NRF_LOG_INFO("TWI initialized.");
}

/**@brief SAADC event handler.
 */
void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

        // Process SAADC result if direct analog input is used
        // nrf_saadc_value_t adc_result = p_event->data.done.p_buffer[0];
        // NRF_LOG_INFO("SAADC result: %d", adc_result);
    }
}

/**@brief Function for initializing SAADC peripheral.
 */
static void saadc_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
    // Configure AIN0 for sensor input

    err_code = nrf_drv_saadc_init(NULL, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("SAADC initialized.");
}

/**@brief Callback for the sensor measurement timer.
 */
static void sensor_measurement_timer_handler(void * p_context)
{
    ARG_UNUSED(p_context);
    NRF_LOG_INFO("Sensor timer triggered.");
    sensor_manager_perform_measurement();
}

void sensor_manager_init(void)
{
    // Initialize TWI for ADS1115 (example)
    twi_init();

    // Initialize SAADC (if direct analog input is also used)
    // saadc_init();

    // Create and start a timer for periodic measurements if needed within active mode
    ret_code_t err_code = app_timer_create(&m_sensor_timer_id, APP_TIMER_MODE_REPEATED, sensor_measurement_timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_sensor_timer_id, APP_TIMER_TICKS(SENSOR_MEASUREMENT_INTERVAL_MS), NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Sensor Manager initialized.");
}

void sensor_manager_deinit(void)
{
    ret_code_t err_code;

    // Stop measurement timer
    err_code = app_timer_stop(m_sensor_timer_id);
    APP_ERROR_CHECK(err_code);

    // Deinitialize TWI
    nrf_drv_twi_disable(&m_twi);
    nrf_drv_twi_uninit(&m_twi);

    // Deinitialize SAADC
    // nrf_drv_saadc_uninit();

    NRF_LOG_INFO("Sensor Manager deinitialized.");
}

uint16_t sensor_manager_perform_measurement(void)
{
    // Placeholder for actual sensor reading logic
    // For ADS1115, this would involve TWI communication to read ADC conversions.
    // For direct SAADC, this would involve triggering a SAADC conversion.

    // Example: Read from ADS1115 (simplified)
    uint8_t tx_buf[3];
    uint8_t rx_buf[2];
    uint16_t raw_adc_value = 0;
    ret_code_t err_code;

    // Configuration for ADS1115 (e.g., single-shot conversion on AIN0, PGA +/-2.048V)
    // This is a highly simplified example. Actual ADS1115 communication requires more steps.
    // Refer to ADS1115 datasheet for full details.
    tx_buf[0] = 0x01; // Pointer to configuration register
    tx_buf[1] = 0xC3; // MSB: Start conversion, AIN0/GND, PGA +/-2.048V, continuous mode (or single-shot)
    tx_buf[2] = 0x83; // LSB: 128 SPS, traditional comparator, active low, non-latching, disable comparator

    // Write config to ADS1115
    err_code = nrf_drv_twi_tx(&m_twi, ADS1115_ADDRESS, tx_buf, sizeof(tx_buf), false);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("ADS1115 config write failed: %d", err_code);
        return 0; // Return 0 on error
    }

    // Wait for conversion to complete (depends on data rate, could use DRDY pin or polling)
    // For simplicity, a small delay. In production, use interrupt or check conversion ready bit.
    nrf_delay_ms(100); 

    // Set pointer to conversion register (0x00)
    tx_buf[0] = 0x00;
    err_code = nrf_drv_twi_tx(&m_twi, ADS1115_ADDRESS, tx_buf, 1, true);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("ADS1115 pointer write failed: %d", err_code);
        return 0; // Return 0 on error
    }

    // Read conversion result
    err_code = nrf_drv_twi_rx(&m_twi, ADS1115_ADDRESS, rx_buf, sizeof(rx_buf));
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("ADS1115 read failed: %d", err_code);
        return 0; // Return 0 on error
    }

    raw_adc_value = (rx_buf[0] << 8) | rx_buf[1];

    // Convert raw ADC value to glucose value (this is a complex calculation
    // involving calibration, temperature compensation, etc. - placeholder here)
    uint16_t glucose_value = raw_adc_value / 100; // Example conversion

    NRF_LOG_INFO("Sensor measurement: Raw ADC = %d, Glucose = %d", raw_adc_value, glucose_value);

    // Send data via BLE
    uint32_t current_time_seconds = nrf_drv_rtc_counter_get(&rtc) / RTC_TICK_FREQ_HZ; // Get current time from RTC
    ble_manager_send_glucose_data(glucose_value, current_time_seconds);

    return glucose_value;
}
