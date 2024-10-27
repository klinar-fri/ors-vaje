#include "main.h"
#include <stdint.h>

#define RCC_AHB4ENR ((volatile uint32_t *)0x580244E0)
/*
#define GPIOD_MODER ((volatile uint32_t *)0x58020C00)
#define GPIOD_OTYPER ((volatile uint32_t *)0x58020C04)
#define GPIOD_OSPEEDR ((volatile uint32_t *)0x58020C08)
#define GPIOD_PUPDR ((volatile uint32_t *)0x58020C0C)
#define GPIOD_IDR ((volatile uint32_t *)0x58020C10)
#define GPIOD_ODR ((volatile uint32_t *)0x58020C14)
#define GPIOD_BSRR ((volatile uint32_t *)0x58020C18)

*/

// makroji za clk_enable
#define GPIOA_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 0))
#define GPIOB_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 1))
#define GPIOC_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 2))
#define GPIOD_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 3))
#define GPIOE_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 4))
#define GPIOF_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 5))
#define GPIOG_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 6))
#define GPIOH_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 7))
#define GPIOI_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 8))
#define GPIOJ_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 9))
#define GPIOK_CLK_ENABLE() (*RCC_AHB4ENR |= (1 << 10))

// s tem structom nam ni treba pisat naslovov kot zgoraj
typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
} GPIO_device;

#define GPIOC ((GPIO_device*) 0x58020800)
#define GPIOD ((GPIO_device*) 0x58020C00)
#define GPIOI ((GPIO_device*) 0x58022000)
#define GPIOJ ((GPIO_device*) 0x58022400)

#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1

void GPIO_Init (GPIO_device* gpio, uint32_t pin, uint32_t mode);
void GPIO_WritePin(GPIO_device* gpio, uint32_t pin, uint32_t value);
uint32_t GPIO_ReadPin(GPIO_device* gpio, uint32_t pin);

int main(){

	// gumb
	// postavimo GPIOCEN
	GPIOC_CLK_ENABLE();

	// LED - PD3
	// postavimo GPIODEN
	GPIOD_CLK_ENABLE();

	//PIN 3 -> a, b, c, d - 3
	// brisemo bita 6 in 7
//	GPIOD->MODER = GPIOD->MODER & ~(3 << (2 * 3));
	// postavimo 6 na 1 in 7 na 0 (01)
//  GPIOD->MODER = GPIOD->MODER | (1 << (2 * 3));

	GPIO_Init(GPIOD, 3, GPIO_MODE_OUTPUT);

	GPIO_Init(GPIOC, 13, GPIO_MODE_INPUT);

	// brisemo bit 3 -> izhod tipa push-pull (0)
	GPIOD->OTYPER = GPIOD->OTYPER & ~(1 << 3);

	// pin 3 -> bita 6 in 7 na 00
	GPIOD->OSPEEDR = GPIOD->OSPEEDR & ~(3 << (2 * 3));

	// No pull down, pull up -> nastavimo na 00
	GPIOD->PUPDR = GPIOD->PUPDR & ~(3 << (2 * 3));

	/*
	// tretji bit na 1 -> prižgemo ledico preko bssr na PD3
	GPIOD->BSRR = 1 << 3;
	// ugasnemo led na PD3, zg 16 bitov je reset
	GPIOD->BSRR = 1 << (3 + 16);
	*/

	GPIO_WritePin(GPIOD, 3, 1);
	GPIO_WritePin(GPIOD, 3, 0);

	while(1){
		if(GPIO_ReadPin(GPIOC, 13)){
			GPIO_WritePin(GPIOD, 3, 1);
		}else{
			GPIO_WritePin(GPIOD, 3, 0);
		}
	}
}

// posplošeno
void GPIO_Init(GPIO_device* gpio, uint32_t pin, uint32_t mode){
	if(mode == GPIO_MODE_INPUT){
		gpio->MODER = gpio->MODER & ~(3 << (2 * pin));
	}else if(mode = GPIO_MODE_OUTPUT){
		gpio->MODER = gpio->MODER & ~(3 << (2 * pin));
		gpio->MODER = gpio->MODER | (1 << (2 * pin));
	}
	/* ALI samo to, + damo mode namesto 1, ker če je mode 1 bo nastavilo na 1, če je 0 se ne bo nič zgodilo
	 * - input = mode 0
	 * - output = mode 1
	gpio->MODER = gpio->MODER & ~(3 << (2 * pin));
	gpio->MODER = gpio->MODER | (mode << (2 * pin));
	*/
}

void GPIO_WritePin(GPIO_device* gpio, uint32_t pin, uint32_t value){
	if(value == 0){
		// reset -> + 16, da pridemo v zgornjih 16 bitov
		gpio->BSRR = 1 << (pin + 16);
	}else{
		// prižgemo
		gpio->BSRR = 1 << pin;
	}
}

uint32_t GPIO_ReadPin(GPIO_device* gpio, uint32_t pin){
	if(gpio->IDR & (1 << pin)){
		return 1;
	}else{
		return 0;
	}
}


