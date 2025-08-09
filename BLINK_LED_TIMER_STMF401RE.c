#include <stdint.h> 

/*OFFSET */
#define RCC_OFFSET (0x3800UL)

#define AHB1ENR_OFFSET (0x30UL)
#define APB1ENR_OFFSET (0x40UL) 

#define GPIOA_OFFSET (0x0000UL) 
#define MODER_OFFSET (0x00UL)
#define ODR_OFFSET (0x14UL)

#define TIM2_CR1_OFFSET (0x00UL) 
#define TIM2_SR_OFFSET (0x10UL)
#define TIM2_ARR_OFFSET (0x2CUL) 
#define TIM2_PSC_OFFSET (0x28UL)

/*BASE*/
#define AHB1_BASE (0x40020000UL)
#define APB1_BASE (0x40000000UL) 
#define RCC_BASE (AHB1_BASE+RCC_OFFSET) 
#define GPIOA_BASE (AHB1_BASE+GPIOA_OFFSET)

/* PERIPH */
#define RCC_AHB1ENR (*(volatile uint32_t  *)(RCC_BASE+AHB1ENR_OFFSET)) 
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE+APB1ENR_OFFSET))  

#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+MODER_OFFSET)) 
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE+ODR_OFFSET))

#define TIM2_CR1 (*(volatile uint32_t *)(APB1_BASE+TIM2_CR1_OFFSET)) 
#define TIM2_SR (*(volatile uint32_t *)(APB1_BASE+TIM2_SR_OFFSET))
#define TIM2_ARR (*(volatile uint32_t *)(APB1_BASE+TIM2_ARR_OFFSET))
#define TIM2_PSC (*(volatile uint32_t *)(APB1_BASE+TIM2_PSC_OFFSET))


int main(){

	RCC_APB1ENR |= (1U<<0); /* ACTIVATE CLOCK FOR APB BUS*/
	RCC_AHB1ENR |= (1U<<0); /*ACTIVATE CLOCK FOR AHB BUS */ 

	GPIOA_MODER &= ~(3U<<11); /*CLEAR MODE*/
	GPIOA_MODER |= (1U<<10); /*SET AS PIN AS OUTPUT*/

	TIM2_PSC = 16000-1; /*SET PRESCALER*/ 
	TIM2_ARR = (1 * 8000000)/(TIM2_PSC+1) - 1; /*MAX COUNT*/ 
	TIM2_CR1 |= (1U<<0); /*ENABLE BIT 0 OF TIM2_CR1, ACTIVATES CLOCK*/

	while(1){

		while(!(TIM2_SR & (1U<<0))); /* WAIT UNTIL COUNT REACHES MAX (ONCE THE MAX COUNT SET BY ARR IS REACHED, A UIF FLAG WILL BE SENT AND THIS WILL BREAK) */
		GPIOA_ODR^=(1U<<5); /* TURN ON GPIOA PORT 5 (LED) */
		TIM2_SR &= ~(1U<<0); /* CLEAR UIF FLAG */

	}
}



