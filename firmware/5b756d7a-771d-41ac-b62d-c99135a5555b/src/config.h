#ifndef CONFIG_H
#define CONFIG_H

// --- ADS1115 Configuration ---
// Define the I2C address for the ADS1115.
// This can be changed based on the ADDR pin connection.
// 0x48 (GND), 0x49 (VCC), 0x4A (SDA), 0x4B (SCL)
#define ADS1115_I2C_ADDRESS     0x48

// --- I2C Driver Configuration ---
// Define the I2C peripheral instance to be used (e.g., I2C1, I2C2)
// This would typically map to a specific hardware I2C peripheral on the MCU.
#define I2C_PERIPHERAL_INSTANCE     I2C1

// Define I2C clock speed (e.g., 100000 for 100kHz, 400000 for 400kHz)
#define I2C_CLOCK_SPEED             100000

// --- General System Configuration ---
// Define system clock frequency (e.g., for delay functions, if needed)
#define SYSTEM_CLOCK_HZ             48000000

#endif // CONFIG_H
