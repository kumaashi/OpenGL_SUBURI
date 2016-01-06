#include <stdio.h>

#define LUAFUNCTION(Masaki) printf( #Masaki )

int main() {
	LUAFUNCTION(CreateWindow);
	return 0;
}

