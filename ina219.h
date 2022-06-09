/*
 * INA219.h
 *
 *  Created on: Dec 30, 2020
 *      Author: Piotr Smolen <komuch@gmail.com>
 * 	Original github repository : https://github.com/komuch/PSM_INA219_STM32
 *
 *	I don't know if I really should use it like this, there was no message from the author about how to use
 * 	this library, with which license, ...
 */

#ifndef INC_INA219_H_
#define INC_INA219_H_

#include "main.h"


#define SHUNT_RESISTOR 100		// 0.1 Ohm already on the current sensor


#define INA219_DEFAULT_ADDRESS (0x40)

//
//	Registers
//
#define	INA219_REG_CONFIG 					0x00U
#define	INA219_REG_SHUNTVOLTAGE			0x01U
#define	INA219_REG_BUSVOLTAGE				0x02U
#define	INA219_REG_POWER						0x03U
#define	INA219_REG_CURRENT					0x04U
#define	INA219_REG_CALIBRATION			0x05U
//
#define INA219_CONFIG_RESET 				0x8000U
//
#define INA219_CONFIG_BVOLTAGERANGE_16V			0x0000U // 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V			0x2000U // 0-32V Range

#define INA219_CONFIG_GAIN_MASK					0x1800U		// Config PGA gain mask
#define	INA219_CONFIG_GAIN_1_40MV				0x0000U  	// Gain 1, 40mV Range
#define	INA219_CONFIG_GAIN_2_80MV				0x0800U 	// Gain 2, 80mV Range
#define	INA219_CONFIG_GAIN_4_160MV			0x1000U 	// Gain 4, 160mV Range
#define	INA219_CONFIG_GAIN_8_320MV			0x1800U 	// Gain 8, 320mV Range

#define	INA219_CONFIG_BADCRES_9BIT							(0x0000)  // 9-bit bus res = 0..511
#define	INA219_CONFIG_BADCRES_10BIT							(0x0080) // 10-bit bus res = 0..1023
#define	INA219_CONFIG_BADCRES_11BIT							(0x0100) // 11-bit bus res = 0..2047
#define	INA219_CONFIG_BADCRES_12BIT							(0x0180) // 12-bit bus res = 0..4097
#define	INA219_CONFIG_BADCRES_12BIT_2S_1060US 	(0x0480) // 2 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_4S_2130US		(0x0500) // 4 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_8S_4260US		(0x0580) // 8 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_16S_8510US	(0x0600) // 16 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_32S_17MS		(0x0680) // 32 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_64S_34MS		(0x0700) // 64 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_128S_69MS		(0x0780) // 128 x 12-bit bus samples averaged together

#define	INA219_CONFIG_SADCRES_9BIT_1S_84US			(0x0000) // 1 x 9-bit shunt sample
#define	INA219_CONFIG_SADCRES_10BIT_1S_148US		(0x0008) // 1 x 10-bit shunt sample
#define	INA219_CONFIG_SADCRES_11BIT_1S_276US		(0x0010) // 1 x 11-bit shunt sample
#define	INA219_CONFIG_SADCRES_12BIT_1S_532US		(0x0018) // 1 x 12-bit shunt sample
#define	INA219_CONFIG_SADCRES_12BIT_2S_1060US		(0x0048) // 2 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_4S_2130US		(0x0050) // 4 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_8S_4260US		(0x0058) // 8 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_16S_8510US	(0x0060) // 16 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_32S_17MS		(0x0068) // 32 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_64S_34MS		(0x0070) // 64 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_128S_69MS		(0x0078) // 128 x 12-bit shunt samples averaged together
//
#define INA219_CONFIG_MODE_MASK									0x07
#define	INA219_CONFIG_MODE_POWERDOWN						0x0 /**< power down */
#define	INA219_CONFIG_MODE_SVOLT_TRIGGERED			0x1 /**< shunt voltage triggered */
#define	INA219_CONFIG_MODE_BVOLT_TRIGGERED			0x2 /**< bus voltage triggered */
#define	INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED	0x3 /**< shunt and bus voltage triggered */
#define	INA219_CONFIG_MODE_ADCOFF								0x4 /**< ADC off */
#define	INA219_CONFIG_MODE_SVOLT_CONTINUOUS			0x5 /**< shunt voltage continuous */
#define	INA219_CONFIG_MODE_BVOLT_CONTINUOUS			0x6 /**< bus voltage continuous */
#define	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS 0x7


/**
* @brief Holds data on a INA219. Must not be initialised or modified directly. Use INA219_Init() and the others dedicated functions
*/
typedef struct
{
	I2C_HandleTypeDef 	*ina219_i2c;	// HAl I2C handler associated with the I2C pins used to communicate
	uint8_t	Address;									// Slave address of the INA219
	double current_LSB;								// For current register, depends of the epected maximum current value in A
} INA219_t;

/**
* @brief Initialises a ina219_t struct and reset the INA219.
*
* @param i2c: the I2C_HandleTypeDef.
* @param Adress: the I2C adress of the INA219.
* @param maxCurrent: the maximum expected current in A
*
* @retval Return 1 if the I2C interface is ready and the INA219 has been reset, return 0 otherwise.
*/
uint8_t INA219_Init(INA219_t *ina219, I2C_HandleTypeDef *i2c, uint8_t Address, double maxCurrent);


/* === Read functions === */

/**
* @brief Reads the raw value stored in the shunt boltage register
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the register value
*/
uint16_t INA219_ReadRawShuntVolage(INA219_t *ina219);

/**
* @brief Reads the shunt voltage
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the shunt voltage in mV
*/
float INA219_ReadShuntVoltage(INA219_t *ina219);

/**
* @brief Reads the raw value stored in the bus voltage register
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the register value
*/
uint16_t INA219_ReadRawBusVoltage(INA219_t * ina219);

/**
* @brief Reads the bus voltage
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the bus voltage in mV
*/
uint16_t INA219_ReadBusVoltage(INA219_t *ina219);

/**
* @brief Returns the raw value of the current register
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the register value
*/
uint16_t INA219_ReadRawCurrent(INA219_t *ina219);

/**
* @brief Reads the current
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the current in A
*/
double INA219_ReadCurrent(INA219_t *ina219);

/**
* @brief Returns the raw value of the power register
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the register value
*/
uint16_t INA219_ReadRawPower(INA219_t * ina219);

/**
*	@brief Returns the power
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the power in W
*/
double INA219_ReadPower(INA219_t * ina219);



/* === Configuration functions === */

/**
* @brief Reset the INA129 with the initial register configuration.
*
* This function set the RST bit in the configuration.
* @param ina219: pointer to the INA219_t struct to be used
*/
void INA219_Reset(INA219_t *ina219);

/**
* @brief Gets the current configuration
*
* @param ina219: pointer to the INA219_t struct to be used
* @retval the configuration register value
*/
uint16_t INA219_getConfig(INA219_t *ina219);

/**
* @brief Sets the configuration register
*
* @param ina219: pointer to the INA219_t struct to be used
* @param Config: the new uint16_t value of the configuration register
*/
void INA219_setConfig(INA219_t *ina219, uint16_t Config);

/**
* @brief Sets the calibration register
*
* @param ina219: pointer to the INA219_t struct to be used
* @param Config: the new uint16_t value of the calibration register
*/
void INA219_setCalibration(INA219_t *ina219, uint16_t CalibrationData);

/**
* @brief Sets the conversion and power mode
*
* @param ina219: pointer to the INA219_t struct to be used
* @param Mode: the mode (only bits 0 to 2 have meaning)
*/
void INA219_setPowerMode(INA219_t *ina219, uint8_t Mode);

/* === Read/Write register I2C functions === */
// These are not specificcally intended to be used directly, prefer the use of the previous dedicated functions

uint16_t Read16(INA219_t *ina219, uint8_t Register);
void Write16(INA219_t *ina219, uint8_t Register, uint16_t Value);



#endif /* INC_INA219_H_ */
