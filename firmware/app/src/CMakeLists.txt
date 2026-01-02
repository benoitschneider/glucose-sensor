add_executable(${PROJECT_NAME}.elf main.c)

target_link_libraries(${PROJECT_NAME}.elf
    drivers_target
    common_target
    config_target
)
