//--------------------------------------------------------------------------------------
// Mesh Loader
//--------------------------------------------------------------------------------------
#ifndef _MESH_H_
#define _MESH_H_

#include "warning.h"

#include "util.h"
#include "boundingbox.h"
#include "reader.h"
#include "shader.h"

class Mesh {
	enum {
		VertexLayout = 0,
		NormalLayout,
		Max,
	};
	BoundingBox bb;

	int         trinum;
	GLuint      layout;
	GLuint      vbo[Max];
	Shader      *shader;

	void Clear() {
		trinum = 0;
		layout = 0;
		shader = 0;
		memset(vbo, 0, sizeof(vbo));
	}
	
	int CreateBuffer(float *v, int vnum, float *n, int nnum) {
		GL_DEBUG0;
		glGenVertexArrays(1, &layout);
		GL_DEBUG0;
		if(!layout) {
			printf("%s:Cant glGenVertexArrays\n", __FUNCTION__);
			return -1;
		}

		GL_DEBUG0;
		glGenBuffers(Max, vbo);
		GL_DEBUG0;
		glBindVertexArray(layout);
		GL_DEBUG0;
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[VertexLayout]);
			GL_DEBUG0;
			glBufferData(GL_ARRAY_BUFFER, vnum * sizeof(float), v, GL_STATIC_DRAW);
			GL_DEBUG0;
			glBindBuffer(GL_ARRAY_BUFFER, vbo[NormalLayout]);
			GL_DEBUG0;
			glBufferData(GL_ARRAY_BUFFER, nnum * sizeof(float), n, GL_STATIC_DRAW);
			GL_DEBUG0;
		}
		glBindVertexArray(0);
		GL_DEBUG0;
		return 0;
	}

	int SetupProgramAndLayout(GLuint program) {
		if(!program) {
			printf("%s:Invalid program\n", __FUNCTION__);
			return -1;
		}

		printf("Setup program layout : %d\n", program);
		int LocPos = glGetAttribLocation( program, "pos" );
		GL_DEBUG0;
		int LocNor = glGetAttribLocation( program, "nor" );
		GL_DEBUG0;

		glBindVertexArray(layout);
		
		if(LocPos >= 0) {
			GL_DEBUG0;
			glBindBuffer(GL_ARRAY_BUFFER, vbo[VertexLayout]);
			GL_DEBUG0;
			glEnableVertexAttribArray(LocPos);
			GL_DEBUG0;
			glVertexAttribPointer(LocPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void *)0);
			GL_DEBUG0;
		} else {
			printf("ERROR : LocPos = %08X\n", LocPos);
		}

		if(LocPos >= 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[NormalLayout]);
			GL_DEBUG0;
			glEnableVertexAttribArray(LocNor);
			GL_DEBUG0;
			glVertexAttribPointer(LocNor, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void *)0);
			GL_DEBUG0;
		} else {
			printf("ERROR : LocNor = %08X\n", LocNor);
		}
		
		glBindVertexArray(0);
		GL_DEBUG0;
		
		return 0;
	}

public:
	Mesh()  { Clear(); }
	~Mesh() { Unload(); }

	void Unload() {
		if(vbo[VertexLayout]) {
			glDeleteBuffers(Max, vbo);
		}
		if(layout) {
			glDeleteVertexArrays(1, &layout);
		}
		Clear();
	}
	
	int Load(Shader *p, const char *name) {
		Unload();
		STLBlob stl;
		OBJBlob obj;
		if(!p || !name) {
			if(!p)    printf("%s : Invalid Shader\n", __FUNCTION__);
			if(!name) printf("%s : Invalid name\n", __FUNCTION__);
			return -1;
		}

		if(strstr(name, "obj")) {
			obj.Load(name);
			return 0;
		}

		//other stl
		trinum = stl.Load(name);
		if(trinum <= 0) {
			printf("%s:Failed load mesh : %s\n", __FUNCTION__, name);
			return -1;
		}

		bb = stl.bb;
		bb.Print();
		
		//DEBUG
		printf("%s : %d, vpsize=%d, npsize=%d\n", name, trinum, stl.vp.size(), stl.np.size());
		return Create(p, &stl.vp[0], stl.vp.size(), &stl.np[0], stl.np.size());
	}

	int SetShader(Shader *p) {
		if(!p) {
			printf("%s : Invalid Shader\n", __FUNCTION__);
			return -1;
		}
		shader = p;
		
		if(SetupProgramAndLayout(shader->Get())) {
			printf("%s : Failed SetupProgramAndLayout\n", __FUNCTION__);
			return -1;
		}
		printf("%s : Done shader\n", __FUNCTION__);
		return 0;
	}

	int Create(Shader *p, float *v, int vnum, float *n, int nnum) {
		if(!p) {
			printf("%s : Invalid Shader\n", __FUNCTION__);
			return -1;
		}
		if(CreateBuffer(v, vnum, n, nnum)) {
			printf("%s : Failed CreateBuffer\n", __FUNCTION__);
			return -1;
		}
		return SetShader(p);
	}

	void Begin() {
		if(shader) {
			GL_DEBUG0;
			glUseProgram(shader->Get());
			GL_DEBUG0;
			glBindVertexArray(layout);
			GL_DEBUG0;
		} else {
			printf("%s : Not setup Shader\n", __FUNCTION__);
		}
	}

	void Draw() {
		GL_DEBUG0;
		glDrawArrays(GL_TRIANGLES, 0, trinum * 3);
		GL_DEBUG0;
	}

	void End() {
		GL_DEBUG0;
		glBindVertexArray(0);
		GL_DEBUG0;
		glDisableVertexAttribArray(NormalLayout);
		GL_DEBUG0;
		glDisableVertexAttribArray(VertexLayout);
		GL_DEBUG0;
		glUseProgram(0);
		GL_DEBUG0;
	}

};

#endif
