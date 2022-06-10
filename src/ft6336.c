#include "ft6336.h"

void delay_us(unsigned int x)
{
		unsigned long i;
		while(--x!=0){
			i=1000;
			while(i--);
		}
}

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   
    
  
    GPIO_Initure.Pin=FT6336_SDA|FT6336_SCL;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
    GPIO_Initure.Pull=GPIO_PULLUP;         
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     
    HAL_GPIO_Init(FT6336_PORT,&GPIO_Initure);
    
    IIC_SDA(1);
    IIC_SCL(1);  
}

void SDA_IN(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		GPIO_Initure.Pin=FT6336_SDA;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  
    GPIO_Initure.Pull=GPIO_PULLUP;         
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     
    HAL_GPIO_Init(FT6336_PORT,&GPIO_Initure);
}

void SDA_OUT(void)
{
		GPIO_InitTypeDef GPIO_Initure;
		GPIO_Initure.Pin=FT6336_SDA;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
    GPIO_Initure.Pull=GPIO_PULLUP;         
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     
    HAL_GPIO_Init(FT6336_PORT,&GPIO_Initure);
}

void IIC_Start(void)
{
	SDA_OUT();     
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);
	delay_us(4);
	IIC_SCL(0);
}	  

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
 	delay_us(4);
	IIC_SCL(1); 
	delay_us(4);			
	IIC_SDA(1);  	
}

unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime=0;
	SDA_IN();     
	IIC_SDA(1);delay_us(1);	   
	IIC_SCL(1);delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0); 
	return 0;  
} 

void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
	    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}					 				     

void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
	SDA_OUT(); 	    
    IIC_SCL(0);
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);  
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 	    

unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        delay_us(2);
		IIC_SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();  
    return receive;
}

unsigned char ft6336_write_byte(unsigned char addr,unsigned char reg,unsigned char data)
{
    IIC_Start();
    IIC_Send_Byte((addr<<1)|0); 
    if(IIC_Wait_Ack())          
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         
    IIC_Wait_Ack();             
    IIC_Send_Byte(data);        
    if(IIC_Wait_Ack())          
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}

unsigned char ft6336_read_byte(unsigned char addr,unsigned char reg)
{
    unsigned char res;
    IIC_Start();
    IIC_Send_Byte((addr<<1)|0); 
    IIC_Wait_Ack();             
    IIC_Send_Byte(reg);         
    IIC_Wait_Ack();             
	  IIC_Start();                
    IIC_Send_Byte((addr<<1)|1); 
    IIC_Wait_Ack();             
    res=IIC_Read_Byte(0);		
    IIC_Stop();                 
    return res;  
}

unsigned char ft6336_get_td_status(void)
{
		unsigned char a;
		a=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TD_STATUS);
		return a;
}

void ft6336_get_touch1_position(unsigned int *x,unsigned int *y)
{
		unsigned int xh=0,xl=0,yh=0,yl=0;
		xh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_XH);
		xl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_XL);
		yh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_YH);
		yl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH1_YL);
		*x=((xh&0x000F)<<8)|xl;
		*y=((yh&0x000F)<<8)|yl;
}

void ft6336_get_touch2_position(unsigned int *x,unsigned int *y)
{
		unsigned int xh=0,xl=0,yh=0,yl=0;
		xh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_XH);
		xl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_XL);
		yh=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_YH);
		yl=ft6336_read_byte(FT6336_ADDR,FT6336_ADDR_TOUCH2_YL);
		*x=((xh&0x000F)<<8)|xl;
		*y=((yh&0x000F)<<8)|yl;
}