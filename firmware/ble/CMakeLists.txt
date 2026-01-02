add_library(ble_target STATIC
    # Add BLE stack adapter source files here
    # e.g., ble_stack_adapter/src/ble_adapter.c
)

target_include_directories(ble_target PUBLIC
    # Add BLE stack adapter include directories here
    # e.g., ${CMAKE_CURRENT_SOURCE_DIR}/ble_stack_adapter/inc
)
