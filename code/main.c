#include "stc8g.h"    
#include "intrins.h" 
#include "stdio.h"    

typedef unsigned char u8;
typedef unsigned int u16;

sbit SW = P3^2;
sbit BTM = P3^3;

u8 idle[] = {0xAA,0x06,0x00,0x9F,0x02,0x02,0x3C,0xC5};
u8 wash[] = {0xAA,0x06,0x01,0x98,0x02,0x02,0xB2,0x0A};
u8 dry[]  = {0xAA,0x06,0x01,0x98,0x04,0x00,0x75,0x03};



void delay_ms(u16 ms)
{
    u16 i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 1575; j++);
    }
}

void UartInit(void)
{
    SCON = 0x50;		// 0101 0000
    AUXR |= 0x40;		// set x1xx xxxx set TIM1 speed 1T.
    AUXR &= 0xFE;		// set xxxx xxx0	set TIM1 as bitrate generater.
    TMOD &= 0x0F;		// set 0000 xxxx reset TIM1;set TIM1 as timer
		TMOD |= 0x20;		// set xx1x xxxx as 8bit autoload.
//		PCON |= 0x80;		// set 1xxx xxxx doublize bitrate.
		PCON &= 0x7f;		// set 0xxx xxxx normal bitrate.
    ET1 = 0;				// disable TimInt.
    TR1 = 1;				// Enable Timer
    TI = 1;					// send flag.should be clear when receive.
	
//		TH1=0xDC;	// 9600
//		TL1=0xDC;
	
		TH1=0x70;	// 2400
		TL1=0x70;
}

void Int1_Init(void)
{
    IT1 = 1;  // 0=low vol trigger,1=fall edge trigger
    EX1 = 1;  // Enable Int1
    EA = 1;   // Enable Int
}

void send_byte(u8 byte)
{
    SBUF = byte;
    while(!TI);
    TI = 0;
}

void usart_sendarry(u8 a[],u8 num){
	u8 i;
	for(i=0;i<num;i++){
		send_byte(a[i]);
	}
}

void main() {
		BTM = 1;
		SW = 1;
    UartInit();
		Int1_Init();
		
    printf("hello");
    while(1){
      usart_sendarry(idle,8);
			
			delay_ms(1000); 
		}			
    
}

void ExtInt2_ISR(void) interrupt 2
{
    delay_ms(10);  
    if(BTM == 0){
			if(SW == 0)
				usart_sendarry(wash,8);
			else
				usart_sendarry(dry,8);
			while(BTM == 0);  
    }
}
