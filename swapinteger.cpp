#include <iostream>

constexpr auto swapints(size_t x, size_t y) {
	
	/*x ^= y; 5 ^ 1 = 4*/
	/*y ^= x; 1 ^ 4 = 5*/
	/*x ^= y; 4 ^ 5 = 1*/

	__asm {
		mov eax, x
		mov ebx, y
		xor eax, ebx
		xor ebx, eax
		xor eax, ebx
		mov x, eax
		mov y, ebx
	}

	return std::make_pair(x, y);
}

int main() {
	constexpr size_t x = 5;
	constexpr size_t y = 7;

	auto [res, ult] = swapints(x, y);
	printf("x: %d, y: %d\n", res, ult);
}