# include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
# include "string.h"
// ����𰸿�
const u8 num = 10;
////һ�ν��յ���������ܳ��� USART_REC_LEN ���ֽڣ����ڶ���USART_RX_BUF �Ĵ�С
char question[num][USART_REC_LEN +1] = {
	"1+1=",
	"What is the course's name?",
	"Who is your partner?",
	"How are you?",
	"What is your major?",
	"��ļ��������",
	"��Ͷ���������ѧ��",
	"��������ǣ�",
	"��ϲ������ˮ����",
	"����ڿ����飿"
};
char answer[num][USART_REC_LEN +1] = {
	"2",
	"Embedded System.",
	"Ziyue Dong.",
	"Not bad. :)",
	"Transportation Engineering.",
	"�Ϻ��������ɰ�С��~",
	"ͬ�ô�ѧ�ƶ���",
	"��з��",
	"���ϺͲ�ݮ ^_^",
	"��������ס��������ָ��׵Ļ���"
};

int main(void)
{		
 	u8 t;
	u8 len;	
	u8 len2;
	char Res[USART_REC_LEN +1];//���յ��ַ���
	char Return[USART_REC_LEN +1];//���͵����Ե��ַ���
	
	// ��ʼ��
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	BEEP_Init();          //��������ʼ��	
	TIM3_Int_Init(2999,7199);  //TIME3��ʱ����ʼ����300ms���붨ʱ�жϣ�ע����timer.h������ 	

	// ʵ�ֹ���
	while(1)
	{		
		//Reference to "����ʵ��"
		//����״̬�Ĵ�����bit14 ���յ�0X0D��־���������
		if(USART_RX_STA&0x8000)
		{				
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			memcpy(Res,USART_RX_BUF,len);//��BUF�ж�ȡ�����ݸ��Ƶ�Res�У��ǵ�����ĩβ'\0'
			Res[len]='\0';
			if(strcmp(Res,"hello world!") == 0)
			//send "hello world!" to computer,beep lasts 1000ms
			{
				BEEP=1;
				delay_ms(1000);
				BEEP=0;
				strcpy(Return,Res);
			}
			else
			//send a question and give answer
			{
				LED1=0;
				delay_ms(500);
				LED1=1;
				for(t=0;t<num;t++)
				{
					if(strcmp(Res,question[t])== 0)
					{
						strcpy(Return,answer[t]);
						break;
					}
					else
					{
						strcpy(Return,"�޷��ش�����⣡");
					}
				}
			}
			
			//send the answer to the computer
			len2=strlen(Return);
			for(t=0;t<len2;t++)
			{
				USART_SendData(USART1, Return[t]);//�򴮿�1�������ݣ�����һ���ַ���������1
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET)
				{
				}//�ȴ����ͽ������ڷ���һ�����ݵ�����֮�󣬼��������Ƿ񱻷������
			}
			
			USART_RX_STA=0;//���������ݵĳ�������
		}
		else
		{
		}
	}	 
 }
