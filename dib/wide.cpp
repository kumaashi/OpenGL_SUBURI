#include <stdio.h>
#include <stdlib.h>

int CreateID(float a)
{
	float k = a / 100.0;
	return int(k);
}

float frand()
{
	return float(rand()) / float(0x7FFF);
}


int main()
{
	for(int i = 0 ; i < 100; i++)
	{
		float k = frand() * 400;
		printf("%d\n", CreateID(k));
	}
}
