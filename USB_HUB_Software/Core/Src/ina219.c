#include <ina219.h>
#include "stm32f1xx_hal.h"

// I2C handle should be configured properly in your application
extern I2C_HandleTypeDef hi2c2;

/**
 * @brief Configures the INA219 for current measurement by writing a calibration value.
 *
 * The calibration value is calculated based on the shunt resistor and desired current LSB.
 * It is written to the Calibration Register of the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 */
void INA219_Calibrate(uint8_t i2c_address) {
    uint8_t buffer[2];
    uint16_t calibration_value;
    float current_lsb = 0.0001;   // 0.1 mA/bit (Current LSB)
    float r_shunt = 0.1;         // 100 mΩ shunt resistor
    uint8_t reg_address = 0x05;  // Address of the Calibration Register

    // Calculate the calibration value for the Calibration Register
    calibration_value = (uint16_t)((0.04096) / (current_lsb * r_shunt));

    buffer[0] = (calibration_value >> 8) & 0xFF;
    buffer[1] = calibration_value & 0xFF;

    // Send the calibration value to the Calibration Register
    HAL_I2C_Master_Transmit(&hi2c2, i2c_address, &reg_address, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c2, i2c_address, buffer, 2, HAL_MAX_DELAY);
}

/**
 * @brief Reads the bus voltage from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The bus voltage in volts.
 */
float INA219_ReadBusVoltage(uint8_t i2c_address) {
    uint8_t reg_address = 0x02; // Register address for bus voltage
    uint8_t buffer[2];

    HAL_I2C_Master_Transmit(&hi2c2, i2c_address, &reg_address, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c2, i2c_address, buffer, 2, HAL_MAX_DELAY);

    int16_t raw_bus_voltage = (buffer[0] << 8) | buffer[1];
    // Shift right by 3 bits and convert to volts
    return (raw_bus_voltage >> 3) * 0.004; // 4 mV per LSB
}

/**
 * @brief Reads the current from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The current in amperes.
 */
float INA219_ReadCurrent(uint8_t i2c_address) {
    uint8_t reg_address = 0x01; // Register address for current
    uint8_t buffer[2];

    HAL_I2C_Master_Transmit(&hi2c2, i2c_address, &reg_address, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c2, i2c_address, buffer, 2, HAL_MAX_DELAY);

    int16_t raw_current = (buffer[0] << 8) | buffer[1];
    // Current is directly multiplied by LSB
    return raw_current * 0.0001; // 100 µA per LSB
}

/**
 * @brief Reads the power from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The power in watts.
 */
float INA219_ReadPower(uint8_t i2c_address) {
    float current = INA219_ReadCurrent(i2c_address);
    float voltage = INA219_ReadBusVoltage(i2c_address);

    // Calculate power as the product of current and voltage
    float power = current * voltage;
    return power;
}

/**
 * @brief Reads the shunt voltage from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The shunt voltage in volts.
 */
float INA219_ReadShuntVoltage(uint8_t i2c_address) {
    uint8_t reg_address = 0x01; // Register address for shunt voltage
    uint8_t buffer[2];

    HAL_I2C_Master_Transmit(&hi2c2, i2c_address, &reg_address, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c2, i2c_address, buffer, 2, HAL_MAX_DELAY);

    int16_t raw_shunt_voltage = (buffer[0] << 8) | buffer[1];
    // Shunt voltage is directly multiplied by LSB
    return raw_shunt_voltage * 0.01; // 10 µV per LSB
}

/**
 * @brief Initializes and calibrates all four INA219 devices.
 *
 * This function calls INA219_Calibrate() for each of the four I2C addresses.
 */
void INA219_Init(void) {
    // Calibrate all four INA219 devices
    INA219_Calibrate(USB1);
    INA219_Calibrate(USB2);
    INA219_Calibrate(USB3);
    INA219_Calibrate(USB4);
}
