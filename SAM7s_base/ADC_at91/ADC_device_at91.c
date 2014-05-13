#include "ADC_device.h"
#include "board.h"

/* ADC field definition for the Mode Register: Reminder
                       TRGEN    => Selection bewteen Software or hardware start of conversion
                       TRGSEL   => Relevant if the previous field set a Hardware Triggering Mode
                       LOWRES   => 10-bit result if ths bit is cleared 0
                       SLEEP    => normal mode if ths is cleared
                       PRESCAL  => ADCclock = MCK / [(PRESCAL + 1)*2]
                       STARTUP  => Startup Time = [(STARTUP + 1)*8] / ADCclock
                       SHTIM    => Tracking time = (SHTIM + 1) / ADCclock
 */
#define   TRGEN    (0x0)    // Software triggering
#define   TRGSEL   (0x0)    // Without effect in Software triggering
#define   LOWRES   (0x0)    // 10-bit result output format
#define   SLEEP    (0x0)    // Normal Mode (instead of SLEEP Mode)
#define   PRESCAL  (0x4)    // Max value
#define   STARTUP  (0xc)    // This time period must be higher than 20 �s
#define   SHTIM    (0x3)    // Must be higher than 3 ADC clock cycles but depends on output
                            // impedance of the analog driver to the ADC input
/* Channel selection */
#define   CHANNEL  (0)      // Write the targeted channel (Notation: the first channel is 0
                            // and the last is 7)

int ADC_device_init(void){
	/* Clear all previous setting and result */
	AT91F_ADC_SoftReset (AT91C_BASE_ADC);

	/* First Step: Set up by using ADC Mode register */
	AT91F_ADC_CfgModeReg (AT91C_BASE_ADC,
					   (SHTIM << 24) | (STARTUP << 16) | (PRESCAL << 8) |
						(SLEEP << 5) | (LOWRES <<4) | (TRGSEL << 1) | (TRGEN )) ;

	/* Second Step: Select the active channel */
	AT91F_ADC_EnableChannel (AT91C_BASE_ADC, (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));

	return 1;
}

void ADC_device_sample_all(	unsigned int *a0,
						unsigned int *a1,
						unsigned int *a2,
						unsigned int *a3,
						unsigned int *a4,
						unsigned int *a5,
						unsigned int *a6,
						unsigned int *a7 ){

        /* Third Step: Start the conversion */
        AT91F_ADC_StartConversion (AT91C_BASE_ADC);

        /* Fourth Step: Waiting Stop Of conversion by polling */
        while (!((AT91F_ADC_GetStatus (AT91C_BASE_ADC)) & (1<<7)));

        *a0 = AT91F_ADC_GetConvertedDataCH0 (AT91C_BASE_ADC);
        *a1 = AT91F_ADC_GetConvertedDataCH1 (AT91C_BASE_ADC);
        *a2 = AT91F_ADC_GetConvertedDataCH2 (AT91C_BASE_ADC);
        *a3 = AT91F_ADC_GetConvertedDataCH3 (AT91C_BASE_ADC);
        *a4 = AT91F_ADC_GetConvertedDataCH4 (AT91C_BASE_ADC);
        *a5 = AT91F_ADC_GetConvertedDataCH5 (AT91C_BASE_ADC);
        *a6 = AT91F_ADC_GetConvertedDataCH6 (AT91C_BASE_ADC);
        *a7 = AT91F_ADC_GetConvertedDataCH7 (AT91C_BASE_ADC);
}

unsigned int ADC_device_sample(unsigned int channel){

       /* Clear all previous setting and result */
    //   AT91F_ADC_SoftReset (AT91C_BASE_ADC);

       /* First Step: Set up by using ADC Mode register */
     //  AT91F_ADC_CfgModeReg (AT91C_BASE_ADC,
       //                     (SHTIM << 24) | (STARTUP << 16) | (PRESCAL << 8) |
         //                   (SLEEP << 5) | (LOWRES <<4) | (TRGSEL << 1) | (TRGEN )) ;

        /* Second Step: Select the active channel */
 //       AT91F_ADC_EnableChannel (AT91C_BASE_ADC, (1<<channel));

        /* Third Step: Start the conversion */
        AT91F_ADC_StartConversion (AT91C_BASE_ADC);

        /* Fourth Step: Waiting Stop Of conversion by pulling */
        while (!((AT91F_ADC_GetStatus (AT91C_BASE_ADC)) & (1<<channel)));

        unsigned int result = 0;

        switch (channel){
        case 0:
        	result = AT91F_ADC_GetConvertedDataCH0 (AT91C_BASE_ADC);
        	break;
        case 1:
        	result = AT91F_ADC_GetConvertedDataCH1 (AT91C_BASE_ADC);
        	break;
        case 2:
        	result = AT91F_ADC_GetConvertedDataCH2 (AT91C_BASE_ADC);
        	break;
        case 3:
        	result = AT91F_ADC_GetConvertedDataCH3 (AT91C_BASE_ADC);
        	break;
        case 4:
        	result = AT91F_ADC_GetConvertedDataCH4 (AT91C_BASE_ADC);
        	break;
        case 5:
        	result = AT91F_ADC_GetConvertedDataCH5 (AT91C_BASE_ADC);
        	break;
        case 6:
        	result = AT91F_ADC_GetConvertedDataCH6 (AT91C_BASE_ADC);
        	break;
        case 7:
        	result = AT91F_ADC_GetConvertedDataCH7 (AT91C_BASE_ADC);
        	break;
        default:
        	break;
        }
	return result;
}

/* TODO delete me if not needed
unsigned int ReadADCx(unsigned int channel){

    unsigned int result ;

    AT91F_ADC_EnableChannel (AT91C_BASE_ADC, (1<<channel));
    AT91F_ADC_StartConversion (AT91C_BASE_ADC);

    //Poll for result
    while (!((AT91F_ADC_GetStatus (AT91C_BASE_ADC)) & (1<<channel)));

   	result = AT91F_ADC_GetLastConvertedData(AT91C_BASE_ADC);
   	AT91F_ADC_DisableChannel(AT91C_BASE_ADC, (1<<channel));

	return result;
}
*/