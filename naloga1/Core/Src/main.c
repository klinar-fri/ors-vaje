#include "main.h"
#include <stdint.h>

#define RCC_AHB4ENR ((volatile uint32_t *) 0x580244E0)

// led PD3
#define GPIOD_MODER ((volatile uint32_t *) 0x58020C00)
#define GPIOD_ODR ((volatile uint32_t *) 0x58020C14)
// modri gumb
#define GPIOC_MODER ((volatile uint32_t *) 0x58020800)
#define GPIOC_IDR ((volatile uint32_t *) 0x58020810)

// define led PJ2
#define GPIOJ_MODER ((volatile uint32_t *) 0x58022400)
#define GPIOJ_ODR ((volatile uint32_t *) 0x58022414)

// define led PI13
#define GPIOI_MODER ((volatile uint32_t *) 0x58022000)
#define GPIOI_ODR ((volatile uint32_t *) 0x58022014)

void delay(uint32_t stevilo){
	while(stevilo > 0){
		stevilo--;
	}
}

int main(void){


	// bit 3 = GPIOD-EN-clock
	// bit 2 = GPIOC-EN-clock
	// bit 9 = GPIOJ-EN-clock
	// bit 8 = GPIOI-EN-clock
	// po tem bo ura naprav prižgana

	*RCC_AHB4ENR = *RCC_AHB4ENR | (1 << 3) | (1 << 2) | (1 << 9) | (1 << 8);

	// pobrišemo bita 6 in 7 in nastavimo enico na bit 6 -> pin 3
	*GPIOD_MODER = *GPIOD_MODER & ~(3 << (2 * 3));
	*GPIOD_MODER = *GPIOD_MODER | (1 << (2 * 3));

	// pri ostalih dveh samo pobrišemo
	// pin 2 -> GPIOJ -> pobrišemo bita 4 in 5
	// pin 13 -> GPIOI -> pobrišemo bita 26 in 27
	*GPIOJ_MODER = *GPIOJ_MODER & ~(3 << (2 * 2));
	*GPIOI_MODER = *GPIOI_MODER & ~(3 << (2 * 13));

	*GPIOJ_MODER = *GPIOJ_MODER | (1 << (2 * 2));
	*GPIOI_MODER = *GPIOI_MODER | (1 << (2 * 13));


	// gumb = PC13
	// brišemo bita 26 in 27
	*GPIOC_MODER = *GPIOC_MODER & ~(3 << (2 * 13));


	while(1){
		uint32_t stanjeGumba = *GPIOC_IDR & (1 << 13);
		if(stanjeGumba){
		    *GPIOD_ODR = *GPIOD_ODR | (1 << 3); // on 1
			delay(5000000);
			*GPIOD_ODR = *GPIOD_ODR & ~(1 << 3); // off 1
			*GPIOJ_ODR = *GPIOJ_ODR & ~(1 << 2); // on 2
			delay(5000000);
			*GPIOJ_ODR = *GPIOJ_ODR | (1 << 2); // off 2
			*GPIOI_ODR = *GPIOI_ODR & ~(1 << 13); // on 3
			delay(5000000);
			*GPIOI_ODR = *GPIOI_ODR | (1 << 13); // off 3
			*GPIOJ_ODR = *GPIOJ_ODR & ~(1 << 2); // on 2
			delay(5000000);
			*GPIOJ_ODR = *GPIOJ_ODR | (1 << 2); // off 2
			*GPIOD_ODR = *GPIOD_ODR | (1 << 3); // on 1
			delay(5000000);
		}else{
			// vse ugasnene
			*GPIOD_ODR = *GPIOD_ODR & ~(1 << 3);
			*GPIOJ_ODR = *GPIOJ_ODR | (1 << 2);
			*GPIOI_ODR = *GPIOI_ODR | (1 << 13);
		}
	}
}
