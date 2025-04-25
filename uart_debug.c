#include "uart_debug.h"



void UART_Config_debug(void){
	GPIO_InitTypeDef GPIOInitStruct;
	USART_InitTypeDef USARTInitStruct;
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	//
	GPIOInitStruct.GPIO_Pin = UART_TX;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	//
	GPIOInitStruct.GPIO_Pin = UART_RX;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	// config led PA5
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
	//Usart
	USARTInitStruct.USART_BaudRate = 9600;
	USARTInitStruct.USART_WordLength = USART_WordLength_8b;
	USARTInitStruct.USART_StopBits = USART_StopBits_1;
	USARTInitStruct.USART_Parity = USART_Parity_No;
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USARTInitStruct);
	USART_Cmd(USART1,ENABLE);
}


/*
 * Function name: UART_SendChar
 * Function: Send data to terminal
 * Input: USART_TypeDef *USARTx, char data  --> data to send
 * Output: No
 */
void UART_SendChar(USART_TypeDef *USARTx, char data){
	USARTx->DR = 0x00;
	USART_SendData(USARTx, data);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}


/*
 * Function name: UART_SendString
 * Function: Send string data
 * Input: USART_TypeDef *USARTx, char *str --> String data to send
 * Output: No
 */
void UART_SendString(USART_TypeDef *USARTx, char *str){
	while(*str)
		{
		UART_SendChar(USARTx, *str);
		str++;
		}
}