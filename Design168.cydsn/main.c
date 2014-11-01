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

#define     TABLE_LENGTH        (64)

uint8   rx8Buf[9]; // Data packet to be sent
uint16  x1; // The first data
uint16  x2; // The second data

static const uint16 sine[TABLE_LENGTH] = {
32768,	34373,	35963,	37523,	39037,	40490,	41869,	43160,
44351,	45431,	46389,	47215,	47902,	48444,	48835,	49071,
49150,	49071,	48835,	48444,	47902,	47215,	46389,	45431,
44351,	43160,	41869,	40490,	39037,	37523,	35963,	34373,
32768,	31162,	29572,	28012,	26498,	25045,	23666,	22375,
21184,	20104,	19146,	18320,	17633,	17091,	16700,	16464,
16386,	16464,	16700,	17091,	17633,	18320,	19146,	20104,
21184,	22375,	23666,	25045,	26498,	28012,	29572,	31162,
};

int main(void) {
    uint32      index1;
    uint32      index2;
    
    CyGlobalIntEnable; // Enable interrupt

    USBUART_Start(0, USBUART_5V_OPERATION); // Initialize USB

    for (;;) {
        // Wait for USB configuration
        while (USBUART_GetConfiguration() == 0);
        
        USBUART_IsConfigurationChanged(); // Ensure the CHANGE flag cleared
        
        USBUART_CDC_Init(); // Initialize as CDC device

        // Initialize index
        index1 = 0;  // 0 degree
        index2 = TABLE_LENGTH / 4; // 90 degree
        
        for (;;) {
            // Get data
            x1 = sine[index1];
            x2 = sine[index2];
            index1 = (index1 + 1) % TABLE_LENGTH;
            index2 = (index2 + 1) % TABLE_LENGTH;
            
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
