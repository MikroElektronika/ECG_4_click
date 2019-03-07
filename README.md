![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# ECG_4 Click

- **CIC Prefix**  : ECG4
- **Author**      : Nemanja Medakovic
- **Verison**     : 1.0.0
- **Date**        : Dec 2018.

---

### Software Support

We provide a library for the ECG_4 Click on our [LibStock](https://libstock.mikroe.com/projects/view/2712/ecg-4-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

The library performs the ECG and Real-time Heart Rate measurements.
For more details check documentation.

Key functions :

- ``` void ecg4_response_handler_set( void ( *handler )( uint8_t*, uint8_t*, uint8_t*, uint8_t* ) ) ``` - Function sets handler on the function that should be performed, 
  for example, function for results logging.
- ``` void ecg4_uart_isr( void ) ``` - Function performs the uart interrupt routine, reads a data from uart rx buffer and makes a response from the BMD101 device.
- ``` T_ECG4_RETVAL ecg4_responseReady( void ) ``` - Function returns 1 when response is ready for reading, or 0 if is not ready.

**Examples Description**

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes UART interface, performs a reset of the device and powers up control of
  Low Drop Out (LDO).
- Application Task - (code snippet) - Always checks does response ready and, when is ready, processes a response.
Note : The Sound library is used to play sound on buzzer every time when R interval on the ECG signal happened.
The Sound Play command can make a delay time which is necessary for play command to be performed.
If you don't need to make sound on buzzer, please disable the Sound Init and Sound Play functions.


```.c
void applicationTask()
{
    resp_ready = ecg4_responseReady();
    
    if (resp_ready == _ECG4_RESPONSE_READY)
    {
        processResponse();
    }
}
```

Additional Functions :

- interrupt - Always checks does new data ready for reading from uart rx buffer, and if is it true reads one byte data from uart
  and makes a response sequence.
- plotData - Sends a desired data (Raw Data) to the serial plotter.
- logData - Sends data to the uart terminal and shows results.
- makeResponse - Pars a response to the buffer.
- processResponse - Processes a parsed response and makes a decide where response will be showed.

The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/2712/ecg-4-click) page.

Other mikroE Libraries used in the example:

- Conversions
- Sound
- UART

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
