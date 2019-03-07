/*
    __ecg4_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__ecg4_driver.h"
#include "__ecg4_hal.c"

/* ------------------------------------------------------------------- MACROS */



/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __ECG4_DRV_I2C__
static uint8_t _slaveAddress;
#endif

static T_ecg4_hdl  driver_hdl;
static T_ECG4_RETVAL  response_ready;

const uint8_t _ECG4_SYNC_BYTE                    = 0xAA;
const uint8_t _ECG4_EXCODE_BYTE                  = 0x55;
const uint8_t _ECG4_SIGNAL_QUALITY_CODE_BYTE     = 0x02;
const uint8_t _ECG4_HEART_RATE_CODE_BYTE         = 0x03;
const uint8_t _ECG4_RAW_DATA_CODE_BYTE           = 0x80;
const T_ECG4_RETVAL _ECG4_RESPONSE_READY         = 0x01;
const T_ECG4_RETVAL _ECG4_RESPONSE_NOT_READY     = 0x00;
const uint8_t _ECG4_ENABLE_LDO_CTRL              = 0x01;
const uint8_t _ECG4_DISABLE_LDO_CTRL             = 0x00;

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */



/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */



/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __ECG4_DRV_SPI__

void ecg4_spiDriverInit(T_ECG4_P gpioObj, T_ECG4_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __ECG4_DRV_I2C__

void ecg4_i2cDriverInit(T_ECG4_P gpioObj, T_ECG4_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __ECG4_DRV_UART__

void ecg4_uartDriverInit(T_ECG4_P gpioObj, T_ECG4_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    response_ready = 0;
}

#endif

/* ----------------------------------------------------------- IMPLEMENTATION */

void ecg4_response_handler_set( void ( *handler )( uint8_t*, uint8_t*, uint8_t*, uint8_t* ) )
{
    driver_hdl = handler;
}

void ecg4_uart_isr( void )
{
    static uint8_t rx_buff[ 256 ];
    static uint8_t rx_cnt = 0;
    static uint8_t rx_idx;
    static uint8_t payload_size;
    static uint8_t row_check = 0;
    static uint8_t op_code;
    static uint8_t row_size;
    static uint8_t row_size_check;
    static uint8_t checksum = 0;
    static uint8_t row_cnt = 0;
    uint8_t rx_dat;
    
    rx_dat = hal_uartRead();
    
    if ((rx_cnt == 0) && (rx_dat != _ECG4_SYNC_BYTE))
    {
        rx_cnt = 0;
        
        return;
    }
    else if ((rx_cnt == 1) && (rx_dat != _ECG4_SYNC_BYTE))
    {
        rx_cnt = 0;
        
        return;
    }
    else if (rx_cnt == 2)
    {
        payload_size = rx_dat;
        rx_cnt++;
        row_check = 1;
        
        return;
    }
    else if (rx_cnt > 2)
    {
        if (rx_cnt - 3 < payload_size)
        {
            if (rx_dat == _ECG4_EXCODE_BYTE)
            {
                row_check = 1;
                checksum += rx_dat;
                rx_cnt++;
                
                return;
            }
            
            if (row_check == 1)
            {
                op_code = rx_dat;
                row_check = 0;
                checksum += rx_dat;
                
                if ((op_code == _ECG4_SIGNAL_QUALITY_CODE_BYTE) || (op_code == _ECG4_HEART_RATE_CODE_BYTE))
                {
                    row_size = 1;
                    row_size_check = 0;
                }
                else if (op_code == _ECG4_RAW_DATA_CODE_BYTE)
                {
                    row_size_check = 1;
                }
                else
                {
                    rx_cnt = 0;
                    checksum = 0;
                    
                    return;
                }
                
                rx_idx = 0;
                rx_cnt++;
                
                return;
            }
            
            if (row_size_check == 1)
            {
                row_size = rx_dat;
                row_size_check = 0;
                checksum += rx_dat;
                rx_cnt++;
                
                return;
            }
            
            if (rx_idx < row_size)
            {
                rx_buff[ rx_idx ] = rx_dat;
                rx_idx++;
                checksum += rx_dat;
                
                if (rx_idx == row_size)
                {
                    driver_hdl( &op_code, &row_size, rx_buff, &row_cnt );
                    row_cnt++;
                    row_check = 1;
                }
                
                rx_cnt++;
                
                return;
            }
        }
        else
        {
            checksum = ~checksum;
            
            if (checksum == rx_dat)
            {
                response_ready = _ECG4_RESPONSE_READY;
            }
            else
            {
                response_ready = _ECG4_RESPONSE_NOT_READY;
            }
            
            checksum = 0;
            row_cnt = 0;
            rx_cnt = 0;
            rx_idx = 0;
            
            return;
        }
    }
    
    rx_cnt++;
}

T_ECG4_RETVAL ecg4_responseReady( void )
{
    if (response_ready)
    {
        response_ready = _ECG4_RESPONSE_NOT_READY;

        return _ECG4_RESPONSE_READY;
    }

    return _ECG4_RESPONSE_NOT_READY;
}

void ecg4_reset( void )
{
    hal_gpio_rstSet( 0 );
    Delay_100ms();
    Delay_100ms();
    hal_gpio_rstSet( 1 );
    Delay_100ms();
    Delay_100ms();
}

void ecg4_enable_LDO_ctrl( uint8_t state )
{
    if (state)
    {
        hal_gpio_csSet( 1 );
    }
    else
    {
        hal_gpio_csSet( 0 );
    }
}

/* -------------------------------------------------------------------------- */
/*
  __ecg4_driver.c

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