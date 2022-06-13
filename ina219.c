/**
 * @file INA219.c
 *
 * Based on the github repository : https://github.com/komuch/PSM_INA219_STM32
 */

#include "ina219.h"

/* === Include here the HAL header corresponding to your board === */
#include "stm32f4xx_hal.h"

#include <math.h>

HAL_StatusTypeDef INA219_I2C_Status;


uint8_t INA219_Init(INA219_t *ina219, I2C_HandleTypeDef *i2c, uint8_t Address, double maxCurrent)
{
	ina219->ina219_i2c = i2c;
	ina219->Address = Address;

	HAL_StatusTypeDef ina219_isReady = HAL_I2C_IsDeviceReady(i2c, (Address << 1), 3, 2);

	if(ina219_isReady == HAL_OK)
	{
		INA219_Reset(ina219);	// Reset the INA219

		ina219->current_LSB = maxCurrent/pow(2.0, 15.0);

		double resistor = ((double)SHUNT_RESISTOR)/1000.0;
		// Using datasheet formula
		uint16_t cal =  (uint16_t) floor(0.04096/(ina219->current_LSB * resistor));

		INA219_setCalibration(ina219, cal);

		return 1;
	}

	return 0;
}


/* === Read functions === */

uint16_t INA219_ReadRawShuntVolage(INA219_t *ina219)
{
	return Read16(ina219, INA219_REG_SHUNTVOLTAGE);
}

float INA219_ReadShuntVoltage(INA219_t *ina219)
{
	uint16_t result = INA219_ReadRawShuntVolage(ina219);
	return ((float)result) * 0.01;	// Conversion in milli-volts
}

uint16_t INA219_ReadRawBusVoltage(INA219_t * ina219)
{
	return Read16(ina219, INA219_REG_BUSVOLTAGE);
}

uint16_t INA219_ReadBusVoltage(INA219_t *ina219)
{
	uint16_t result = Read16(ina219, INA219_REG_BUSVOLTAGE);
	return ((result >> 3  ) * 4);
}

uint16_t INA219_ReadRawCurrent(INA219_t *ina219)
{
	return Read16(ina219, INA219_REG_CURRENT);
}

double INA219_ReadCurrent(INA219_t *ina219)
{
	int16_t result = INA219_ReadRawCurrent(ina219);

	return ina219->current_LSB * result;
}

uint16_t INA219_ReadRawPower(INA219_t * ina219)
{
	return Read16(ina219, INA219_REG_POWER);
}

double INA219_ReadPower(INA219_t * ina219)
{
	return INA219_ReadRawPower(ina219) * ina219->current_LSB * 20.0;
}


/* === Configuration functions === */

void INA219_Reset(INA219_t *ina219)
{
	Write16(ina219, INA219_REG_CONFIG, INA219_CONFIG_RESET);
}

uint16_t INA219_getConfig(INA219_t *ina219)
{
	uint16_t result = Read16(ina219, INA219_REG_CONFIG);
	return result;
}

void INA219_setConfig(INA219_t *ina219, uint16_t Config)
{
	Write16(ina219, INA219_REG_CONFIG, Config);
}

void INA219_setCalibration(INA219_t *ina219, uint16_t CalibrationData)
{
	Write16(ina219, INA219_REG_CALIBRATION, CalibrationData);
}

void INA219_setPowerMode(INA219_t *ina219, uint8_t Mode)
{
	uint16_t config = INA219_getConfig(ina219);

	config = (config & ~INA219_CONFIG_MODE_MASK) | (Mode & INA219_CONFIG_MODE_MASK);
	INA219_setConfig(ina219, config);
}


/* === Read/Write register I2C functions === */

uint16_t Read16(INA219_t *ina219, uint8_t Register)
{
	uint8_t Value[2];

	INA219_I2C_Status = HAL_I2C_Mem_Read(ina219->ina219_i2c, (ina219->Address << 1), Register, 1, Value, 2, HAL_MAX_DELAY);

	return ((Value[0] << 8) | Value[1]);
}


void Write16(INA219_t *ina219, uint8_t Register, uint16_t Value)
{
	uint8_t addr[2];
	addr[0] = (Value >> 8) & 0xff;  // MSByte
	addr[1] = (Value >> 0) & 0xff; // LSByte
	INA219_I2C_Status = HAL_I2C_Mem_Write(ina219->ina219_i2c, (ina219->Address << 1), Register, 1, (uint8_t*)addr, 2, HAL_MAX_DELAY);
}
