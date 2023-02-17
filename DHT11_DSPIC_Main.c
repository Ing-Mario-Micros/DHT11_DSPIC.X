/*
 * File:   DHT11_DSPIC_Main.c
 * Author: mario
 *
 * Created on 21 de agosto de 2022, 07:32 PM
 */

#include <xc.h>       //libreria de todsa las funciones del pic
#include "RS232.h"    //libreria para el uso de los puertos seriales
#include "DHT11.h"    //libreria para la lectura de sensores DHT11
//Fosc = 7.37MHz Por Defecto
#define FCY 5000000   //Definición de la frecuencia de bus del micro
#include <libpic30.h> //Libreria necesaria para el uso de retardos del DSPIC

// DSPIC30F4013 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = HS              // Oscillator (HS)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)
#pragma config BOREN = PBOR_OFF         // PBOR Enable (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define LED_CPU _RC13

/*------------------------- Función de Interrupción Timer 1 ----------------*/
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void);

void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void);
/*--------------------------- Variables DHT11 ---------------------------*/
extern unsigned char Temp1,Hum1;//Variables de sensor1
extern unsigned char Temp2,Hum2;//Variables de sensor2
extern unsigned char bandera; //Variable de time out DHT11


void main(void) {
    /*------------------ Configuración de Pines Digital --------------------*/
    TRISC=0; //Definición de todo el pueto C como salida
    LATC=0;  //Inicialización del pueto C en 0
    _LATC14 = 1;
    /*------------------ Configuración del Timer 1 -------------------------*/
    PR1=65535;
    TMR1=0;
    _T1IF=0;
    T1CON = 0x8010;
    /*------------------ Configuración de RS232 ---------------------------*/
    Activar_RS232();
    /*------------------ Configuracion DHT11 ------------------------------*/
    Activar_DHT11();/* En caso de usar alguna fución analoga del puerto B se
                     * requiere modificar esta función para el funcionamiento
                     * correcto del sistema */
    /*------------------ Configuración de Interrupciones -------------------*/
    /**** Interrupción Timer 1 ****/
    _T1IE = 1;  //Habilitación Interrupción Timer1
    _T1IP = 7;  //Definición de Prioridad del Timer1
    _T1IF = 0;  //Inicializar la bandera de interrupción en 0
    
    __delay_ms(1000);
    _LATC14 = 0;
    while(1){
        MensajeRS232(BufferR2);
        
        LeerHT11();
        __delay_ms(50);
        LeerHT11_2();
        
        MensajeRS232("DHT 11-3  = ");
        Transmitir('T');
        Transmitir(Temp1/10 + 48);
        Transmitir(Temp1%10 + 48);
        Transmitir(' ');
        Transmitir('H');
        Transmitir(Hum1/10 + 48);
        Transmitir(Hum1%10 + 48);
        Transmitir('\n');
        
        MensajeRS232("DHT 11-4 = ");
        Transmitir('T');
        Transmitir(Temp2/10 + 48);
        Transmitir(Temp2%10 + 48);
        Transmitir(' ');
        Transmitir('H');
        Transmitir(Hum2/10 + 48);
        Transmitir(Hum2%10 + 48);
        Transmitir('\n');
        __delay_ms(1000);
    }
}
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void){
    LED_CPU=LED_CPU^ 1; // Conmutar PinC13 LED CPU
    bandera=1;          // Variable de time out DHT11
    _T1IF=0;            // Reset de bandera de interrupción en Cero
}
void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void){
    Interrupcion_RS232();
}






