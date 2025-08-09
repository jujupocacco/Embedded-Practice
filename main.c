#include <stdint.h>
#include <string.h>                                                                                                                                                                    
/*OFFSET */                                                                                                                                                                   
#define RCC_OFFSET (0x3800UL)                                                                                                                                                 
#define AHB1ENR_OFFSET (0x30UL)                                                                                                                                               
#define APB1ENR_OFFSET (0x40UL)                                                                                                                                                                                                                                                                                                                           
#define GPIOA_OFFSET (0x0000UL)                                                                                                                                               
#define MODER_OFFSET (0x00UL)                                                                                                                                                 
#define AFRL_OFFSET (0x20UL)
#define ODR_OFFSET (0x14UL) 	

#define USART2_OFFSET (0x4400)
#define USART2_BRR_OFFSET (0x08UL) 
#define USART2_DR_OFFSET (0x04UL) 
#define USART2_SR_OFFSET (0x00UL)
#define USART2_CR1_OFFSET (0x0CUL)

/*BASE*/                                                                                                                                                                      
#define AHB1_BASE (0x40020000UL)
#define APB1_BASE (0x40000000UL) 
#define RCC_BASE (AHB1_BASE+RCC_OFFSET) 
#define GPIOA_BASE (AHB1_BASE+GPIOA_OFFSET) 
#define USART2_BASE (APB1_BASE+USART2_OFFSET)

/* PERIPH */
#define RCC_AHB1ENR (*(volatile uint32_t  *)(RCC_BASE+AHB1ENR_OFFSET)) 
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE+APB1ENR_OFFSET))  

#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+MODER_OFFSET)) 
#define GPIOA_AFRL (*(volatile uint32_t *)(GPIOA_BASE+AFRL_OFFSET))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE+ODR_OFFSET))

#define USART_SR (*(volatile uint32_t *)(USART2_BASE+USART2_SR_OFFSET))  
#define USART_DR (*(volatile uint32_t *)(USART2_BASE+USART2_DR_OFFSET)) 
#define USART_CR1 (*(volatile uint32_t *)(USART2_BASE+USART2_CR1_OFFSET))
#define USART_BRR (*(volatile uint32_t *)(USART2_BASE+USART2_BRR_OFFSET))

int main(){
	

	RCC_AHB1ENR |= (1U<<0); /*Clocks for AHB and APB bus*/ 
	RCC_APB1ENR |= (1U<<17); 
	
	GPIOA_MODER |= (1U<<10); /*SET GPIO PA5 as output for LED test*/

	GPIOA_MODER &= ~(3U<<4);
	GPIOA_MODER &= ~(3U<<6); /*CLEAR MODER2 and MODER3 to ensure functionality*/

	GPIOA_MODER |= (1U<<7); /* SET PA3 to alternate function mode for RX */ 
	GPIOA_MODER |= (1U<<5); /* same thing but for PA2*/ 

	GPIOA_AFRL |= (1U<<14) | (1U<<13) | (1U<<12); /*SET GPIOA PA3 to AF7 for RX (USART2)*/
	GPIOA_AFRL |= (1U<<10) | (1U<<9) | (1U<<8); /*SET GPIOA PA2 to AF7 for TX just to verify contents*/
	
	GPIOA_AFRL &= ~(1U<<11);
	GPIOA_AFRL &= ~(1U<<15); /* Ensure these bits are unset for AF7*/
	
	USART_BRR = 0x8B; /*SET Baud rate to ~115200*/ 
	USART_CR1 |= (1U<<2); /*Enable RX*/ 
	USART_CR1 |= (1U<<3); /*Enable TX*/
	USART_CR1 |= (1U<<13); /*Enable USART*/
	
	USART_CR1 &= ~(1U<<12);   // 8-bit word length (clear M bit)
	USART_CR1 &= ~(1U<<10);   // No parity (clear PCE)  
	USART_CR1 &= ~(1U<<9); 

	char rxbuffer[8];
	memset(rxbuffer, 0, sizeof(rxbuffer));
	int i = 0; 

	while(1){  

		for(i; i < 8; i++){ 
			while(!(USART_SR & (1U<<5))); // if RXNE is NOT SET wait til recieved data ready to be read
			rxbuffer[i] = USART_DR; // recieve from data register
			GPIOA_ODR |= (1U<<5); // verify that were recieving data by blinking led
			if(rxbuffer[i] == '\n'){ 
				i=0; // if delimiter reset 'i'
				break;
			}
			else continue; 
		}

		for (int n = 0; n < 8; n++){
			while(!(USART_SR & (1U<<7))); // while TXE is not set wait
			USART_DR = rxbuffer[n];  // send buffer char by char through data register 
		} 

		memset(rxbuffer, 0, sizeof(rxbuffer)); // clear buffer 
	}
}
