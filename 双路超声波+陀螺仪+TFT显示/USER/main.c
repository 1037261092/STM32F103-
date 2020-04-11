#include "delay.h"
#include "hcsr04.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 #include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "TFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#define Trig PCout(4)		//PC1	TRIG
#define Trig2 PCout(5)		//PC2	TRIG
//��������
void UltrasonicWave_Configuration(void);		//����
void UltrasonicWave_StartMeasure(void);			//����
void UltrasonicWave_Configuration2(void);		//����
void UltrasonicWave_StartMeasure2(void);			//����

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ	
extern u8  TIM5CH2_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH2_CAPTURE_VAL;	//���벶��ֵ	

float pitch,roll,yaw,Parallel,Vertical,r,d; 		//ŷ����









 int main(void)
 {		
	 //�����Ƕ���
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�	    
	 u8 report=1;			//Ĭ�Ͽ����ϱ�
	
	 //����������
	float length;
	u32 tempdis=0; 	
	 u32 tempdis2=0; 
	 float UltrasonicWave_Distance=0;      //������ľ���
	 float UltrasonicWave_Distance2=0;      //������ľ���
  SystemInit();	//System init.
  delay_init();//Delay init.
	 uart_init(9600);
	HC_SR04Init();
	  MPU_Init();
	 mpu_dmp_init();
	Test_Demo();
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

 
	UltrasonicWave_Configuration();
	UltrasonicWave_Configuration2();
 	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
	 
	 while(mpu_dmp_init())       //6050��ʼ�����
 	{
		printf("none\n");
	}  
	printf("Yes\n");
   	while(1)
	{
		delay_us(50);
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
				/***��������ʾ ***/
				temp=pitch*10;
				if(temp<0)
				{
					Gui_DrawFont_GBK16(10,100,BLACK,GRAY0,"-");
					temp=-temp;		//תΪ����
				}else Gui_DrawFont_GBK16(10,100,BLACK,GRAY0," ");
				 shuzi(3,35,100,temp);
				
				printf("--pitch %d--",temp);
				//if()
				
				/***�������ʾ***/
				temp=roll*10;
				if(temp<0)
				{
					Gui_DrawFont_GBK16(50,100,BLACK,GRAY0,"-");
					temp=-temp;		//תΪ����
				}else Gui_DrawFont_GBK16(50,100,BLACK,GRAY0," ");
				 shuzi(3,75,100,temp);
				printf("--roll %d--",temp);
				
				/***�������ʾ***/
				temp=yaw*10;
				if(temp<0)
				{
					Gui_DrawFont_GBK16(90,100,BLACK,GRAY0,"-");
					temp=-temp;		//תΪ����
				}else Gui_DrawFont_GBK16(90,100,BLACK,GRAY0," ");
				 shuzi(3,115,100,temp);
			printf("--yaw %d--\n",temp);
		 
				Vertical=roll;
		  	Parallel=pitch;
		
		}
		
		
		
		
//		
		
		
	{	UltrasonicWave_StartMeasure();//��ʼ���
		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			tempdis=TIM5CH1_CAPTURE_STA&0X3F;
			tempdis*=65536;//���ʱ���ܺ�
			tempdis+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			
			UltrasonicWave_Distance=(float)tempdis*170/10000;//������㹫ʽ������=�ߵ�ƽ����ʱ��*����/2   
														  //�� ����= �ߵ�ƽ����ʱ��*340*100/��2*1000 000�� = �ߵ�ƽ����ʱ��*170/10000
			 
			
			shuzi(5,100,35,(int)(UltrasonicWave_Distance));
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
		}
	}
	
	length = HC_SR04_GetLength();
	shuzi(5,100,75,(int)(length));
		delay_ms(50);
	
	
		{UltrasonicWave_StartMeasure2();//��ʼ���
		 		 
 		if(TIM5CH2_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			tempdis2=TIM5CH2_CAPTURE_STA&0X3F;
			tempdis2*=65536;//���ʱ���ܺ�
			tempdis2+=TIM5CH2_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			
			UltrasonicWave_Distance2=(float)tempdis2*170/10000;//������㹫ʽ������=�ߵ�ƽ����ʱ��*����/2   
														  //�� ����= �ߵ�ƽ����ʱ��*340*100/��2*1000 000�� = �ߵ�ƽ����ʱ��*170/10000
			 
			shuzi(5,100,55,(int)(UltrasonicWave_Distance2));
		
			TIM5CH2_CAPTURE_STA=0;//������һ�β���
		}
	}
	
	}
 }


/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //PC4��TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //��ʼ������GPIO 

}
void UltrasonicWave_Configuration2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;					 //PC8��TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //��ʼ������GPIO 

}
/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(void)
{
	Trig = 1;
	delay_us(20);		                      //��ʱ20US
	Trig = 0;
	
}
void UltrasonicWave_StartMeasure2(void)
{
	Trig2 = 1;
	delay_us(20);		                      //��ʱ20US
	Trig2 = 0;
	
}

