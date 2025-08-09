#include <stdint.h> 

#define RCC_OFFSET (0x3800UL)
#define AHB1ENR_OFFSET (0x30UL)
#define GPIOA_OFFSET (0x0000UL) 
#define MODER_OFFSET (0x00UL)
#define ODR_OFFSET (0x14UL)

#define AHB1_BASE (0x40020000UL)
#define RCC_BASE (AHB1_BASE + RCC_OFFSET) 
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)

#define RCC_AHB1ENR (*(volatile uint32_t  *)(RCC_BASE+AHB1ENR_OFFSET)) 
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE+MODER_OFFSET)) 
#define GPIOA_ODR (*(volatile uint32_t *)(GPIOA_BASE+ODR_OFFSET))
#define IWDG_KR (*(volatile uint32_t *)(0x40003000)) 

int main(){

	RCC_AHB1ENR |= (1U<<0); 
	GPIOA_MODER &= ~(3U<<10);
	GPIOA_MODER |= (1U<<10); 
	while(1){ 
		for(volatile int i = 0; i < 10000; i++) {
			GPIOA_ODR |= (1U<<5);
		}
	}
}
