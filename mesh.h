//--------------------------------------------------------------------------------------
// Mesh Loader
//--------------------------------------------------------------------------------------
#ifndef _MESH_H_
#define _MESH_H_

#include "warning.h"

#include "util.h"
#include "boundingbox.h"
#include "reader.h"

struct Mesh {
	enum {
		VertexLayout = 0,
		NormalLayout,
		Max,
	};

	GLuint      vbo[Max];
	GLuint      layout;
	GLint       shader;
	BoundingBox bb;
	int         trinum;
	
	void Init() {
		if(trinum) {
			glDeleteBuffers(Max, vbo);
		}
		trinum = 0;
	}
	
	void Load(int p, const char *name) {
		STLBlob stl;
		OBJBlob obj;

		if(strstr(name, "obj")) {
			obj.Load(name);
			return;
		}

		//other stl
		trinum = stl.Load(name);
		if(trinum <= 0) {
			printf("Failed load mesh : %s\n", name);
			return ;
		}
		bb = stl.bb;
		bb.Print();
		printf("%s : %d, vpsize=%d, npsize=%d\n", name, trinum, stl.vp.size(), stl.np.size());
		Create(p, &stl.vp[0], stl.vp.size(), &stl.np[0], stl.np.size());
	}

	void Create(int p, float *v, int vnum, float *n, int nnum) {
		if(!p) return;
		int index = 0;
		int size  = sizeof(float) * 3;
		shader = p;
		
		//Create VAO
		glGenVertexArrays(1, &layout);
		glBindVertexArray(layout);
		
		glGenBuffers(Max, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VertexLayout]);
		glBufferData(GL_ARRAY_BUFFER, vnum * sizeof(float), v, GL_STATIC_DRAW);
		int LocPos = glGetAttribLocation( shader, "pos" );
		glEnableVertexAttribArray(LocPos);
		glVertexAttribPointer(LocPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void *)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NormalLayout]);
		glBufferData(GL_ARRAY_BUFFER, nnum * sizeof(float), n, GL_STATIC_DRAW);
		int LocNor = glGetAttribLocation( shader, "nor" );
		glEnableVertexAttribArray(LocNor);
		glVertexAttribPointer(LocNor, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void *)0);

		glBindVertexArray(0);
	}

	void Begin(int p) {
		shader = p;
		glUseProgram(shader);
		glBindVertexArray(layout);
	}

	void Draw() {
		glDrawArrays(GL_TRIANGLES, 0, trinum * 3);
	}

	void End() {
		glBindVertexArray(0);
		glDisableVertexAttribArray(NormalLayout);
		glDisableVertexAttribArray(VertexLayout);
		glUseProgram(0);
	}

};

#endif
