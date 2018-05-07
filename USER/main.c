# include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
# include "string.h"
// 问题答案库
const u8 num = 10;
////一次接收的数据最大不能超过 USART_REC_LEN 个字节，用于定义USART_RX_BUF 的大小
char question[num][USART_REC_LEN +1] = {
	"1+1=",
	"What is the course's name?",
	"Who is your partner?",
	"How are you?",
	"What is your major?",
	"你的家乡是哪里？",
	"你就读于哪所大学？",
	"你的星座是？",
	"你喜欢哪种水果？",
	"最近在看的书？"
};
char answer[num][USART_REC_LEN +1] = {
	"2",
	"Embedded System.",
	"Ziyue Dong.",
	"Not bad. :)",
	"Transportation Engineering.",
	"上海崇明，可爱小岛~",
	"同济大学黄渡理工",
	"巨蟹座",
	"西瓜和草莓 ^_^",
	"《无声告白》，《道林格雷的画像》"
};

int main(void)
{		
 	u8 t;
	u8 len;	
	u8 len2;
	char Res[USART_REC_LEN +1];//接收的字符串
	char Return[USART_REC_LEN +1];//发送到电脑的字符串
	
	// 初始化
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	BEEP_Init();          //蜂鸣器初始化	
	TIM3_Int_Init(2999,7199);  //TIME3定时器初始化，300ms进入定时中断，注意在timer.h中申明 	

	// 实现功能
	while(1)
	{		
		//Reference to "串口实验"
		//接收状态寄存器的bit14 接收到0X0D标志，接收完成
		if(USART_RX_STA&0x8000)
		{				
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			memcpy(Res,USART_RX_BUF,len);//将BUF中读取的数据复制到Res中，记得数组末尾'\0'
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
						strcpy(Return,"无法回答该问题！");
					}
				}
			}
			
			//send the answer to the computer
			len2=strlen(Return);
			for(t=0;t<len2;t++)
			{
				USART_SendData(USART1, Return[t]);//向串口1发送数据：发送一个字符串到串口1
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET)
				{
				}//等待发送结束：在发送一个数据到串口之后，检测该数据是否被发送完成
			}
			
			USART_RX_STA=0;//将接受数据的长度置零
		}
		else
		{
		}
	}	 
 }
