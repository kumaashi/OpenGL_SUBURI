#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>

//--------------------------------------------------------------------------------------
// BoundingBox
//--------------------------------------------------------------------------------------
struct BoundingBox {
	enum {
		Max = 99999999 //todo std::honyarara
	};
	float bmax[3];
	float bmin[3];

	void Init() {
		for(int i = 0 ; i < 3; i++) bmax[i] = -Max;
		for(int i = 0 ; i < 3; i++) bmin[i] =  Max;
	}

	void GetParam(float *p) {
		for(int i = 0; i < 3; i++) {
			if(bmax[i] < p[i]) bmax[i] = p[i];
			if(bmin[i] > p[i]) bmin[i] = p[i];
		}
	}

	void Print() {
		printf("\n-----------------------------------\n");
		for(int i = 0;  i < 3; i++) {
			const char ch[] = {'X', 'Y', 'Z'};
			printf("%c : min=%3.5f, max=%3.5f\n", ch[i], bmin[i], bmax[i]);
		}
		printf("-----------------------------------\n");
	}
};

#endif
