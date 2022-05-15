/*
 * File:   slave.c
 * Author: Josea
 *
 * Created on 11 de mayo de 2022, 08:50 PM
 */


/*
 * File:   Lab_presencial-_-.c
 * Author: Josea
 *
 * Created on 11 de mayo de 2022, 07:13 PM
 */


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000

/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t contador = 0, pot = 0;

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
   //SERIAL
    if(PIR1bits.SSPIF){             // ¿Recibió datos el esclavo?
        //pot = SSPBUF;             // Mostramos valor recibido en el PORTD
        SSPBUF = contador;
        PIR1bits.SSPIF = 0;         // Limpiamos bandera de interrupción
        
    }
    //Push Puerto B 
    if(INTCONbits.RBIF){                // Fue interrupci n del PORTB?
            
        if (!PORTBbits.RB0){
            contador++;              // Incremento del contador
            INTCONbits.RBIF = 0;
        }

        if (!PORTBbits.RB1){
            contador--;              // Decremento del contador
            INTCONbits.RBIF = 0; 
        }
     PORTA = contador;
    } 
    
    
    
}



/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    
    while (1){
        
    }
}
/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    TRISA = 0b10100000;         // SS, RA0 y RA1 como entradas
    PORTA = 0;
    
    
    TRISC = 0b00011000; // -> SDI y SCK entradas, SD0 como salida
    PORTC = 0;

    

   // SSPCON <5:0>
    SSPCONbits.SSPM = 0b0100;   // -> SPI Esclavo, SS hablitado
    SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
    SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
    // SSPSTAT<7:6>
    SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 0;        // -> Dato al final del pulso de reloj

    PIR1bits.SSPIF = 0;         // Limpiamos bandera de SPI
    PIE1bits.SSPIE = 1;         // Habilitamos int. de SPI
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    //Pullups RB0 RB1
    TRISBbits.TRISB0 = 1;       // RB0 como entrada (configurada con bits de control)
    TRISBbits.TRISB1 = 1;       // RB1 como entrada (configurada con bits de control)
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    INTCONbits.RBIE = 1;
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitamos resistencia de pull-up de RB1
    IOCBbits.IOCB0 = 1;         // Habilitamos interrupcion por cambio de estado ?para RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos interrupcion por cambio de estado ?para RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupcion

}   