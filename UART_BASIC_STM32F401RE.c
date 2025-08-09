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

#define USART_SR (*(volatile uint32_t *)(USART2_BASE+USART2_SR_OFFSET))  
#define USART_DR (*(volatile uint32_t *)(USART2_BASE+USART2_DR_OFFSET)) 
#define USART_CR1 (*(volatile uint32_t *)(USART2_BASE+USART2_CR1_OFFSET))
#define USART_BRR (*(volatile uint32_t *)(USART2_BASE+USART2_BRR_OFFSET))

int main(){
	

	RCC_AHB1ENR |= (1U<<0); /*Clocks for AHB and APB bus*/ 
	RCC_APB1ENR |= (1U<<17); 
	
	GPIOA_MODER |= (1U<<5); /*Setting PA2 and PA3 to alternate function mode*/
	GPIOA_MODER |= (1U<<7);

	GPIOA_AFRL |= (1U<<14) | (1U<<13) | (1U<<12); /*Set GPIOA PA3 to AF7 (USART2)*/
	GPIOA_AFRL |= (1U<<10) | (1U<<9) | (1U<<8); /*Set to GPIOA PA2 to AF7 (USART2)*/

	USART_BRR = 0x683; /*Set Baud rate to 9600*/ 
	USART_CR1 |= (1U<<3); /*Enable TX*/ 
	USART_CR1 |= (1U<<13); /*Enable USART*/
	
	char* message = "Hello\r\n"; 

	while(1){
		for(volatile int i = 0; i < 8; i++){ 
			while(!(USART_SR & (1U<<6))); /*While TC (transmission complete) register is not set do nothing*/
			USART_DR = message[i]; 	/*Send message through data register*/
		}
		for(volatile int i = 0; i < 1000000; i++); 
	}
}
