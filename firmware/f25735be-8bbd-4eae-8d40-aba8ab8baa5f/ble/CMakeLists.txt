# ble/CMakeLists.txt
# This file can be used to integrate BLE stack components or custom BLE services.

# For now, we'll define a placeholder library.
add_library(ble_stack_lib INTERFACE)

# Add common include directories for BLE components
target_include_directories(ble_stack_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Example: Nordic SDK BLE includes
    ${NRF5_SDK_PATH}/components/ble/common
    ${NRF5_SDK_PATH}/components/ble/ble_services/ble_bas
    ${NRF5_SDK_PATH}/components/softdevice/s132/headers
)

# Example of how to add a source file directly if no sub-directory is needed
# set(BLE_SOURCES
#     src/ble_service_glucose.c
# )
# target_sources(ble_stack_lib PRIVATE ${BLE_SOURCES})
