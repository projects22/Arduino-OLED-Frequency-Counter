//OLED frequency counter PIC16F876A
//MPLAB X v3.10 xc8 v1.35
//moty22.co.uk


#include <pic16f876a.h>
#include <xc.h>
#include "oled_font.c"
#pragma config LVP=OFF, FOSC=HS, CP=OFF, CPD=OFF, WDTE=OFF
#define _XTAL_FREQ 8000000

// prototypes
void command( unsigned char comm);
void oled_init();
void drawChar(char fig, unsigned char y, unsigned char x);
void clrScreen();
void sendData(unsigned int dataB);
void startBit(void);
void stopBit(void);
void drawChar2(char fig, unsigned char y, unsigned char x);


void main(void) {
   	unsigned long total;
	unsigned char timebase,nz,i,d[7];
	unsigned int freq2;
    
   	TRISA = 0b10000;		//RA4 freq input
    TRISC = 0b11111;		// sck rc3, sda rc4.
    //set timers
    OPTION_REG = 0b11111000;	//tmr0 1:1
   	T1CON=0;		//timer OFF, 1:1
	CCP1CON=0b1011;		//compare mode
	CCPR1L=0x40;	//CCP in compare mode sets TMR1 to a period of 20 ms
    CCPR1H=0x9c;
    
	//i2c init
	SMP=1;	//slew rate 100khz
	CKE=0;	//SMBus dis
	SSPADD = 9; //200khz
	SSPCON = 0B101000;	//SSPEN=1, Master mode, clock = FOSC / (4 * (SSPADD+1))
	SSPCON2 = 0;
    __delay_ms(100);
     
    oled_init();   // oled init
    clrScreen();       // clear screen

    while (1) {

                //Frequency Counter
        nz=1;
        freq2 = 0;	//clear timers
        timebase=50;
        TMR1L=0;  TMR1H=0;			
        TMR1ON = 1;	//start count
        TMR0 = 0;
        TMR0IF = 0;

        while(timebase){		//1 sec 
            if(TMR0IF){++freq2; TMR0IF = 0;}
            if(CCP1IF){CCP1IF=0; --timebase;}
        }
        TMR1ON = 0;	//stop count

        total=(unsigned long)TMR0 + (unsigned long)freq2 * 256;
 
        //convert to 7 decimal digits
        d[6]=total/1000000;		//1MHz digit
        d[5]=(total/100000) %10;	//100KHz digit
        d[4]=(total/10000) %10;
        d[3]=(total/1000) %10;
        d[2]=(total/100) %10;
        d[1]=(total/10) %10;		//10Hz digit	
        d[0]=total %10;

        clrScreen();
        for(i=7;i>1;i--){
            if(!d[i-1] && nz){} //if 0 don't display
            if(d[i-1] && nz){nz=0; }
            if(!nz){drawChar2(d[i-1]+48, 1, 7-i);}
        }
        drawChar2(d[0]+48, 1, 6);	//display digit 0

        drawChar2(0x48, 1, 8); //H 0x48
        drawChar2(0x7A, 1, 9); //z

        nz=1;
        if(total<1000){
            total=1000000/total; 
            drawChar2(0x75, 5, 8); //u
        }else{
            total=1000000000/total;
            drawChar2(0x6E, 5, 8);}    //n

        //convert to 7 decimal digits
        d[6]=total/1000000;		//1MHz digit
        d[5]=(total/100000) %10;	//100KHz digit
        d[4]=(total/10000) %10;
        d[3]=(total/1000) %10;
        d[2]=(total/100) %10;
        d[1]=(total/10) %10;		//10Hz digit	
        d[0]=total %10;

        for(i=7;i>1;i--){
            if(!d[i-1] && nz){} //if 0 don't display
            if(d[i-1] && nz){nz=0; }
            if(!nz){drawChar2(d[i-1]+48, 5, 7-i);}
        }	
         drawChar2(d[0]+48, 5, 6);	//display digit 0

         drawChar2(0x53, 5, 9); //S

         __delay_ms(2000);

   }
 
}

unsigned int addr=0x78;  //

void sendData(unsigned int dataB)
{
    SSPIF=0;          // clear interrupt
    SSPBUF = dataB;              // send dataB
    while(!SSPIF);    // Wait to send
}

void startBit(void)
{
    SSPIF=0;
    SEN=1;          // start bit
    while(!SSPIF);
}

void stopBit(void)
{
    SSPIF=0;
    PEN=1;          // send stop
    while(!SSPIF);
}

void command( unsigned char comm){
    
    startBit();
    sendData(addr);            // address
    sendData(0x00);
    sendData(comm);             // command code
    stopBit();
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
 //   command(0xA6);      // NORMALDISPLAY
    command(0xAF);          //DISPLAYON

}

void clrScreen()    //fill screen with 0
{
    unsigned char y, i;
    
    for ( y = 0; y < 8; y++ ) {
    command(0xB0 + y);     //0 to 7 pages
    command(0x00); //low nibble
    command(0x10);  //high nibble
    
        startBit();
        sendData(addr);            // address
        sendData(0x40);
        for (i = 0; i < 128; i++){
             sendData(0x0);
        }
        stopBit();
    }    
}

    //size 1 chars
void drawChar(char fig, unsigned char y, unsigned char x)
{
    
    command(0x21);     //col addr
    command(7 * x); //col start
    command(7 * x + 4);  //col end
    command(0x22);    //0x22
    command(y); // Page start
    command(y); // Page end
    
        startBit();
        sendData(addr);            // address
        sendData(0x40);
        for (unsigned char i = 0; i < 5; i++){
             sendData(font[5*(fig-32)+i]);
        }
        stopBit();
        
 
 }
    //size 2 chars
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
    
        startBit();
        sendData(addr);            // address
        sendData(0x40);
        
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

             sendData(top); //top page
             sendData(btm);  //second page
             sendData(top);
             sendData(btm);
        }
        stopBit();
        
    command(0x20);      // horizontal mode
    command(0x00);    
        
}





