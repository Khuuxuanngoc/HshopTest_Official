//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STC12C5A60S2,����12M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for C51
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/8/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
//********************************************************************************
//=========================================��Դ����================================================//
//5V��DC 5V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI
//Һ����ģ��            ��Ƭ��
//  SDI          ��       P15        //SPIд�ź�
//=======================================Һ���������߽���==========================================//
//Һ����ģ��            ��Ƭ��
//  CS           ��       P13       //Ƭѡ�����ź�
//  RST          ��       P33       //��λ�ź�
//  RS           ��       P12       //����/����ѡ������ź�
//  CLK          ��       P17       //SPIʱ���ź�
//  LED          ��       P32       //��������źţ����ʹ��STC89C52RC��Ƭ�����߲���Ҫ���ƣ���3.3V
//=========================================����������=========================================//
//STC89C52RC��Ƭ��ROM̫С���޷����д��������ܵĳ������Դ���������Ҫ����
//��ʹ�ô�������ģ�鱾������������ɲ�����
//������ʹ�õ�������������ΪSPI
//������ģ��            ��Ƭ��
//  T_CLK        ��       P36       //������SPIʱ���ź�
//  T_CS         ��       P37       //������Ƭѡ�����ź�
//  T_DIN        ��       P34       //������SPIд�ź�
//  T_DO         ��       P35       //������SPI���ź�
//  T_IRQ        ��       P40       //��������Ӧ����źţ��絥Ƭ����P4�飬�����и�����������IO���޸Ĵ��붨��
//**************************************************************************************************/		
#include "sys.h"
#include "lcd.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

void  delay_ms_new(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void	SPI_init(void)	
{
	SPCTL = (SSIG << 7)+(SPEN << 6)+(DORD << 5)+(MSTR << 4)+(CPOL << 3)+(CPHA << 2)+SPEED_4;
}

/*****************************************************************************
 * @name       :void SPI_WriteByte(u8 byte)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using C51's Hardware SPI
 * @parameters :d:Data to be written
 * @retvalue   :None
******************************************************************************/
void SPI_WriteByte(u8 byte)
{			
			SPDAT = byte;		//����һ���ֽ�
			while((SPSTAT & SPIF)==0)	;	//�ȴ��������
			SPSTAT = SPIF+WCOL;			//��0 SPIF��WCOL��־
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 Reg)	 
{	
	LCD_CS=0;
	LCD_RS=0;
	SPI_WriteByte(Reg);
	LCD_CS=1;
} 

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 Data)
{
	LCD_CS=0;
	LCD_RS=1;
	SPI_WriteByte(Data);
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void LCD_WR_DATA_16Bit(u16 Data)
{
	LCD_CS=0;
	LCD_RS=1;
	SPI_WriteByte(Data>>8);
	SPI_WriteByte(Data);
	LCD_CS=1;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA_16Bit(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	u16 i,j;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
    for(i=0;i<lcddev.width;i++)
	 {
	  for (j=0;j<lcddev.height;j++)
	   	{
        	 LCD_WR_DATA_16Bit(Color);
	    }

	  }
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	    
} 	 

/*****************************************************************************
 * @name       :void LCD_Reset(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_Reset(void)
{
	LCD_RESET=1;
	delay_ms_new(50);	
	LCD_RESET=0;
	delay_ms_new(50);
	LCD_RESET=1;
	delay_ms_new(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{
	SPI_init(); //��ʼ��Ӳ��SPI
	LCD_Reset(); //��ʼ��֮ǰ��λ
//************* ILI9225��ʼ��**********//	
	LCD_WriteReg(0x01,0x011c);
	LCD_WriteReg(0x02,0x0100);
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x08,0x0808); // set BP and FP
	LCD_WriteReg(0x0B,0x1100); //frame cycle
	LCD_WriteReg(0x0C,0x0000); // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit
	LCD_WriteReg(0x0F,0x1401); // Set frame rate----0801
	LCD_WriteReg(0x15,0x0000); //set system interface
	LCD_WriteReg(0x20,0x0000); // Set GRAM Address
	LCD_WriteReg(0x21,0x0000); // Set GRAM Address
	//*************Power On sequence ****************//
	delay_ms(50);                         // Delay 50ms
	LCD_WriteReg(0x10,0x0800); // Set SAP,DSTB,STB----0A00
	LCD_WriteReg(0x11,0x1F3F); // Set APON,PON,AON,VCI1EN,VC----1038
	delay_ms(50);                         // Delay 50ms
	LCD_WriteReg(0x12,0x0121); // Internal reference voltage= Vci;----1121
	LCD_WriteReg(0x13,0x006F); // Set GVDD----0066
	LCD_WriteReg(0x14,0x4349); // Set VCOMH/VCOML voltage----5F60
	//-------------- Set GRAM area -----------------//
	LCD_WriteReg(0x30,0x0000);
	LCD_WriteReg(0x31,0x00DB);
	LCD_WriteReg(0x32,0x0000);
	LCD_WriteReg(0x33,0x0000);
	LCD_WriteReg(0x34,0x00DB);
	LCD_WriteReg(0x35,0x0000);
	LCD_WriteReg(0x36,0x00AF);
	LCD_WriteReg(0x37,0x0000);
	LCD_WriteReg(0x38,0x00DB);
	LCD_WriteReg(0x39,0x0000);
	// ----------- Adjust the Gamma Curve ----------//
	LCD_WriteReg(0x50,0x0001);  //0400
	LCD_WriteReg(0x51,0x200B);  //060B
	LCD_WriteReg(0x52,0x0000);  //0C0A
	LCD_WriteReg(0x53,0x0404);  //0105
	LCD_WriteReg(0x54,0x0C0C);  //0A0C
	LCD_WriteReg(0x55,0x000C);  //0B06
	LCD_WriteReg(0x56,0x0101);  //0004
	LCD_WriteReg(0x57,0x0400);  //0501
	LCD_WriteReg(0x58,0x1108);  //0E00
	LCD_WriteReg(0x59,0x050C);  //000E
	delay_ms(50);                                     // Delay 50ms
	LCD_WriteReg(0x07,0x1017);
	LCD_WR_REG(0x22);

	//����LCD���Բ���
	LCD_direction(USE_HORIZONTAL);//����LCD��ʾ���� 
	LCD_BL=1;//��������	 
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	u16 x,y,tmp;
	switch(lcddev.dir)
	{
		case 0:
			x = xStar;
			y = yStar;
			break;
		case 1:
			tmp = yStar;
			yStar = xStar;
			xStar = LCD_W-1-yEnd;
			yEnd = xEnd;
			xEnd = LCD_W-1-tmp;
			x = xEnd;
			y = yStar;
			break;
		case 2:
			tmp = xStar;
			xStar = LCD_W-1-xEnd;
			xEnd = LCD_W-1-tmp;
			tmp = yStar;
			yStar = LCD_H-1-yEnd;
			yEnd = LCD_H-1-tmp;
			x = xEnd;
			y = yEnd;
			break;
		case 3:
			tmp = xStar;
			xStar = yStar;
			yStar = LCD_H-1-xEnd;
			xEnd = yEnd;
			yEnd = LCD_H-1-tmp;
			x = xStar;
			y = yEnd;
			break;
		default:
			break;
	}
	LCD_WriteReg(0x36, xEnd);
	LCD_WriteReg(0x37, xStar);
	LCD_WriteReg(0x38, yEnd);
	LCD_WriteReg(0x39, yStar);
	LCD_WriteReg(lcddev.setxcmd, x);
	LCD_WriteReg(lcddev.setycmd, y);
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.dir = direction&3;
			lcddev.setxcmd=0x20;
			lcddev.setycmd=0x21;
			lcddev.wramcmd=0x22;
	switch(lcddev.dir){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x03,0x1030);
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x03,0x1028);
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x03,0x1000);
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x03,0x1018);
		break;	
		default:break;
	}		
}	 





