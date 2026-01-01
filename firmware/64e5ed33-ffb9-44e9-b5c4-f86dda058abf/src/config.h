#ifndef CONFIG_H
#define CONFIG_H

// --- ADS1115 Configuration --- //
#define ADS1115_I2C_ADDRESS     0x48  // Default I2C address for ADS1115 (ADDR pin tied to GND)

// --- I2C Configuration (Placeholder - actual values depend on MCU and I2C driver) ---
#define I2C_BUS_SPEED_HZ        150000 // 150 kHz
#define I2C_TIMEOUT_MS          100    // I2C communication timeout

#endif // CONFIG_H
