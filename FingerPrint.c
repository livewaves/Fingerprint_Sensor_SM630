                                                                                                                                                                                                                                                                                                                                                                  #include "built_in.h"
// Lcd pinout settings
sbit LCD_RS at PORTA0_bit;
sbit LCD_EN at PORTA1_bit;
sbit LCD_D7 at PORTA5_bit;
sbit LCD_D6 at PORTA4_bit;
sbit LCD_D5 at PORTA3_bit;
sbit LCD_D4 at PORTA2_bit;

// Pin direction
sbit LCD_RS_Direction at DDA0_bit;
sbit LCD_EN_Direction at DDA1_bit;
sbit LCD_D7_Direction at DDA5_bit;
sbit LCD_D6_Direction at DDA4_bit;
sbit LCD_D5_Direction at DDA3_bit;
sbit LCD_D4_Direction at DDA2_bit;

char i, RD, Send[15];
int FP_Counter;
char txt[6];

void Command() {
 Send[0]=0x4D;
 Send[1]=0x58;
 Send[2]=0x10;
 for(i=                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             0;i<3;i++){
  UART1_Write(Send[i]);
 }
}

void Add_FP() {
 Send[3]=0x03;
 Send[4]=0x40;
 Send[5]=0x00;
 Send[6]=0x00;
 Send[7]=0xF8;
 for(i=3;i<8;i++){
  UART1_Write(Send[i]);
 }
}

void Del_FP() {
 Send[3]=0x03;
 Send[4]=0x42;
 Send[5]=0x00;
 Send[6]=0x00;
 Send[7]=0xFA;
 for(i=3;i<8;i++){
  UART1_Write(Send[i]);
 }
}

void Response() {
     //ByteToStr(Send[4], txt);
     //Lcd_Out(2,7, txt);
 switch (Send[4]) {
  case 0x40:
   switch (RD) {
    case 0xD7:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,2, ">...FINGER...<");Lcd_Out(2,6, txt);break;
    case 0x48:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,2, "ADD SUCCESSFUL");Lcd_Out(2,6, txt);break;
    case 0x4C:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,2, "PARAMETER ERR.");Lcd_Out(2,6, txt);break;
    case 0x4B:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,2, "PROCESS FAILED");Lcd_Out(2,6, txt);break;
    case 0x4A:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,5, "TIME OUT");break;
   }
  break;
  
  case 0x42:
   switch (RD) {
    case 0xD7:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,5, "DELETION");break;
    case 0x4A:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,4, "FP DELETED");Lcd_Out(2,6, txt);break;
    case 0x4E:Lcd_Cmd(_LCD_CLEAR);Lcd_Out(1,2, "PARAMETER ERR.");Lcd_Out(2,6, txt);break;
   }
  break;
 }
}

void main() {
FP_Counter=0;
 i=0;
 DDRB = 0x00;
 PORTB = 0xFF;
 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Out(1, 2, "FP SENSOR CTRL");
 Delay_ms(2000);
 Lcd_Cmd(_LCD_CLEAR);
 UART1_Init(57600);              // Initialize UART module at 9600 bps
 Delay_ms(800);                  // Wait for UART module to stabilize
 
 while(1) {
  switch (PINB) {
   case 0xFE:
    Command();
    Add_FP();
   break;
   
   case 0xFD:
    Command();
    Del_FP();
   break;
   
   case 0xFB:
    Command();
    Del_FP();
   break;
   
   case 0xF7:
    Command();
    Del_FP();
   break;

   case 0xEF:
    FP_Counter--;
    if(FP_Counter<0) FP_Counter=0;
    Send[5] = Hi(FP_Counter);
    Send[6] = Lo(FP_Counter);
    WordToStrWithZeros(FP_Counter, txt);
    Lcd_Out(2,6, txt);
    delay_ms(100);
   break;

   case 0xDF:
    FP_Counter++;
    if(FP_Counter>768) FP_Counter=768;
    Send[5] = Hi(FP_Counter);
    Send[6] = Lo(FP_Counter);
    WordToStrWithZeros(FP_Counter, txt);
    Lcd_Out(2,6, txt);
    delay_ms(100);
   break;
  }
    if(UART1_Data_Ready()) {
     RD = UART1_Read();
     //ByteToStr(RD, txt);
     //Lcd_Out(2,1, txt);
     Response();
    }
    Delay_ms(100);
 }
}