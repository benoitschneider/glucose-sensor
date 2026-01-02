#ifndef BLE_GLS_H
#define BLE_GLS_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_UUID_GLUCOSE_SERVICE        0x1808                      /**< The UUID of the Glucose Service. */
#define BLE_UUID_GLUCOSE_MEASUREMENT_CHAR 0x2A18                    /**< The UUID of the Glucose Measurement Characteristic. */

// Forward declaration of the ble_gls_t type.
typedef struct ble_gls_s ble_gls_t;

/**@brief Glucose Service event handler type.
 * @details This is the type of the event handler that is to be provided by the application
 *          when GLS is initialized. Upon an event from the Glucose Service, this handler
 *          will be called to notify the application.
 *
 * @param[in] p_gls       Glucose Service structure.
 * @param[in] p_evt       Event received from the Glucose Service.
 */
typedef void (*ble_gls_evt_handler_t) (ble_gls_t * p_gls, void * p_evt);

/**@brief Glucose Service init structure.
 * @details This structure contains the initialization information for the service.
 */
typedef struct
{
    ble_gls_evt_handler_t   evt_handler;                                /**< Event handler to be called for events from the Glucose Service. */
    // Add any initial values or security settings for characteristics here
    ble_srv_cccd_security_mode_t glucose_measurement_char_attr_md;
} ble_gls_init_t;

/**@brief Glucose Service structure.
 * @details This structure contains status information for the service.
 */
typedef struct ble_gls_s
{
    uint16_t                    service_handle;                     /**< Handle of Glucose Service (as provided by the SoftDevice). */
    ble_gatts_char_handles_t    glucose_measurement_handles;        /**< Handles for the Glucose Measurement characteristic. */
    uint8_t                     uuid_type;                          /**< UUID type for the Glucose Service. */
    ble_gls_evt_handler_t       evt_handler;                        /**< Event handler to be called for events from the Glucose Service. */
}
ble_gls_t;

/**@brief Function for initializing the Glucose Service.
 * @param[out] p_gls      Glucose Service structure. This structure will have to be supplied to
 *                         the Glucose Service functions throughout the application's lifetime.
 * @param[in] p_gls_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS on successful initialization of service.
 * @retval NRF_ERROR_NULL if a NULL pointer was passed as input.
 * @retval Other error code if the service could not be initialized.
 */
uint32_t ble_gls_init(ble_gls_t * p_gls, const ble_gls_init_t * p_gls_init);

/**@brief Function for sending a Glucose Measurement notification.
 * @param[in] p_gls          Glucose Service structure.
 * @param[in] glucose_value  The glucose value to send.
 * @param[in] timestamp      The timestamp of the measurement.
 *
 * @retval NRF_SUCCESS on success.
 * @retval NRF_ERROR_INVALID_STATE if the device is not connected or notifications are not enabled.
 * @retval NRF_ERROR_BUSY if the SoftDevice is busy.
 */
uint32_t ble_gls_glucose_measurement_send(ble_gls_t * p_gls, uint16_t glucose_value, uint32_t timestamp);

#endif // BLE_GLS_H
