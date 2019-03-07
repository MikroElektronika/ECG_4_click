/*
Example for ECG_4 Click

    Date          : Dec 2018.
    Author        : Nemanja Medakovic

Test configuration STM32 :
    
    MCU              : STM32F107VCT6
    Dev. Board       : EasyMx PRO v7 for STM32
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes UART interface, performs a reset of the device and powers up control of
  Low Drop Out (LDO).
- Application Task - (code snippet) - Always checks does response ready and, when is ready, processes a response.
Note : The Sound library is used to play sound on buzzer every time when R interval on the ECG signal happened.
The Sound Play command can make a delay time which is necessary for play command to be performed.
If you don't need to make sound on buzzer, please disable the Sound Init and Sound Play functions.

Additional Functions :

- interrupt - Always checks does new data ready for reading from uart rx buffer, and if is it true reads one byte data from uart
  and makes a response sequence.
- plotData - Sends a desired data (Raw Data) to the serial plotter.
- logData - Sends data to the uart terminal and shows results.
- makeResponse - Pars a response to the buffer.
- processResponse - Processes a parsed response and makes a decide where response will be showed.

*/

#include "Click_ECG_4_types.h"
#include "Click_ECG_4_config.h"

const uint8_t RX_UART_READY = 1;

uint8_t response[ 256 ];
uint8_t row_counter;
uint32_t plot_x;
uint8_t row_size_cnt;
T_ECG4_RETVAL resp_ready;

void plotData( int16_t plot_data )
{
    char plot_string[ 50 ];
    
    IntToStr( plot_data, plot_string );
    mikrobus_logWrite( plot_string, _LOG_TEXT );
    mikrobus_logWrite( ",", _LOG_TEXT );
    LongWordToStr( plot_x, plot_string );
    mikrobus_logWrite( plot_string, _LOG_LINE );
    
    if (plot_data > 4000)
    {
        Sound_Play( 520, 25 );
    }
    
    if (plot_x == 0xFFFFFFFF)
    {
        plot_x = 0;
    }
    else
    {
        plot_x++;
    }
    
    Delay_ms( 3 );
}

void logData( uint8_t code_val, uint8_t data_val )
{
    char log_string[ 20 ];

    if (code_val == _ECG4_SIGNAL_QUALITY_CODE_BYTE)
    {
        mikrobus_logWrite( "** Signal Quality (0 - sensor off, 200 - sensor on) : ", _LOG_TEXT );
        ByteToStr( data_val, log_string );
        mikrobus_logWrite( log_string, _LOG_TEXT );
        mikrobus_logWrite( " **", _LOG_LINE );
    }
    else if (code_val == _ECG4_HEART_RATE_CODE_BYTE)
    {
        mikrobus_logWrite( "** Real-time Heart Rate : ", _LOG_TEXT );
        ByteToStr( data_val, log_string );
        mikrobus_logWrite( log_string, _LOG_TEXT );
        mikrobus_logWrite( " BPM **", _LOG_LINE );
    }
}

void makeResponse( uint8_t *opCode, uint8_t *rowSize, uint8_t *rxBuff, uint8_t *rowCnt )
{
    uint8_t idx_cnt;
    
    if (*rowCnt == 0)
    {
        row_size_cnt = 0;
    }
    
    response[ row_size_cnt ] = *opCode;
    response[ row_size_cnt + 1 ] = *rowSize;

    for (idx_cnt = 0; idx_cnt < *rowSize; idx_cnt++)
    {
        response[ row_size_cnt + 2 + idx_cnt ] = rxBuff[ idx_cnt ];
    }
    
    row_size_cnt += (*rowSize + 2);
    row_counter = *rowCnt;
}

void processResponse()
{
    uint8_t cnt;
    uint8_t idx_cnt;
    int16_t raw_data;
    
    idx_cnt = 0;
    
    for (cnt = 0; cnt <= row_counter; cnt++)
    {
        if (response[ idx_cnt ] == _ECG4_RAW_DATA_CODE_BYTE)
        {
            raw_data = response[ idx_cnt + 2 ];
            raw_data <<= 8;
            raw_data |= response[ idx_cnt + 3 ];
            
            plotData( raw_data );
        }
        
        /*if ((response[ idx_cnt ] == _ECG4_SIGNAL_QUALITY_CODE_BYTE) || (response[ idx_cnt ] == _ECG4_HEART_RATE_CODE_BYTE))
        {
            logData( response[ idx_cnt ], response[ idx_cnt + 2 ] );
        }*/
        
        idx_cnt += (response[ idx_cnt + 1 ] + 2);
    }
}

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );

    mikrobus_uartInit( _MIKROBUS1, &_ECG4_UART_CFG[0] );

    mikrobus_logInit( _LOG_USBUART_A, 57600 );
    mikrobus_logWrite( "*** Initializing... ***", _LOG_LINE );

    Delay_ms( 100 );
}

void applicationInit()
{
    ecg4_uartDriverInit( (T_ECG4_P)&_MIKROBUS1_GPIO, (T_ECG4_P)&_MIKROBUS1_UART );
    ecg4_response_handler_set( &makeResponse );
    Delay_ms( 300 );
    
    ecg4_reset();
    ecg4_enable_LDO_ctrl( _ECG4_ENABLE_LDO_CTRL );
    Sound_Init( &GPIOE_ODR, 14 );
    Delay_ms( 200 );
	
	plot_x = 0;

    RXNEIE_USART3_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART3 );
    EnableInterrupts();
    
    mikrobus_logWrite( "** ECG 4 is initialized **", _LOG_LINE );
    mikrobus_logWrite( "", _LOG_LINE );
}

void applicationTask()
{
    resp_ready = ecg4_responseReady();
    
    if (resp_ready == _ECG4_RESPONSE_READY)
    {
        processResponse();
    }
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}

void RX_ISR() iv IVT_INT_USART3 ics ICS_AUTO
{
    if ( RXNE_USART3_SR_bit == RX_UART_READY )
    {
        ecg4_uart_isr();
    }
}
