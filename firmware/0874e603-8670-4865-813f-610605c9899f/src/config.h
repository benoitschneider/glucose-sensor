#ifndef CONFIG_H
#define CONFIG_H

// --- ADS1115 Configuration --- //
#define ADS1115_I2C_ADDRESS     0x48  // Default I2C address for ADS1115
#define ADS1115_CONVERSION_TIMEOUT_MS 100 // Maximum time to wait for a conversion

// I2C Driver Configuration (assuming an existing I2C driver)
// These should be defined based on your specific I2C driver's API
// #define I2C_MASTER_WRITE(address, data, len)  your_i2c_write_function(address, data, len)
// #define I2C_MASTER_READ(address, data, len)   your_i2c_read_function(address, data, len)
// #define I2C_MASTER_INIT()                     your_i2c_init_function()

#endif // CONFIG_H