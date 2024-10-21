#include "main.h"
#include <stdint.h>

#define RCC_AHB4ENR ((volatile uint32_t *) 0x580244E0)

#define GPIOD_MODER ((volatile uint32_t *)0x58020C00)
#define GPIOD_ODR ((volatile uint32_t *)0x58020C14)
#define GPIOC_MODER ((volatile uint32_t *)0x58020800)
#define GPIOC_IDR ((volatile uint32_t *)0x58020810)

int main(void){

	//postavi bit 3 v RCC_AHB4ENR
	// bit 3 = GPIODEN
	// pot tem bo ura naprave prižgana
	// bit 2 = GPIOCEN
	*RCC_AHB4ENR = *RCC_AHB4ENR | (1 << 3) | (1 << 2);

	// najprej pobrišemo dva bita
	// bita 6 in 7 -> 2 * 3 je ker se pomikamo po 2 bita in gre za tretji pin
	/*
	 * Lahko bi napisali to spodaj namesto 3, saj 3 je 11 in pač oba bita brišemo...
	 *
	 * *GPIOD_MODER = *GPIO_MODER & ~(1 << (2 * 3));
	 * *GPIOD_MODER = *GPIO_MODER & ~(2 << (2 * 3));
	 *
	 */

	*GPIOD_MODER = *GPIOD_MODER & ~(3 << (2 * 3));

	// nastavimo bit 6 na 1
	*GPIOD_MODER = *GPIOD_MODER | (1 << (2 * 3));

	// postavimo bit 3 v ODR -> prižgemo LED
	*GPIOD_ODR = *GPIOD_ODR | (1 << 3);
	// ugasnemo ledico
	*GPIOD_ODR = *GPIOD_ODR & ~(1 << 3);

	// gumb = PC13
	// brišemo bita 26 in 27
	*GPIOC_MODER = *GPIOC_MODER &~(3 << (2 * 13));

	while(1){
		uint32_t stanjeGumba = *GPIOC_IDR & (1 << 13);
		if(stanjeGumba){
			*GPIOD_ODR = *GPIOD_ODR | (1 << 3);
		}else{
			*GPIOD_ODR = *GPIOD_ODR & ~(1 << 3);
		}
	}
}
