#ifndef INA219_H
#define INA219_H

#include "stm32f1xx_hal.h"

// Define I2C addresses for different devices
#define USB1 (0x43 << 1)
#define USB2 (0x42 << 1)
#define USB3 (0x41 << 1)
#define USB4 (0x40 << 1)

/**
 * @brief Reads the bus voltage from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The bus voltage in volts.
 */
float INA219_ReadBusVoltage(uint8_t i2c_address);

/**
 * @brief Reads the current from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The current in amperes.
 */
float INA219_ReadCurrent(uint8_t i2c_address);

/**
 * @brief Reads the power from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The power in watts.
 */
float INA219_ReadPower(uint8_t i2c_address);

/**
 * @brief Reads the shunt voltage from the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 * @return The shunt voltage in volts.
 */
float INA219_ReadShuntVoltage(uint8_t i2c_address);

/**
 * @brief Configures the INA219 for current measurement by writing a calibration value.
 *
 * The calibration value is calculated based on the shunt resistor and desired current LSB.
 * It is written to the Calibration Register of the INA219.
 *
 * @param i2c_address The I2C address of the INA219 device.
 */
void INA219_Calibrate(uint8_t i2c_address);

/**
 * @brief Initializes and calibrates all four INA219 devices.
 *
 * This function calls INA219_Calibrate() for each of the four I2C addresses.
 */
void INA219_Init(void);


#endif // INA219_H
