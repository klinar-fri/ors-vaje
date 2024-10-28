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

#define GPIO_OUTPUT_TYPE_PUSH_PULL 0
#define GPIO_OUTPUT_TYPE_OPEN_DRAIN 1

#define GPIO_NO_PULL 0
#define GPIO_PULL_UP 1
#define GPIO_PULL_DOWN 1

#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_HIGH 2
#define GPIO_SPEED_VERY_HIGH 3


void GPIO_Init (GPIO_device* gpio, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd);
void GPIO_WritePin(GPIO_device* gpio, uint32_t pin, uint32_t value);
uint32_t GPIO_ReadPin(GPIO_device* gpio, uint32_t pin);


void delay(uint32_t stevilo){
	while(stevilo > 0){
		stevilo--;
	}
}

int main(){

	// clk enable
	GPIOC_CLK_ENABLE();
	GPIOD_CLK_ENABLE();
	GPIOI_CLK_ENABLE();
	GPIOJ_CLK_ENABLE();

	//PIN 3 -> a, b, c, d - 3
	// brisemo bita 6 in 7
//	GPIOD->MODER = GPIOD->MODER & ~(3 << (2 * 3));
	// postavimo 6 na 1 in 7 na 0 (01)
//  GPIOD->MODER = GPIOD->MODER | (1 << (2 * 3));


	// pin PD3 IN PI13 inicializiramo kot izhod push-pull brez uporov
	GPIO_Init(GPIOD, 3, GPIO_MODE_OUTPUT, GPIO_OUTPUT_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NO_PULL);

	GPIO_Init(GPIOI, 13, GPIO_MODE_OUTPUT, GPIO_OUTPUT_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NO_PULL);

	// pin PJ2 kot izhod open-drain s pull-up
	GPIO_Init(GPIOJ, 2, GPIO_MODE_OUTPUT, GPIO_OUTPUT_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_UP);

	// PIN PC13
	GPIO_Init(GPIOC, 13, GPIO_MODE_INPUT, GPIO_OUTPUT_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NO_PULL);


	//najprej vse tri ugasnene
	GPIO_WritePin(GPIOD, 3, 0);
	GPIO_WritePin(GPIOJ, 2, 1); // 1 na izhodu, da sta ugasnene
	GPIO_WritePin(GPIOI, 13, 1);


	while(1){
		if(GPIO_ReadPin(GPIOC, 13)){
			GPIO_WritePin(GPIOD, 3, 1); // on 1
			delay(5000000);
			GPIO_WritePin(GPIOD, 3, 0); // off 1
			GPIO_WritePin(GPIOJ, 2, 0); // on 2
			delay(5000000);
			GPIO_WritePin(GPIOJ, 2, 1); // off 2
			GPIO_WritePin(GPIOI, 13, 0); // on 3
			delay(5000000);
			GPIO_WritePin(GPIOI, 13, 1); // off 3
			GPIO_WritePin(GPIOJ, 2, 0); // on 2
			delay(5000000);
			GPIO_WritePin(GPIOJ, 2, 1); // off 2
			GPIO_WritePin(GPIOD, 3, 1); // on 1
			delay(5000000);
		}else{
			GPIO_WritePin(GPIOD, 3, 0); // off PD3
			GPIO_WritePin(GPIOI, 13, 1); // off PI13
			GPIO_WritePin(GPIOJ, 2, 1); // off PJ2
		}
	}
}

// sam funkcija da ni treba 2x pisat, brez mode
void GPIO_Output_Input(GPIO_device* gpio, uint32_t pin, uint32_t otype, uint32_t ospeed, uint32_t pupd){
	if(otype == GPIO_OUTPUT_TYPE_PUSH_PULL){
		// reset pin da je push-pull
		gpio->OTYPER = gpio->OTYPER & ~(1 << pin);
	}else if(otype == GPIO_OUTPUT_TYPE_OPEN_DRAIN){
		// set pin na 1 da je open-drain
		gpio->OTYPER = gpio->OTYPER | (1 << pin);
	}else if(ospeed == GPIO_SPEED_LOW){
		// 00 -> low speed, zato reset, več ne rabimo
		gpio->OSPEEDR = gpio->OSPEEDR & ~(3 << (2 * pin));
	}else if(pupd == GPIO_NO_PULL){
		// No pull down, pull up -> nastavimo na 00
		gpio->PUPDR = gpio->PUPDR & ~(3 << 2 * pin);
	}else if(pupd == GPIO_PULL_UP){
		// pull up -> 01 (1), set en bit na 1 drug ostane na 0
		gpio->PUPDR = gpio->PUPDR | (1 << 2 * pin);
	}
}

// posplošeno
void GPIO_Init(GPIO_device* gpio, uint32_t pin, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd){
	if(mode == GPIO_MODE_INPUT){
		gpio->MODER = gpio->MODER & ~(3 << (2 * pin));
		GPIO_Output_Input(gpio, pin, otype, ospeed, pupd);
	}else if(mode == GPIO_MODE_OUTPUT){
		gpio->MODER = gpio->MODER & ~(3 << (2 * pin));
		gpio->MODER = gpio->MODER | (1 << (2 * pin));
		GPIO_Output_Input(gpio, pin, otype, ospeed, pupd);
	}

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


