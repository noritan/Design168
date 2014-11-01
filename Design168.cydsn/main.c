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

int main(void) {
    CyGlobalIntEnable; // Enable interrupt

    USBUART_Start(0, USBUART_5V_OPERATION); // Initialize USB

    for (;;) {
        // Wait for USB configuration
        while (USBUART_GetConfiguration() == 0);
        
        USBUART_IsConfigurationChanged(); // Ensure the CHANGE flag cleared
        
        USBUART_CDC_Init(); // Initialize as CDC device

        for (;;) {
            // Re-initialize when configuration changed
            if (USBUART_IsConfigurationChanged()) {
                break;
            }
        }
    }
}

/* [] END OF FILE */
