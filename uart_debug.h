#ifndef __UART_H
#define __UART_H


#include "stm32f10x.h"
#include "stm32f10x_usart.h"

#define	UART_GPIO 	GPIOA
#define	UART_TX 		GPIO_Pin_9
#define	UART_RX			GPIO_Pin_10


void UART_Config_debug(void);
void UART_SendChar(USART_TypeDef *USARTx, char data);
void UART_SendString(USART_TypeDef *USARTx, char *str);


#endif