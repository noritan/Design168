/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

uint8   rx8Buf[9]; // Data packet to be sent
uint16  x1; // The first data
uint16  x2; // The second data

int main(void) {
    CyGlobalIntEnable; // Enable interrupt

    USBUART_Start(0, USBUART_5V_OPERATION); // Initialize USB
    
    // Initialize DAC
    WaveDAC8_Start();
    
    // Initialize ADC
    AMux_Start();
    ADC_Start();

    for (;;) {
        // Wait for USB configuration
        while (USBUART_GetConfiguration() == 0);
        
        USBUART_IsConfigurationChanged(); // Ensure the CHANGE flag cleared
        
        USBUART_CDC_Init(); // Initialize as CDC device

        // Initialize AMux
        AMux_Init();
        
        for (;;) {
            // Get ADC value from CH1
            AMux_Next();
            ADC_StartConvert();
            while (!ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)) ;
            x1 = ADC_GetResult16();
            
            // Get ADC value from CH2
            AMux_Next();
            ADC_StartConvert();
            while (!ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)) ;
            x2 = ADC_GetResult16();
                        
            // Prepare a data packet
            rx8Buf[0]  = 0x0D;
            rx8Buf[1]  = 0x0A;
            rx8Buf[2]  = HI8(x1);
            rx8Buf[3]  = LO8(x1);
            rx8Buf[4]  = HI8(x2);
            rx8Buf[5]  = LO8(x2);
            rx8Buf[6]  = 0x00;
            rx8Buf[7]  = 0xFF;
            rx8Buf[8]  = 0xFF;
            
            // Re-initialize when configuration changed
            if (USBUART_IsConfigurationChanged()) {
                goto reconfigure;
            }

            // Send data to host
            while (!USBUART_CDCIsReady()) {
                if (USBUART_IsConfigurationChanged()) {
                    goto reconfigure;
                }
            }
            USBUART_PutData(rx8Buf, sizeof(rx8Buf));
        }
        reconfigure: ;
    }
}

/* [] END OF FILE */
