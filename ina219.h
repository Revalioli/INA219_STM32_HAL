/**
 * \file INA219.h
 *
 * Based on the github repository : https://github.com/komuch/PSM_INA219_STM32
 */

#ifndef INC_INA219_H_
#define INC_INA219_H_

/* === Include here the HAL header corresponding to your board === */
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif


#define SHUNT_RESISTOR 100		///< 0.1 Ohm shunt resistor already on the Adafruit arduino breakout


#define INA219_DEFAULT_ADDRESS (0x40U)	///< I2C adress if A0 and A1 on GND (default address on the Adafruit arduino breakout)

/* === Registers === */
#define	INA219_REG_CONFIG				(0x00U)	///< Configuration register address
#define	INA219_REG_SHUNTVOLTAGE			(0x01U)	///< Shunt voltage register address
#define	INA219_REG_BUSVOLTAGE			(0x02U)	///< Bus voltage register address
#define	INA219_REG_POWER				(0x03U)	///< Power register address
#define	INA219_REG_CURRENT				(0x04U)	///< Current register address
#define	INA219_REG_CALIBRATION			(0x05U)	///< Calobration register address

#define INA219_CONFIG_RESET 			(0x8000U)	///< Reset configuration

#define INA219_CONFIG_BVOLTAGERANGE_16V				(0x0000U)	///< 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V				(0x2000U)	///< 0-32V Range

#define INA219_CONFIG_GAIN_MASK						(0x1800U)	///< Config PGA gain mask
#define	INA219_CONFIG_GAIN_1_40MV					(0x0000U)	///< Gain /1, 40mV Range
#define	INA219_CONFIG_GAIN_2_80MV					(0x0800U)	///< Gain /2, 80mV Range
#define	INA219_CONFIG_GAIN_4_160MV					(0x1000U)	///< Gain /4, 160mV Range
#define	INA219_CONFIG_GAIN_8_320MV					(0x1800U)	///< Gain /8, 320mV Range

#define	INA219_CONFIG_BADCRES_9BIT					(0x0000U)	///< 9-bit bus sample
#define	INA219_CONFIG_BADCRES_10BIT					(0x0080U)	///< 10-bit bus sample
#define	INA219_CONFIG_BADCRES_11BIT					(0x0100U)	///< 11-bit bus sample
#define	INA219_CONFIG_BADCRES_12BIT					(0x0180U)	///< 12-bit bus sample
#define	INA219_CONFIG_BADCRES_12BIT_2S 				(0x0480U)	///< 2 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_4S				(0x0500U)	///< 4 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_8S				(0x0580U)	///< 8 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_16S				(0x0600U)	///< 16 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_32S				(0x0680U)	///< 32 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_64S				(0x0700U)	///< 64 x 12-bit bus samples averaged together
#define	INA219_CONFIG_BADCRES_12BIT_128S			(0x0780U)	///< 128 x 12-bit bus samples averaged together

#define	INA219_CONFIG_SADCRES_9BIT					(0x0000U)	///< 1 x 9-bit shunt sample
#define	INA219_CONFIG_SADCRES_10BIT					(0x0008U)	///< 1 x 10-bit shunt sample
#define	INA219_CONFIG_SADCRES_11BIT					(0x0010U)	///< 1 x 11-bit shunt sample
#define	INA219_CONFIG_SADCRES_12BIT					(0x0018U)	///< 1 x 12-bit shunt sample
#define	INA219_CONFIG_SADCRES_12BIT_2S				(0x0048U)	///< 2 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_4S				(0x0050U)	///< 4 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_8S				(0x0058U)	///< 8 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_16S				(0x0060U)	///< 16 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_32S				(0x0068U)	///< 32 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_64S				(0x0070U)	///< 64 x 12-bit shunt samples averaged together
#define	INA219_CONFIG_SADCRES_12BIT_128S			(0x0078U)	///< 128 x 12-bit shunt samples averaged together

#define INA219_CONFIG_MODE_MASK						(0x7U)
#define	INA219_CONFIG_MODE_POWERDOWN				(0x0U)	///< Power down
#define	INA219_CONFIG_MODE_SVOLT_TRIGGERED			(0x1U) 	///< Shunt voltage triggered
#define	INA219_CONFIG_MODE_BVOLT_TRIGGERED			(0x2U) 	///< Bus voltage triggered
#define	INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED		(0x3U) 	///< Shunt and bus voltage triggered
#define	INA219_CONFIG_MODE_ADCOFF					(0x4U) 	///< ADC off
#define	INA219_CONFIG_MODE_SVOLT_CONTINUOUS			(0x5U) 	///< Shunt voltage continuous
#define	INA219_CONFIG_MODE_BVOLT_CONTINUOUS			(0x6U) 	///< Bus voltage continuous
#define	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS 	(0x7U)	///< Shunt and bus voltage continuous


extern HAL_StatusTypeDef INA219_I2C_Status;	///< Stores the returned status of the last I2C communication


/**
* \brief Hold data on a INA219, must not be initialised or modified directly. Use INA219_Init() and the others dedicated functions
*/
typedef struct
{
	I2C_HandleTypeDef 	*ina219_i2c;	///< HAl I2C handler of the I2C peripheral connected to the INA219
	volatile uint8_t	Address;		///< Address of the INA219
	volatile double current_LSB;		///< Value of the LSB of the INA219 current register
} INA219_t;

/**
* \brief Initialise an INA219_t struct and reset the INA219.
*
* \param i2c: the I2C_HandleTypeDef of the I2C peripheral used.
* \param Adress: the I2C adress of the INA219.
* \param maxCurrent: the maximum expected current in A, has to be greater than 205 mA !
*
* \return 1 if the I2C interface is ready and the INA219 has been reset, 0 otherwise.
*/
uint8_t INA219_Init(INA219_t *ina219, I2C_HandleTypeDef *i2c, uint8_t Address, double maxCurrent);


/* === Read functions === */

/**
* \brief Read the raw value stored in the shunt boltage register.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the register value.
*/
uint16_t INA219_ReadRawShuntVolage(INA219_t *ina219);

/**
* \brief Read the shunt voltage.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the shunt voltage in mV.
*/
float INA219_ReadShuntVoltage(INA219_t *ina219);

/**
* \brief Read the raw value stored in the bus voltage register.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the register value.
*/
uint16_t INA219_ReadRawBusVoltage(INA219_t * ina219);

/**
* \brief Read the bus voltage.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the bus voltage in mV.
*/
uint16_t INA219_ReadBusVoltage(INA219_t *ina219);

/**
* \brief Read the raw value of the current register.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the register value.
*/
uint16_t INA219_ReadRawCurrent(INA219_t *ina219);

/**
* \brief Read the current.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the current in A.
*/
double INA219_ReadCurrent(INA219_t *ina219);

/**
* \brief Read the raw value of the power register.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the register value.
*/
uint16_t INA219_ReadRawPower(INA219_t * ina219);

/**
* \brief Read the power.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the power in W.
*/
double INA219_ReadPower(INA219_t * ina219);



/* === Configuration functions === */

/**
* \brief Reset the INA129.
*
* \param ina219: pointer to the INA219_t struct to be used.
*/
void INA219_Reset(INA219_t *ina219);

/**
* \brief Get the current configuration.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \return the configuration register value.
*/
uint16_t INA219_getConfig(INA219_t *ina219);

/**
* \brief Set the configuration register.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \param Config: the new uint16_t value of the configuration register.
*/
void INA219_setConfig(INA219_t *ina219, uint16_t Config);

/**
* \brief Set the calibration register.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \param Config: the new uint16_t value of the calibration register.
*/
void INA219_setCalibration(INA219_t *ina219, uint16_t CalibrationData);

/**
* \brief Set the conversion and power mode.
*
* \param ina219: pointer to the INA219_t struct to be used.
* \param Mode: the mode (only bits 0 to 2 have meaning).
*/
void INA219_setPowerMode(INA219_t *ina219, uint8_t Mode);

/* === Read/Write register I2C functions === */
// These are not specificcally intended to be used directly, prefer the use of the previous dedicated functions

uint16_t Read16(INA219_t *ina219, uint8_t Register);
void Write16(INA219_t *ina219, uint8_t Register, uint16_t Value);

#ifdef __cplusplus
}
#endif

#endif /* INC_INA219_H_ */
