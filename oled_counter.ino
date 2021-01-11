/*
 * oled Frequency Counter
 *
 * Created: 10/01/2021
 *  Author: moty22.co.uk
 */ 
#include <Wire.h>
//#include "oled_font.c"

unsigned char addr=0x3C;  //0x78

// standard ascii 5x7 font 
//  0x3E, 0x51, 0x49, 0x45, 0x3E,  //crossed 0
static const unsigned char  font[] = {

  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x5F, 0x00, 0x00, 
  0x00, 0x07, 0x00, 0x07, 0x00, 
  0x14, 0x7F, 0x14, 0x7F, 0x14, 
  0x24, 0x2A, 0x7F, 0x2A, 0x12, 
  0x23, 0x13, 0x08, 0x64, 0x62, 
  0x36, 0x49, 0x56, 0x20, 0x50, 
  0x00, 0x08, 0x07, 0x03, 0x00, 
  0x00, 0x1C, 0x22, 0x41, 0x00, 
  0x00, 0x41, 0x22, 0x1C, 0x00, 
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 
  0x08, 0x08, 0x3E, 0x08, 0x08, 
  0x00, 0x80, 0x70, 0x30, 0x00, 
  0x08, 0x08, 0x08, 0x08, 0x08, 
  0x00, 0x00, 0x60, 0x60, 0x00, 
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x41, 0x41, 0x41, 0x3E,   //un-crossed 0
  0x00, 0x42, 0x7F, 0x40, 0x00,
  0x72, 0x49, 0x49, 0x49, 0x46, 
  0x21, 0x41, 0x49, 0x4D, 0x33, 
  0x18, 0x14, 0x12, 0x7F, 0x10, 
  0x27, 0x45, 0x45, 0x45, 0x39, 
  0x3C, 0x4A, 0x49, 0x49, 0x31, 
  0x41, 0x21, 0x11, 0x09, 0x07, 
  0x36, 0x49, 0x49, 0x49, 0x36, 
  0x46, 0x49, 0x49, 0x29, 0x1E, 
  0x00, 0x00, 0x14, 0x00, 0x00, 
  0x00, 0x40, 0x34, 0x00, 0x00, 
  0x00, 0x08, 0x14, 0x22, 0x41, 
  0x14, 0x14, 0x14, 0x14, 0x14, 
  0x00, 0x41, 0x22, 0x14, 0x08, 
  0x02, 0x01, 0x59, 0x09, 0x06, 
  0x3E, 0x41, 0x5D, 0x59, 0x4E,
  0x7C, 0x12, 0x11, 0x12, 0x7C,    //A
  0x7F, 0x49, 0x49, 0x49, 0x36, 
  0x3E, 0x41, 0x41, 0x41, 0x22, 
  0x7F, 0x41, 0x41, 0x41, 0x3E, 
  0x7F, 0x49, 0x49, 0x49, 0x41, 
  0x7F, 0x09, 0x09, 0x09, 0x01, 
  0x3E, 0x41, 0x41, 0x51, 0x73, 
  0x7F, 0x08, 0x08, 0x08, 0x7F,   //H
  0x00, 0x41, 0x7F, 0x41, 0x00, 
  0x20, 0x40, 0x41, 0x3F, 0x01, 
  0x7F, 0x08, 0x14, 0x22, 0x41, 
  0x7F, 0x40, 0x40, 0x40, 0x40, 
  0x7F, 0x02, 0x1C, 0x02, 0x7F, 
  0x7F, 0x04, 0x08, 0x10, 0x7F, 
  0x3E, 0x41, 0x41, 0x41, 0x3E, 
  0x7F, 0x09, 0x09, 0x09, 0x06, 
  0x3E, 0x41, 0x51, 0x21, 0x5E, 
  0x7F, 0x09, 0x19, 0x29, 0x46, 
  0x26, 0x49, 0x49, 0x49, 0x32,   //S 
  0x03, 0x01, 0x7F, 0x01, 0x03, 
  0x3F, 0x40, 0x40, 0x40, 0x3F, 
  0x1F, 0x20, 0x40, 0x20, 0x1F, 
  0x3F, 0x40, 0x38, 0x40, 0x3F, 
  0x63, 0x14, 0x08, 0x14, 0x63, 
  0x03, 0x04, 0x78, 0x04, 0x03, 
  0x61, 0x59, 0x49, 0x4D, 0x43, 
  0x00, 0x7F, 0x41, 0x41, 0x41, 
  0x02, 0x04, 0x08, 0x10, 0x20, 
  0x00, 0x41, 0x41, 0x41, 0x7F, 
  0x04, 0x02, 0x01, 0x02, 0x04, 
  0x40, 0x40, 0x40, 0x40, 0x40, 
  0x00, 0x03, 0x07, 0x08, 0x00, 
  0x20, 0x54, 0x54, 0x78, 0x40, 
  0x7F, 0x28, 0x44, 0x44, 0x38, 
  0x38, 0x44, 0x44, 0x44, 0x28, 
  0x38, 0x44, 0x44, 0x28, 0x7F, 
  0x38, 0x54, 0x54, 0x54, 0x18, 
  0x00, 0x08, 0x7E, 0x09, 0x02, 
  0x18, 0xA4, 0xA4, 0x9C, 0x78, 
  0x7F, 0x08, 0x04, 0x04, 0x78, 
  0x00, 0x44, 0x7D, 0x40, 0x00, 
  0x20, 0x40, 0x40, 0x3D, 0x00, 
  0x7F, 0x10, 0x28, 0x44, 0x00, 
  0x00, 0x41, 0x7F, 0x40, 0x00, 
  0x7C, 0x04, 0x78, 0x04, 0x78, 
  0x7C, 0x08, 0x04, 0x04, 0x78,   //n
  0x38, 0x44, 0x44, 0x44, 0x38, 
  0xFC, 0x18, 0x24, 0x24, 0x18, 
  0x18, 0x24, 0x24, 0x18, 0xFC, 
  0x7C, 0x08, 0x04, 0x04, 0x08, 
  0x48, 0x54, 0x54, 0x54, 0x24, 
  0x04, 0x04, 0x3F, 0x44, 0x24, 
  0x3C, 0x40, 0x40, 0x20, 0x7C,   //u
  0x1C, 0x20, 0x40, 0x20, 0x1C, 
  0x3C, 0x40, 0x30, 0x40, 0x3C, 
  0x44, 0x28, 0x10, 0x28, 0x44, 
  0x4C, 0x90, 0x90, 0x90, 0x7C, 
  0x44, 0x64, 0x54, 0x4C, 0x44,   //z
  0x00, 0x08, 0x36, 0x41, 0x00, 
};

void setup() {

          //set timer0 in=250Hz out=1Hz
    OCR0A = 249;    //divide by 250
    TCCR0A=0b1000011;  //
    TCCR0B=0b1110;  //  PWM mode, input T0 pin D4
    pinMode(6, OUTPUT);
    
      // set timer2 in=16MHz out=250Hz
    OCR2A =249;
    OCR2B = 125;    //50% duty cycle
    TCCR2A=0b110011;  //output B in phase, fast PWM mode
    TCCR2B=0b1110; // set prescaler to 256 and start the timer
    pinMode(3, OUTPUT);
    
        //  set timer1
    OCR1A = 32767;   //32768 counts
    TCCR1A = 0b1000011;  
    TCCR1B = 0b11110; //input pin D5
    pinMode(9, OUTPUT);

    Wire.begin();        // init i2c bus
    Wire.setClock(100000);  //200khz
    oled_init();
    clrScreen();
}

void loop()
{
  unsigned char d[7], nz, i;
  byte overF=0;
  unsigned long total, period;

    nz=1;
      //wait for high output
    while(digitalRead(6)){}
    while(!digitalRead(6)){}
      //start the count
    TIFR1 = _BV(OCF1A);    //reset int
    OCR1A = 32767;
    TCNT1=0;
    overF=0;
    while(digitalRead(6)){
    if(TIFR1 & _BV(1)) {++overF; TIFR1 |= _BV(1);}   //on overflow reset interrupt 
    }
        //count end
    total = (unsigned long)TCNT1 + ((unsigned long)overF * 32768);  //sum up registers

        //convert to 7 decimal digits
    d[6]=total/1000000;    //1MHz digit
    d[5]=(total/100000) %10;  //100KHz digit
    d[4]=(total/10000) %10;
    d[3]=(total/1000) %10;
    d[2]=(total/100) %10;
    d[1]=(total/10) %10;    //10Hz digit  
    d[0]=total %10;

    for(i=7;i>1;i--){
        if(!d[i-1] && nz){drawChar2(0x20, 1, 7-i);} //if leading 0 display blank
        if(d[i-1] && nz){nz=0; }
        if(!nz){drawChar2(d[i-1]+48, 1, 7-i);}
    }
    drawChar2(d[0]+48, 1, 6); //display digit 0

    drawChar2(0x48, 1, 8); //H 0x48
    drawChar2(0x7A, 1, 9); //z

    nz=1;
    if(total<1000){
        period=1000000/total; 
        drawChar2(0x75, 5, 8); //u
    }else{
        period=1000000000/total;
        drawChar2(0x6E, 5, 8);}    //n
    if(total==0)period=0;    

    //convert to 7 decimal digits
    d[6]=period/1000000;   //1MHz digit
    d[5]=(period/100000) %10;  //100KHz digit
    d[4]=(period/10000) %10;
    d[3]=(period/1000) %10;
    d[2]=(period/100) %10;
    d[1]=(period/10) %10;    //10Hz digit  
    d[0]=period %10;

    for(i=7;i>1;i--){
        if(!d[i-1] && nz){drawChar2(0x20, 5, 7-i);} //if leading 0 display blank
        if(d[i-1] && nz){nz=0; }
        if(!nz){drawChar2(d[i-1]+48, 5, 7-i);}
    } 
     drawChar2(d[0]+48, 5, 6);  //display digit 0

     drawChar2(0x53, 5, 9); //S
     
}

void clrScreen()    //fill screen with 0
{
    unsigned char y, x;

    for ( y = 0; y < 8; y++ ) {
      for (x = 0; x < 17; x++){
          command(0x21);     //col addr
          command(8 * x); //col start
          command(8 * x + 7);  //col end
          command(0x22);    //0x22
          command(y); // Page start
          command(y); // Page end
          
          Wire.beginTransmission(addr);
          Wire.write(0x40);
          for (unsigned char i = 0; i < 8; i++){
               Wire.write(0x00);          
          }
          Wire.endTransmission();         
      }
     
    }
}

void command( unsigned char comm){
    Wire.beginTransmission(addr); 
    Wire.write(0x00);    
    Wire.write(comm); // LSB
    Wire.endTransmission();       
}

void oled_init() {
    
    command(0xAE);   // DISPLAYOFF
    command(0x8D);         // CHARGEPUMP *
    command(0x14);     //0x14-pump on
    command(0x20);         // MEMORYMODE
    command(0x0);      //0x0=horizontal, 0x01=vertical, 0x02=page
    command(0xA1);        //SEGREMAP * A0/A1=top/bottom 
    command(0xC8);     //COMSCANDEC * C0/C8=left/right
    command(0xDA);         // SETCOMPINS *
    command(0x12);   //0x22=4rows, 0x12=8rows
    command(0x81);        // SETCONTRAST
    command(0x9F);     //0x8F
    //following settings are set by default
//    command(0xD5);  //SETDISPLAYCLOCKDIV 
//    command(0x80);  
//    command(0xA8);       // SETMULTIPLEX
//    command(0x3F);     //0x1F
//    command(0xD3);   // SETDISPLAYOFFSET
//    command(0x0);  
//    command(0x40); // SETSTARTLINE  
//    command(0xD9);       // SETPRECHARGE
//    command(0xF1);
//    command(0xDB);      // SETVCOMDETECT
//    command(0x40);
//    command(0xA4);     // DISPLAYALLON_RESUME
//    command(0xA6);      // NORMALDISPLAY
    command(0xAF);          //DISPLAYON

}

    //size x1 chars
void drawChar(char fig, unsigned char y, unsigned char x)
{
    
    command(0x21);     //col addr
    command(7 * x); //col start
    command(7 * x + 4);  //col end
    command(0x22);    //0x22
    command(y); // Page start
    command(y); // Page end
    
    Wire.beginTransmission(addr);
    Wire.write(0x40);
    for (unsigned char i = 0; i < 5; i++){
         Wire.write(font[5*(fig-32)+i]);          
    }
    Wire.endTransmission();
 
 }
    //size x2 chars
void drawChar2(char fig, unsigned char y, unsigned char x)
{
    unsigned char i, line, btm, top;    //
    
    command(0x20);    // vert mode
    command(0x01);

    command(0x21);     //col addr
    command(13 * x); //col start
    command(13 * x + 9);  //col end
    command(0x22);    //0x22
    command(y); // Page start
    command(y+1); // Page end
    
    Wire.beginTransmission(addr);
    Wire.write(0x40);

    for (i = 0; i < 5; i++){
        line=font[5*(fig-32)+i];
        btm=0; top=0;
            // expend char    
        if(line & 64) {btm +=192;}
        if(line & 32) {btm +=48;}
        if(line & 16) {btm +=12;}           
        if(line & 8) {btm +=3;}
        
        if(line & 4) {top +=192;}
        if(line & 2) {top +=48;}
        if(line & 1) {top +=12;} 
               
         Wire.write(top); //top page
         Wire.write(btm);  //second page
         Wire.write(top);
         Wire.write(btm);

    }
    Wire.endTransmission();
        
    command(0x20);      // horizontal mode
    command(0x00);    
        
}
