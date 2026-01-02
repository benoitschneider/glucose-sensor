# app/CMakeLists.txt
# Defines the application executable target.

# Source files for the application
set(APP_SOURCES
    src/main.c
)

# Define the executable target
add_executable(GlucoseSensorFirmware ${APP_SOURCES})

# Link libraries (if any, e.g., drivers, BLE stack)
# These will be defined in other CMakeLists.txt files and linked here.
# For now, we link to the SDK's C runtime and common utilities.
target_link_libraries(GlucoseSensorFirmware
    # Add libraries from other subdirectories here, e.g.:
    # drivers_lib
    # ble_stack_lib
    # common_lib
)

# Add include directories specific to the application
target_include_directories(GlucoseSensorFirmware PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add definitions specific to the application
target_compile_definitions(GlucoseSensorFirmware PRIVATE
    APP_NAME="GlucoseSensor"
    APP_VERSION="1.0.0"
)
