/*
    __ecg4_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __ecg4_driver.h
@brief    ECG_4 Driver
@mainpage ECG_4 Click
@{

@image html libstock_fb_view.jpg

@}

@defgroup   ECG4
@brief      ECG_4 Click Driver
@{

| Global Library Prefix | **ECG4** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **Dec 2018.**      |
| Developer             | **Nemanja Medakovic**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"

#ifndef _ECG4_H_
#define _ECG4_H_

/** 
 * @macro T_ECG4_P
 * @brief Driver Abstract type 
 */
#define T_ECG4_P    const uint8_t*
#define T_ECG4_RETVAL     uint8_t

/** @defgroup ECG4_COMPILE Compilation Config */              /** @{ */

//  #define   __ECG4_DRV_SPI__                            /**<     @macro __ECG4_DRV_SPI__  @brief SPI driver selector */
//  #define   __ECG4_DRV_I2C__                            /**<     @macro __ECG4_DRV_I2C__  @brief I2C driver selector */                                          
  #define   __ECG4_DRV_UART__                           /**<     @macro __ECG4_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup ECG4_VAR Variables */                           /** @{ */

extern const uint8_t _ECG4_SYNC_BYTE                    ;
extern const uint8_t _ECG4_EXCODE_BYTE                  ;
extern const uint8_t _ECG4_SIGNAL_QUALITY_CODE_BYTE     ;
extern const uint8_t _ECG4_HEART_RATE_CODE_BYTE         ;
extern const uint8_t _ECG4_RAW_DATA_CODE_BYTE           ;
extern const T_ECG4_RETVAL _ECG4_RESPONSE_READY         ;
extern const T_ECG4_RETVAL _ECG4_RESPONSE_NOT_READY     ;
extern const uint8_t _ECG4_ENABLE_LDO_CTRL              ;
extern const uint8_t _ECG4_DISABLE_LDO_CTRL             ;

                                                                       /** @} */
/** @defgroup ECG4_TYPES Types */                             /** @{ */

typedef void ( *T_ecg4_hdl )( uint8_t*, uint8_t*, uint8_t*, uint8_t* );

                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup ECG4_INIT Driver Initialization */              /** @{ */

#ifdef   __ECG4_DRV_SPI__
void ecg4_spiDriverInit(T_ECG4_P gpioObj, T_ECG4_P spiObj);
#endif
#ifdef   __ECG4_DRV_I2C__
void ecg4_i2cDriverInit(T_ECG4_P gpioObj, T_ECG4_P i2cObj, uint8_t slave);
#endif
#ifdef   __ECG4_DRV_UART__
void ecg4_uartDriverInit(T_ECG4_P gpioObj, T_ECG4_P uartObj);
#endif

                                                                       /** @} */
/** @defgroup ECG4_FUNC Driver Functions */                   /** @{ */

/**
 * @brief Set Handler function
 *
 * @param[in] handler  Pointer on the function that should be performed
 *
 * Function sets handler on the function that should be performed.
 */
void ecg4_response_handler_set( void ( *handler )( uint8_t*, uint8_t*, uint8_t*, uint8_t* ) );

/**
 * @brief Uart Interrupt Routine function
 *
 * Function performs the uart interrupt routine, reads a data from uart rx buffer and makes a response from the BMD101 device.
 */
void ecg4_uart_isr( void );

/**
 * @brief Response Ready Check function
 *
 * Function returns 1 when response is ready for reading, or 0 if is not ready.
 */
T_ECG4_RETVAL ecg4_responseReady( void );

/**
 * @brief Reset function
 *
 * Function performs a reset of the BMD101 module.
 */
void ecg4_reset( void );

/**
 * @brief LDO Control function
 *
 * @param[in] state  0 - Disable LDO control, 1 - Enable LDO control
 *
 * Function powers up or down control of LDO (Low Drop Out).
 * @note The BMD101 contains an internal LDO which consists of a bandgap cell to generate a 1.2V reference
 *       followed by two separate unity gain buffers, for the analog and digital supplies.
 */
void ecg4_enable_LDO_ctrl( uint8_t state );

                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_ECG_4_STM.c
    @example Click_ECG_4_TIVA.c
    @example Click_ECG_4_CEC.c
    @example Click_ECG_4_KINETIS.c
    @example Click_ECG_4_MSP.c
    @example Click_ECG_4_PIC.c
    @example Click_ECG_4_PIC32.c
    @example Click_ECG_4_DSPIC.c
    @example Click_ECG_4_AVR.c
    @example Click_ECG_4_FT90x.c
    @example Click_ECG_4_STM.mbas
    @example Click_ECG_4_TIVA.mbas
    @example Click_ECG_4_CEC.mbas
    @example Click_ECG_4_KINETIS.mbas
    @example Click_ECG_4_MSP.mbas
    @example Click_ECG_4_PIC.mbas
    @example Click_ECG_4_PIC32.mbas
    @example Click_ECG_4_DSPIC.mbas
    @example Click_ECG_4_AVR.mbas
    @example Click_ECG_4_FT90x.mbas
    @example Click_ECG_4_STM.mpas
    @example Click_ECG_4_TIVA.mpas
    @example Click_ECG_4_CEC.mpas
    @example Click_ECG_4_KINETIS.mpas
    @example Click_ECG_4_MSP.mpas
    @example Click_ECG_4_PIC.mpas
    @example Click_ECG_4_PIC32.mpas
    @example Click_ECG_4_DSPIC.mpas
    @example Click_ECG_4_AVR.mpas
    @example Click_ECG_4_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __ecg4_driver.h

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */