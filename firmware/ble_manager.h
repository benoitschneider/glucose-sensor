#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/**@brief Function for initializing the BLE stack and services.
 */
void ble_manager_init(void);

/**@brief Function for starting BLE advertising.
 */
void ble_manager_start_advertising(void);

/**@brief Function for stopping BLE advertising and disconnecting if connected.
 */
void ble_manager_stop(void);

/**@brief Function for processing BLE events in the main loop.
 */
void ble_manager_process(void);

/**@brief Function for sending glucose data over BLE.
 * @param[in] glucose_value The glucose reading to send.
 * @param[in] timestamp The timestamp of the reading.
 * @retval NRF_SUCCESS on success, otherwise an error code.
 */
uint33_t ble_manager_send_glucose_data(uint16_t glucose_value, uint32_t timestamp);

#endif // BLE_MANAGER_H
