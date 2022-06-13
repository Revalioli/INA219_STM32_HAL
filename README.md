# INA219_STM32_HAL

C library using the STM32 HAL driver to configure and communicate with the INA219 current sensor.

This library is based on the code of komuch at https://github.com/komuch/PSM_INA219_STM32.

## How to import in a project

1. Add both ina219.c/.h files in your project.
2. Include in both files the HAL header corresponding to your board (for example `stm32f4xx_hal.h`).
3. Include ina219.h in files where you want to use it.

## How to use the library

In order to properly use the library, you must have set up the I2C communication between the board and the INA219
using HAL functions and the I2C_HandleTypeDef.

### INA219_t struct

First you must create an `INA219_t` struct and initialise it with `INA219_Init()`.
This basic struct will then contain basic information to communication with the INA219 and correctly read its values.
**Warning !** The maximum current given to `INA219_Init()` must be greater than 205 mA !

### Configure the INA219

You can set the configuration register of the INA219 with the function `INA219_setConfig()`. You can give the configuration using a logical OR combination of the config `#define`.

`INA219_Reset()` will reset the INA219 to its default configuration :
- 32V bus voltage range
- Gain /8
- Bus and shunt voltage samples on 12-bits (1 sample)
- Shunt and bus voltage in continuous mode

### Read register values

This library provide read functions to get :
- the raw register values as they are stored using `INA219_ReadRaw___()`
- the measured values using `INA219_Read___()`
