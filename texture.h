#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "util.h"

//--------------------------------------------------------------------------------------
// 3D Texture3D
//--------------------------------------------------------------------------------------
struct Texture3D {
	GLuint tex3d;
	void Create(int rect = 0, float *d = NULL) {
		float *src = d;
		if(d) {
		} else {
			std::vector<float> buf;
			int size = 64;
			int mult = 255 / size;
			for(int k = 0; k < size; k++) {
				for(int j = 0; j < size; j++) {
					for(int i = 0; i < size; i++) {
						unsigned char c = abs((i^j^k) * mult);
						buf.push_back(float(c)/ 256.0);
						buf.push_back(float(c)/ 256.0);
						buf.push_back(float(c)/ 256.0);
						buf.push_back(1.0);
					}
				}
			}
			rect = size;
		}
		glGenTextures(1, &tex3d);
		glBindTexture(GL_TEXTURE_3D, tex3d);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, rect, rect, rect, 0, GL_RGBA, GL_FLOAT, src);
		glBindTexture(GL_TEXTURE_3D, 0);
	}
	
	void Bind() {
		glBindTexture(GL_TEXTURE_3D, tex3d);
	}
	
	void Unbind() {
		glBindTexture(GL_TEXTURE_3D, 0);
	}
};

#endif
