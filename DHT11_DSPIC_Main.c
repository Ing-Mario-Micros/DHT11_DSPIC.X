/*
 * File:   DHT11_DSPIC_Main.c
 * Author: mario
 *
 * Created on 25 de agosto de 2022, 07:32 PM
 */

#include <xc.h>
#include "RS232.h"
#include "DHT11.h"
//Fosc = 7.37MHz Por Defecto
#define FCY 5000000
#include <libpic30.h>

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

// Pines del DHT


#define DATA_DIR2 _TRISB12//Definición del pin de señal del sensor2
#define DATA_IN2 _RB12    //Definición de Entrada y salida de sensor2
#define DATA_OUT2 _LATB12 //Definición de salida para sensor2

/*------------------------- Función de Interrupción Timer 1 ----------------*/
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void);

void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void);
/*--------------------------- Variables DHT11 ---------------------------*/
extern unsigned char Temp1,Hum1;

unsigned char Temp2=10,Hum2=20; //

/*--------------------------- Funciones DHT11-2 ---------------------------*/
void LeerHT11_2(void);
unsigned char LeerByte_2(void);
unsigned char LeerBit_2(void);
/*--------------------------- Funciones DHT11-2 ---------------------------*/
unsigned char Check();

void main(void) {
    /*------------------ Configuración de Pines Digital --------------------*/
    TRISC=0;
    LATC=0;
    _LATD9 = 1;
    /*------------------ Configuración del Timer 1 -------------------------*/
    PR1=65535;
    TMR1=0;
    _T1IF=0;
    T1CON = 0x8010;
    /*------------------ Configuración de RS232 ---------------------------*/
    Activar_RS232();
    /*------------------ Configuracion DHT11 ------------------------------*/
    ADPCFG=0xFFFF;    // Del Analogo 8 al 15 como Pin Digital
    DATA_OUT=0;       // Inicializar pin de salida en cero
    DATA_DIR=1;       // Definir el puerto como Entrada
    
    DATA_OUT2=0;       // Inicializar pin de salida en cero
    DATA_DIR2=1;       // Definir el puerto como Entrada
    /*------------------ Configuración de Interrupciones -------------------*/
    /**** Interrupción Timer 1 ****/
    _T1IE = 1;  //Habilitación Interrupción Timer1
    _T1IP = 7;  //Definición de Prioridad del Timer1
    _T1IF = 0;  //Inicializar la bandera de interrupción en 0
    
    __delay_ms(1000);
    _LATD9 = 0;
    while(1){
        //AUX=U2RXREG;
        //_LATD9 = 1;
        MensajeRS232("DHT 11-3  = ");
        /*if(DATA_IN==1){
            MensajeRS232("On\n");
        }
        else{
            MensajeRS232("Off\n");
        }/****/
        MensajeRS232(BufferR2);
        LeerHT11();
        Transmitir('T');
        Transmitir(Temp1/10 + 48);
        Transmitir(Temp1%10 + 48);
        Transmitir(' ');
        Transmitir('H');
        Transmitir(Hum1/10 + 48);
        Transmitir(Hum1%10 + 48);
        Transmitir('\n');
        __delay_ms(50);
        MensajeRS232("DHT 11-4 = ");
        LeerHT11_2();
        Transmitir('T');
        Transmitir(Temp2/10 + 48);
        Transmitir(Temp2%10 + 48);
        Transmitir(' ');
        Transmitir('H');
        Transmitir(Hum2/10 + 48);
        Transmitir(Hum2%10 + 48);
        Transmitir('\n');
        //_LATD9 = 0;
        //Transmitir(AUX);
        __delay_ms(1000);
    }
}
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void){
    LED_CPU=LED_CPU^ 1; // Conmutar PinC13 LED CPU
    bandera=1;
    _T1IF=0;            // Reset de bandera de interrupción en Cero
}
void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void){
    Interrupcion_RS232();
}


/* -------------------------Funciones DHT11-2 ------------------------------ */
void LeerHT11_2(void){
  unsigned char i,contr=0;
  unsigned char repetir=0;
  unsigned char contador=0;
  bandera=0;
  TMR1=0;
  do{
      /*------------- Condición De Start Inicio -----------------*/
    DATA_DIR2=0;
    DATA_OUT2=0;
    __delay_ms(18);
    DATA_DIR2=1;
    while(DATA_IN2==1){
      if(bandera==1) break;  
    }
    __delay_us(40);
    if(DATA_IN2==0) contr++;
    __delay_us(80);
    if(DATA_IN2==1) contr++;
    //__delay_us(120);
    while(DATA_IN2==1){
      if(bandera==1) break;  
    }
     /*---------------- Condición de Start Final ----------------*/
    Hum2=LeerByte_2();
    LeerByte_2();
    Temp2=LeerByte_2();
    LeerByte_2();
    Che=LeerByte_2();
    if(bandera==1){
      repetir=1;
      bandera=0;
      contador++;
      TMR1=0;
    }
  }while(repetir==1 && contador<6);
  
  repetir=0;
  if(contador==6){
    Temp1=0;
    Hum1=0;
    contador=0;
  }
}

unsigned char LeerByte_2(void){
  unsigned char res=0,i;
  
  for(i=8;i>0;i--){
    res=(res<<1) | LeerBit_2();  
  }
  return res;
}
unsigned char LeerBit_2(void){
    unsigned char res=0;
     while(DATA_IN2==0){
       if(bandera==1) break;  
     }
     __delay_us(13);
     if(DATA_IN2==1) res=0;
     __delay_us(22);
     if(DATA_IN2==1){
       res=1;
       while(DATA_IN2==1){
         if(bandera==1) break;  
       }
     }  
     return res;  
}




