/* 
 * File:   DHT11.h
 * Author: mario
 *
 * Created on 17 de febrero de 2023, 08:58 AM
 */

#ifndef DHT11_H
#define	DHT11_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif
/**----------------------- Librerias del dspic -----------------------------**/
#include <xc.h>   //libreria de todsa las funciones del pic
#define FCY 5000000 //Definición de la frecuencia de bus del micro
#include <libpic30.h>//Libreria necesaria para el uso de retardos del DSPIC
/**---------------- definición de los pines del sensor ---------------------**/
#define DATA_DIR _TRISB11 //Definición del pin de señal del sensor1
#define DATA_IN _RB11     //Definición de Entrada y salida de sensor1
#define DATA_OUT _LATB11  //Definición de salida para sensor1

#define DATA_DIR2 _TRISB12//Definición del pin de señal del sensor2
#define DATA_IN2 _RB12    //Definición de Entrada y salida de sensor2
#define DATA_OUT2 _LATB12 //Definición de salida para sensor2
/**------------ Declaración de las variables para leer el sensor -----------**/
unsigned char Temp1=10,Hum1=20; // Variables de sensor1
unsigned char Che,bandera = 0;  // Variables de funcionamiento general
unsigned char Temp2=10,Hum2=20; // Variables de sensor2
/**---------------------- declaración  de Funciones ------------------------**/
/** ----------------- Configuración DHT11 -------------------**/
void Activar_DHT11 (void);
/** ------------------------ DHT11-1 ------------------------**/
void LeerHT11(void);
unsigned char LeerByte(void);
unsigned char LeerBit(void);
/**------------------------- DHT11-2 ------------------------**/
void LeerHT11_2(void);
unsigned char LeerByte_2(void);
unsigned char LeerBit_2(void);
/*------------------------ Funcion DHT11 ---------------------*/
unsigned char Check();
/**---------------------------------------------------------------------**/

/*------------------------- Configuración DHT11 --------------------------*/
void Activar_DHT11 (void){
    ADPCFG=0xFFFF;    // Del Analogo 8 al 15 como Pin Digital
    DATA_OUT=0;       // Inicializar pin de salida en cero
    DATA_DIR=1;       // Definir el puerto como Entrada
    
    DATA_OUT2=0;       // Inicializar pin de salida en cero
    DATA_DIR2=1;       // Definir el puerto como Entrada
}

/* -------------------------Funciones DHT11-1------------------------------ */
void LeerHT11(void){
  unsigned char i,contr=0;
  unsigned char repetir=0;
  unsigned char contador=0;
  bandera=0;
  TMR1=0;
  do{
      /*------------- Condición De Start Inicio -----------------*/
    DATA_DIR=0;
    DATA_OUT=0;
    __delay_ms(18);
    DATA_DIR=1;
    while(DATA_IN==1){
      if(bandera==1) break;  
    }
    __delay_us(40);
    if(DATA_IN==0) contr++;
    __delay_us(80);
    if(DATA_IN==1) contr++;
    //__delay_us(120);
    while(DATA_IN==1){
      if(bandera==1) break;  
    }
     /*---------------- Condición de Start Final ----------------*/
    Hum1=LeerByte();
    LeerByte();
    Temp1=LeerByte();
    LeerByte();
    Che=LeerByte();
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
unsigned char LeerByte(void){
  unsigned char res=0,i;
  
  for(i=8;i>0;i--){
    res=(res<<1) | LeerBit();  
  }
  return res;
}
unsigned char LeerBit(void){
    unsigned char res=0;
     while(DATA_IN==0){
       if(bandera==1) break;  
     }
     __delay_us(13);
     if(DATA_IN==1) res=0;
     __delay_us(22);
     if(DATA_IN==1){
       res=1;
       while(DATA_IN==1){
         if(bandera==1) break;  
       }
     }  
     return res;  
}
unsigned char Check(void){
  unsigned char res=0,aux;
  aux=Temp1+Hum1;
  if(aux==Che) res=1;
  return res;  
}
/**-------------------------------------------------------------------------**/
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
/**-------------------------------------------------------------------------**/
#endif	/* DHT11_H */

