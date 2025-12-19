#include "ads1115.h"
#include "i2c_driver.h"
#include "config.h"

// Example main function for demonstration purposes
// In a real embedded system, this would be part of a larger application loop.
int main(void)
{
    // Initialize I2C driver (assuming this is done elsewhere or here)
    // i2c_driver_init(); 

    // Example 1: Configure ADS1115 for AIN0/AIN1 differential, +/-2.048V gain, 128 SPS
    if (ads1115_init(ADS1115_GAIN_PM2_048V, ADS1115_SPS_128, ADS1115_MUX_AIN0_AIN1) != ADS1115_OK)
    {
        // Handle error, e.g., log, blink LED, or halt
        while(1);
    }

    // Example 2: Configure ADS1115 for AIN2 single-ended, +/-0.256V gain, 8 SPS
    // This configuration would overwrite the previous one.
    // if (ads1115_init(ADS1115_GAIN_PM0_256V, ADS1115_SPS_8, ADS1115_MUX_AIN2_GND) != ADS1115_OK)
    // {
    //     // Handle error
    //     while(1);
    // }

    // Further application logic would go here, e.g., reading conversions
    while (1)
    {
        // Application loop
    }

    return 0;
}
