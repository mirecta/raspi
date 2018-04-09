#include <stdio.h>
#include <wiringPi.h>
 
#define Button 6        // Pin 22 Orange Pi Zero
#define EncoderA 10 // Pin 24 Orange Pi Zero
#define EncoderB 11 // Pin 26 Orange Pi Zero
 
int Value=50;
int right=0;
int left=0;
 
void ButtonInterrupt (void) 
{ 
printf("Button\n");
delay (500) ;
}
 
void EncoderAInterrupt (void) 
{ 
 right=digitalRead(EncoderB);  //  A=0 und B=1 Erste Impuls bei Rechtslauf
 if(!right && left)       //  A=0 und B=0 Zweite Impuls bei Linkslauf
    {
    Value--;
    printf("Value %i \n", Value);
    }
 left=0;                       // Zustand vom Ersten Impuls loeschen 
}
 
void EncoderBInterrupt (void) 
{ 
left= digitalRead(EncoderA);   //  B=0 und A=1 Erste Impuls bei Linkslauf
if(!left && right)        //  B=0 und A=0 Zweite Impuls bei Rechtslauf
    {
     Value++;
     printf("Value %i \n", Value);
    }
 right=0;                  // Zustand vom Ersten Impuls loeschen
}
 
int main (void)
{
  wiringPiSetup () ;
  pinMode (Button, INPUT);          // PIN als Ausgang konfigurieren
 // pullUpDnControl (Taster, PUD_UP);     // PIN  mit Pul-Upp versehen
  pinMode (EncoderA, INPUT);            // PIN als Ausgang konfigurieren
 // pullUpDnControl (EncoderA, PUD_UP);   // PIN  mit Pul-Upp versehen
  pinMode (EncoderB, INPUT);            // PIN als Ausgang konfigurieren
 // pullUpDnControl (EncoderB, PUD_UP);   // PIN  mit Pul-Upp versehen
 
  wiringPiISR (Button, INT_EDGE_FALLING, &ButtonInterrupt) ;    // Interrupt Routine definieren
  wiringPiISR (EncoderA, INT_EDGE_FALLING, &EncoderAInterrupt); // Interrupt Routine definieren
  wiringPiISR (EncoderB, INT_EDGE_FALLING, &EncoderBInterrupt); // Interrupt Routine definieren
  printf("Value %i \n", Value);
  while(1) delay (5000);                                // Endlosschleife
  return 0 ;
}
