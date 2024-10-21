#include "main.h"
#include <math.h>

uint32_t reset_bit(uint32_t stevilo, int bitNumber){
	stevilo = stevilo & ~(1 << bitNumber);  // premaknemo za bitNumber v levo, in nato negiramo
	return stevilo;
}

uint32_t reset_two_bits(uint32_t stevilo, int bitNumber){

	stevilo = stevilo & ~(1 << bitNumber);
	stevilo = stevilo & ~(1 << (bitNumber + 1));
	/* V eni vrstici: Nastavimo masko na 11 (3), da oba bita resetiramo..
	stevilo = stevilo & ~(3 << bitNumber);
	*/
	return stevilo;
}

uint32_t set_bit(uint32_t stevilo, int bitNumber){
	stevilo = stevilo ^ (1 << bitNumber);
	return stevilo;
}

uint32_t set_two_bits_to(uint32_t stevilo, int bitNumber, int value){
	stevilo = stevilo | (value << bitNumber);
	return stevilo;
}

struct vector{
	int x;
	int y;
	float length;
};


void set_vector_length(struct vector* a) {
    a->length = (float) sqrt(a->x * a->x + a->y * a->y);
}


int main(void){
	uint32_t a;
	uint32_t b;

	a = 0xf;
	b = reset_bit(a, 2); // 0xB
	a = 0xA;
	b = reset_bit(a, 0); // 0xA

	a = 0xFF;
	b = reset_two_bits(a, 3); // 0xE7
	a = 0xB7;
	b = reset_two_bits(a, 3); // 0xA7


	a = 0xB;
	b = set_bit(a, 0); // 0xB
	a = 0xE;
	b = set_bit(a, 2); // 0xE


	a = 0xEF;
	b = set_two_bits_to(a, 3, 1); // 0xEF
	a = 0xB7;
	b = set_two_bits_to(a, 3, 2); // 0xB7

	struct vector c;
	c.x = 4;
	c.y = -2;
	set_vector_length(&c);
	// c.length = 4,47

	}
