#include <stdint.h>
/*OFFSET */                                                                                                                                                                   
#define RCC_OFFSET (0x3800UL)                                                                                                                                                 
#define AHB1ENR_OFFSET (0x30UL)                                                                                                                                               
#define APB1ENR_OFFSET (0x40UL)                                                                                                                                               

#define GPIOA_OFFSET (0x0000UL) 
#define GPIOB_OFFSET (0x0400UL)                                                                                                                                               
#define MODER_OFFSET (0x00UL)                                                                                                                                                 
#define ODR_OFFSET (0x14UL)                                                                                                                                                   
#define AFRH_OFFSET (0x24UL)
#define SPI2_OFFSET (0x3800UL)
#define SPI2_CR1_OFFSET (0x00UL) 
#define SPI2_SR_OFFSET (0x08UL) 
#define SPI2_DR_OFFSET (0x0CUL) 
/*BASE*/                                                                                                                                                                      
#define AHB1_BASE (0x40020000UL)
#define APB1_BASE (0x40000000UL) 
#define RCC_BASE (AHB1_BASE+RCC_OFFSET) 
#define GPIOB_BASE (AHB1_BASE+GPIOB_OFFSET) 
#define GPIOA_BASE (AHB1_BASE+GPIOA_OFFSET)
#define SPI2_BASE (APB1_BASE+SPI2_OFFSET) 

/* PERIPH */
#define RCC_AHB1ENR (*(volatile uint32_t  *)(RCC_BASE+AHB1ENR_OFFSET)) 
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE+APB1ENR_OFFSET))  

#define SPI2_SR (*(volatile uint32_t *)(SPI2_BASE+SPI2_SR_OFFSET))
#define SPI2_CR1 (*(volatile uint32_t *)(SPI2_BASE+SPI2_CR1_OFFSET)) 		
#define SPI2_DR (*(volatile uint32_t *)(SPI2_BASE+SPI2_DR_OFFSET)) 

#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+MODER_OFFSET))
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE+ODR_OFFSET))

#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE+MODER_OFFSET)) 
#define GPIOB_AFRH (*(volatile uint32_t *)(GPIOB_BASE+AFRH_OFFSET))


int main(){
	

	RCC_AHB1ENR |= (1U<<0); /*Clocks for AHB and APB bus*/ 
	RCC_APB1ENR |= (1U<<17); 
		
	GPIOB_MODER &= ~(3U<<24);
	GPIOB_MODER &= ~(3U<<26); /*Clear fields to set pin mode alternate function*/
	GPIOB_MODER &= ~(3U<<28);
	GPIOB_MODER &= ~(3U<<30);
	
	/*Set MODER for PB12,PB13,PB14, and PB15*/
	GPIOB_MODER |= (1U<<25) | (1U<<27) | (1U<<29) | (1U<<31); 

	/*Set alternate function for above pins to AF5 (SPI2)*/
	GPIOB_AFRH |= (1U<<30) | (1U<<28);  /*PB15*/
	GPIOB_AFRH |= (1U<<26) | (1U<<24);  /*PB14*/
	GPIOB_AFRH |= (1U<<22) | (1U<<20); /*PB13*/
	GPIOB_AFRH |= (1U<<18) | (1U<<16);  /*PB12*/

	SPI2_CR1 &= ~(3U<<11); /*Clear DFF bit to ensure 8-Bit transmission/recieving*/

	SPI2_CR1 |= (1U<<2); /*Enable master configuration*/
	SPI2_CR1 |= (1U<<9); /*Set software slave management (ignore hardware SS pin)*/
	SPI2_CR1 |= (1U<<8);  /*Internal slave select (required when the above option is applied */  

	SPI2_CR1 |= (1U<<6); /*Enable SPI*/

	char message = 'L';
	
	while(SPI2_SR & (1U<<7)); /*While SPI is busy wait*/
	SPI2_DR = message; 
	while(!(SPI2_SR & (1U<<0))); /*While recieve buffer is empty wait*/

	if(SPI2_DR == 'L'){
		GPIOA_ODR |= (1U<<5); /*If recieved data is equal to the message sent turn on the LED*/
	} 
}
