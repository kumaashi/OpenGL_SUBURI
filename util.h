//--------------------------------------------------------------------------------------
// util.h
//--------------------------------------------------------------------------------------
#ifndef _UTIL_H_
#define _UTIL_H_

//Windows
#include <windows.h>
#include <windowsX.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <mmsystem.h>

//OpenGL
#include <gl/gl.h>
#include <gl/glext.h>

//STL
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

//TEKITOU
#pragma  comment(lib, "gdi32.lib")
#pragma  comment(lib, "winmm.lib")
#pragma  comment(lib, "user32.lib")
#pragma  comment(lib, "opengl32.lib")

#pragma warning ( disable : 4313 )
#pragma warning ( disable : 4477 )
#pragma warning ( disable : 4838 )

//type w
#pragma warning ( disable : 4311 ) //todo
#pragma warning ( disable : 4312 ) //todo

#define  DEFAULT_WIDTH   (640)
#define  DEFAULT_HEIGHT  (480)

//HA?
#define _CRT_SECURE_NO_WARNINGS
#define sscanf sscanf_s
#define sprintf sprintf_s


//DEBUG
#define GL_DEBUG       printf("%08d%s:%08d : glErr:%08X\n", __LINE__, __FUNCTION__, __LINE__, glGetError())
#define GL_DEBUGEx(x)  printf("%08d%s:%08d : glErr:%08X %08X\n", __LINE__, __FUNCTION__, __LINE__, glGetError(), x)

//--------------------------------------------------------------------------------------
// OpenGL Function
//--------------------------------------------------------------------------------------
extern PFNGLDEBUGMESSAGECALLBACKPROC           glDebugMessageCallback      ;
extern PFNGLCREATEPROGRAMPROC                  glCreateProgram             ;
extern PFNGLCREATESHADERPROC                   glCreateShader              ;
extern PFNGLSHADERSOURCEPROC                   glShaderSource              ;
extern PFNGLCOMPILESHADERPROC                  glCompileShader             ;
extern PFNGLATTACHSHADERPROC                   glAttachShader              ;
extern PFNGLDELETESHADERPROC                   glDeleteShader              ;
extern PFNGLLINKPROGRAMPROC                    glLinkProgram               ;
extern PFNGLUSEPROGRAMPROC                     glUseProgram                ;
extern PFNGLUNIFORM1FPROC                      glUniform1f                 ;
extern PFNGLUNIFORM1IPROC                      glUniform1i                 ;
extern PFNGLUNIFORM4FVPROC                     glUniform4fv                ;
extern PFNGLUNIFORMMATRIX4FVPROC               glUniformMatrix4fv          ;
extern PFNGLGETUNIFORMLOCATIONPROC             glGetUniformLocation        ;
extern PFNGLGENBUFFERSPROC                     glGenBuffers                ;
extern PFNGLDELETEBUFFERSPROC                  glDeleteBuffers             ;
extern PFNGLBINDBUFFERPROC                     glBindBuffer                ;
extern PFNGLBINDBUFFERBASEPROC                 glBindBufferBase            ;
extern PFNGLBUFFERDATAPROC                     glBufferData                ;
extern PFNGLGETBUFFERSUBDATAPROC               glGetBufferSubData          ;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC      glTransformFeedbackVaryings ;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC         glBeginTransformFeedback    ;
extern PFNGLENDTRANSFORMFEEDBACKPROC           glEndTransformFeedback      ;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC        glEnableVertexAttribArray   ;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC       glDisableVertexAttribArray  ;
extern PFNGLVERTEXATTRIBPOINTERPROC            glVertexAttribPointer       ;
extern PFNGLGETSHADERINFOLOGPROC               glGetShaderInfoLog          ;
extern PFNGLGETSHADERIVPROC                    glGetShaderiv               ;
extern PFNGLTEXIMAGE3DPROC                     glTexImage3D                ;
extern PFNGLPROGRAMUNIFORM1IPROC               glProgramUniform1i          ;
extern PFNGLACTIVETEXTUREPROC                  glActiveTexture             ;
extern PFNGLGETPROGRAMINFOLOGPROC              glGetProgramInfoLog         ;
extern PFNGLGETATTRIBLOCATIONPROC              glGetAttribLocation         ;
extern PFNGLBINDVERTEXARRAYPROC                glBindVertexArray           ;
extern PFNGLBINDATTRIBLOCATIONPROC             glBindAttribLocation        ;
extern PFNGLGENVERTEXARRAYSPROC                glGenVertexArrays           ;
extern PFNGLGENFRAMEBUFFERSPROC                glGenFramebuffers           ;
extern PFNGLBINDFRAMEBUFFERPROC                glBindFramebuffer           ;
extern PFNGLGENRENDERBUFFERSPROC               glGenRenderBuffers          ;
extern PFNGLBINDRENDERBUFFERPROC               glBindRenderBuffer          ;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC        glFramebufferRenderbuffer   ;
extern PFNGLRENDERBUFFERSTORAGEPROC            glRenderBufferStorage       ;
extern PFNGLFRAMEBUFFERTEXTUREPROC             glFramebufferTexture        ;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC           glFramebufferTexture2D      ;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC         glCheckFramebufferStatus    ;
extern PFNGLDRAWBUFFERSPROC                    glDrawBuffers               ;
extern PFNGLGENERATEMIPMAPPROC                 glGenerateMipmap            ;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC          glTexImage2DMultisample     ;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
extern PFNGLBLITFRAMEBUFFERPROC                glBlitFramebuffer           ;
//extern PFNGLDEBUGMESSAGECALLBACKPROC           glDebugMessageCallback      ;

extern  BOOL  Windowed;
extern  int   Width   ;
extern  int   Height  ;


//--------------------------------------------------------------------------------------
// math
//--------------------------------------------------------------------------------------
#ifndef PI
#define  PI  3.141592653
#endif

#define d2r(x)  (((x) * PI * 2) / 180.0)

inline float clamp(float x, float a, float b) {
	return (std::min)((std::max)(x, a), b);
}

inline float lerp(float x, float y, float a) {
	return x * (1.0 - a) + y * a;
}

inline float mix(float x, float y, float a) {
	return lerp(x, y, a);
}

inline float lerpc(float a, float b, float x) { 
	float ft = x * PI;
	float  f = (1.0 - cos(ft)) * 0.5;
	return a * (1.0 - f) + b * f;
}

inline float smoothstep(float e0, float e1, float x) {
	float t = clamp((x - e0) / (e1 - e0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}


//--------------------------------------------------------------------------------------
// vec
//--------------------------------------------------------------------------------------
#define _VEC3_DEFOP_(o)  vec operator o (const vec &a) { return vec(x o a.x, y o a.y, z o a.z, w o a.w); }
__declspec(align(256)) struct vec {
	union {
		struct { float x, y, z, w; };
		float data[4];
	};

	void print() {
		printf("%08X: %5.4f %5.4f %5.4f %5.4f\n", (unsigned long )this, x, y, z, w);
	}

	vec() {
		x = y = z = 0;
		w = 1;
	}

	vec(float a) {
		x = y = z = a;
		w = 1;
	}

	vec(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
		w = 1;
	}

	vec(float a, float b, float c, float d) {
		x = a;
		y = b;
		z = c;
		w = d;
	}

	float dot(vec &a) {
		return x * a.x + y * a.y + z * a.z;
	}

	float dot() {
		return dot(*this);
	}

	float length()	{
		return length(*this);
	}

	float length(vec &a)	{
		return sqrt(a.dot(a));
	}

	vec cross(vec &a, vec &b) {
		return vec(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	vec cross(vec &b) {
		return cross(*this, b);
	}

	vec normalize() {
		float l = 1.0 / length();
		x *= l;
		y *= l;
		z *= l;
		return *this;
	}

	vec smoothstep(vec &a, vec &b, float k) {
		float (*fn)(float, float, float) = ::smoothstep;
		/*
		x = fn(a.x, b.x, k);
		y = fn(a.y, b.y, k);
		z = fn(a.z, b.z, k);
		*/
		float bk  = fn(1,0,k);
		float ibk = 1 - bk;
		x = (bk * a.x) + (ibk * b.x);
		y = (bk * a.y) + (ibk * b.y);
		z = (bk * a.z) + (ibk * b.z);
		return *this;
	}

	vec lerp(vec &a, vec &b, float k) {
		float (*fn)(float, float, float) = ::lerp;
		x = fn(a.x, b.x, k);
		y = fn(a.y, b.y, k);
		z = fn(a.z, b.z, k);
		return *this;
	}

	vec lerpc(vec &a, vec &b, float k) {
		float (*fn)(float, float, float) = ::lerpc;
		x = fn(a.x, b.x, k);
		y = fn(a.y, b.y, k);
		z = fn(a.z, b.z, k);
		return *this;
	}

	
	_VEC3_DEFOP_( +)	 _VEC3_DEFOP_( -)	 _VEC3_DEFOP_( *)	 _VEC3_DEFOP_( /) 
	_VEC3_DEFOP_(+=)	 _VEC3_DEFOP_(-=)	 _VEC3_DEFOP_(*=)	 _VEC3_DEFOP_(/=) 
	
};


//--------------------------------------------------------------------------------------
// Matrix
//--------------------------------------------------------------------------------------
struct Matrix {
	float data[16];
	
	void Print() {
		printf("======================================================");
		for(int i = 0 ; i < 16; i++) {
			if( (i % 4) == 0) printf("\n");
			printf("%5.3f, ", data[i]);
		}
		printf("\n======================================================\n");
	}

	float *Ident() {
		float *d = data;
		memset(d, 0, sizeof(float) * 16);
		d[0]	= 1.0;
		d[5]	= 1.0;
		d[10] = 1.0;
		d[15] = 1.0;
		return d;
	}
	


	float *Scale(float x, float y, float z) {
		float *d = data;
		d[0]	= x;
		d[5]	= y;
		d[10] = z;
		d[15] = 1.0;
		return d;
	}

	float *Trans(float x, float y, float z) {
		float *d = data;
		d[12] = x;
		d[13] = y;
		d[14] = z;
		d[15] = 1.0;
		return d;
	}

	float *Perspective(float fovy, float aspect, float fnear, float ffar) {
		float *d = data;
		Ident();
		float t = fnear * tan(fovy * 3.141592653 / 360.0);
		float r = t * aspect;
		float a = r * 2;
		float b = t * 2;
		float c = ffar - fnear;
		d[0]    = fnear * 2 / a;
		d[5]    = fnear * 2 / b;
		d[10]   = -(ffar + fnear) / c;
		d[11]   = -1;
		d[14]   = -(ffar * fnear * 2) / c;
		return d;
	};
	
	float *LookAt(vec &eye, vec &center, vec &up) {
		LookAt((float *)&eye, (float *)&center, (float *)&up);
		return (float *)this;
	}

	float *LookAt(float *eye, float *center, float *up) {
		float *d = data;
		Ident();
		float eyeX    = eye[0],    eyeY    = eye[1],    eyeZ    = eye[2];
		float upX     = up[0],     upY     = up[1],     upZ     = up[2];
		float centerX = center[0], centerY = center[1], centerZ = center[2];
		float x0, x1, x2, y0, y1, y2, z0, z1, z2, l;
		z0 = eyeX - center[0];
		z1 = eyeY - center[1];
		z2 = eyeZ - center[2];
		l = 1.0 / sqrt(z0 * z0 + z1 * z1 + z2 * z2);
		z0 *= l; z1 *= l; z2 *= l;
		
		//cross
		x0 = upY * z2 - upZ * z1;
		x1 = upZ * z0 - upX * z2;
		x2 = upX * z1 - upY * z0;
		l = sqrt(x0 * x0 + x1 * x1 + x2 * x2);
		if(!l){
		  x0 = 0; x1 = 0; x2 = 0;
		} else {
		  l = 1.0 / l;
		  x0 *= l; x1 *= l; x2 *= l;
		}
		
		//cross
		y0 = z1 * x2 - z2 * x1;
		y1 = z2 * x0 - z0 * x2;
		y2 = z0 * x1 - z1 * x0;
		l = sqrt(y0 * y0 + y1 * y1 + y2 * y2);
		if(!l) {
		  y0 = 0; y1 = 0; y2 = 0;
		} else {
		  l = 1 / l;
		  y0 *= l; y1 *= l; y2 *= l;
		}
		d[0]  = x0; d[1] = y0; d[2]  = z0; d[3]  = 0;
		d[4]  = x1; d[5] = y1; d[6]  = z1; d[7]  = 0;
		d[8]  = x2; d[9] = y2; d[10] = z2; d[11] = 0;
		d[12] = -(x0 * eyeX + x1 * eyeY + x2 * eyeZ);
		d[13] = -(y0 * eyeX + y1 * eyeY + y2 * eyeZ);
		d[14] = -(z0 * eyeX + z1 * eyeY + z2 * eyeZ);
		d[15] = 1;
		return d;
	}

	float *RotX(float a) {
		float *d = data;
		Ident();
		d[5]  =  cos(a);
		d[6]  = -sin(a);
		d[9]  =  sin(a);
		d[10] =  cos(a);
	}
	
	float *RotY(float a) {
		float *d = data;
		Ident();
		d[0]  =  cos(a);
		d[2]  =  sin(a);
		d[8]  = -sin(a);
		d[10] =  cos(a);
	}

	float *RotZ(float a) {
		float *d = data;
		Ident();
		d[0]  =  cos(a);
		d[1]  = -sin(a);
		d[4]  =  sin(a);
		d[5]  =  cos(a);
	}
	
	Matrix operator * (const Matrix &x) {
		Matrix ret;
		float  *a = (float *)&x;
		float  *b = (float *)this;
		float  *c = (float *)&ret;
		int idx = 0;
		for(int w = 0 ; w < 4; w++) {
			for(int j = 0 ; j < 4; j++) {
				c[idx] = 0;
				for(int i = 0 ; i < 4; i++) {
					c[idx] += a[i + w * 4] * b[i * 4 + j];
				}
				idx++;
			}
		}
		return ret;
	}
};

//--------------------------------------------------------------------------------------
// Filereader
//--------------------------------------------------------------------------------------
struct File {
	std::vector<unsigned char>       buf;
	std::vector<char>                sep;
	char *ptr;

	bool Empty() { return buf.empty(); }
	int Size()   { return buf.size(); }
	unsigned char *Buf()  { return &buf[0]; }

	void Clear() {
		buf.clear();
		sep.clear();
	}

	int Open(const char *name, bool il) {
		Clear();
		char *mode = "rb";
		if(!name) return -1;
		if(il) mode = "r";

		//FILE *fp = fopen(name, "rb");
		FILE *fp = NULL;
		fopen_s(&fp, name, "rb"); //medoi, atoe fopen;
		if(!fp) return -1;

		fseek(fp, 0, SEEK_END);
		buf.resize(ftell(fp));
		fseek(fp, 0, SEEK_SET);

		memset(&buf[0], 0, buf.size());
		fread(&buf[0], 1, buf.size(), fp);
		fclose(fp);

		if(il) buf.push_back(0);
		ptr = (char *)&buf[0];

		//printf("DEBUG : %s -> bufsize = %d\n", name, buf.size());
		return buf.size();
	}

	char *Line(const char c = '\n') {
		if(buf.empty()) return NULL;
		if(ptr) if(*ptr == 0) return NULL;
		sep.clear();
		while(*ptr && *ptr != c) {
			sep.push_back(*ptr++);
		}
		ptr++;
		sep.push_back(0);
		return &sep[0];
	}
};


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

//--------------------------------------------------------------------------------------
// STL Loader
//--------------------------------------------------------------------------------------
struct STLBlob {
	#pragma pack(push,1)
	struct stldata {
		float n[3];
		float p[9];
		unsigned short reserve;
		void Print() {
			for(int i = 0 ; i < 9; i++) {
				if( (i % 3) == 0) printf("\n");
					printf("P%04d:%f, ", i, n[i]);
			}
		}
	};
	#pragma pack(pop)
	
	std::vector<float> vp;
	std::vector<float> np;
	
	//bb
	BoundingBox bb;

	int Load(const char *name) {
		File f;
		int size = f.Open(name, false);
		if(size <= 0) return size;
		printf("%s -> size = %d\n", name, size);
		unsigned char *buf = (unsigned char *)f.Buf();

		//Parse BIN STL
		int index          = 80;
		int trimax         = *(int *)(&buf[index]);
		index             += 4;
		int cnt            = 0;
		printf("trimax = %d\n", trimax);
		bb.Init();
		while(cnt < trimax) {
			stldata d = *(stldata *)(&buf[index]); 
			for(int j = 0 ; j < 3; j++) {
					for(int i = 0 ; i < 3; i++) { np.push_back(d.n[i]); }
			}
			for(int i = 0 ; i < 9; i++) { vp.push_back(d.p[i]); }
			for(int i = 0; i < 9; i += 3) { bb.GetParam(&d.p[i]); }
			index += sizeof(stldata);
			cnt++;
		}
		return trimax;
	}
};

//--------------------------------------------------------------------------------------
// OBJ Loader (line)
//--------------------------------------------------------------------------------------
struct OBJBlob {
	std::vector<float> vp;
	std::vector<int>   vi;
	int Load(const char *name) {
		File f;
		int size = f.Open(name, true);
		if(size <= 0) return size;
		printf("%s -> size = %d\n", name, size);
		char *s;

		//parse obj for line
		while(s = f.Line()) {
			while(isspace(*s)) s++;
			char type = *s++;
			if(type == 'v') {
				float x, y, z;
				sscanf(s, "%f %f %f", &x, &y, &z);
				vp.push_back(x);
				vp.push_back(y);
				vp.push_back(z);
				continue;
			}
			if(type == 'f') {
				std::vector<char> temp;
				while(*s) {
					char c = *s++;
					if(isspace(c) || isdigit(c)) {
						temp.push_back(c);
						continue;
					}
					if(c == '/' && temp.size()) {
						temp.push_back(0);
						int n = strtol(&temp[0], 0, 10) - 1; //w
						vi.push_back(n);
						temp.clear();

						//discard tex and normal
						while(!isspace(*s)) s++;
					}
				}
				continue;
			}
		}
		printf("vp=%d, vi=%d\n", vp.size(), vi.size());
		return 0;
	}
};


//--------------------------------------------------------------------------------------
// Mesh Loader
//--------------------------------------------------------------------------------------
struct Mesh {
	enum {
		Vertex = 0,
		Normal,
		Max,
	};
	GLuint      vbo[Max];
	GLint       shader;
	BoundingBox bb;
	int         trinum;
	
	void Init() {
		if(trinum) {
			glDeleteBuffers(Max, vbo);
		}
		trinum = 0;
	}
	
	void Load(const char *name) {
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
		Create(&stl.vp[0], stl.vp.size(), &stl.np[0], stl.np.size());
	}
	
	void SetShader(int p) {
		if(p) {
			shader = p;
			//GL_DEBUG;
			//nAttLocPos = glGetAttribLocation( shader, locpos );
			//nAttLocNor = glGetAttribLocation( shader, locnor );
			//GL_DEBUG;
			//printf("%d\n", nAttLocPos);
			//printf("%d\n", nAttLocNor);
		}
	}
	
	void Create(float *v, int vnum, float *n, int nnum) {
		glGenBuffers(Max, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[Vertex]);
		glBufferData(GL_ARRAY_BUFFER, vnum * sizeof(float), v, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[Normal]);
		glBufferData(GL_ARRAY_BUFFER, nnum * sizeof(float), n, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		printf("%s : vnum=%d, nnum=%d\n", __FUNCTION__, vnum, nnum);
	}

	void Begin() {
		int offset = 0;
		const char *locpos = "pos";
		const char *locnor = "nor";
		glUseProgram(shader);
		glBindAttribLocation(shader, Vertex, locpos);
		glBindAttribLocation(shader, Normal, locnor);

		//DEBUG
		printf("shader = %d\n", shader);
		printf("vbo(Pos) = %d\n", vbo[Vertex]);
		printf("vbo(Nor) = %d\n", vbo[Normal]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[Vertex]);
		glEnableVertexAttribArray(Vertex);
		glVertexAttribPointer(Vertex, 3, GL_FLOAT, GL_FALSE, 0, (const void *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[Normal]);
		glEnableVertexAttribArray(Normal);
		glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, 0, (const void *)NULL);
	}

	void Draw() {
		glDrawArrays(GL_TRIANGLES, 0, trinum * 3);
	}

	void End() {
		/*
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(Normal);
		glDisableVertexAttribArray(Vertex);
		*/
		glUseProgram(0);
		GL_DEBUG;
	}

};


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


//------------------------------------------------------------------------------------------
// RenderTarget
//------------------------------------------------------------------------------------------
struct RenderTarget {
	GLuint fbo;
	GLuint fbo2;
	GLuint rttex;
	GLuint rttex2;
	GLuint rbo;
	int Width, Height, Sample;

	void glSetupState() {
		struct Param {
			GLenum pname;
			GLint  param;
		};
		Param param[] = {
			//{ GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST},
			{ GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST},
			{ GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE},
			{ GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE},
			{ GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE},
			{ GL_GENERATE_MIPMAP,    GL_FALSE},
			{ 0, 0},
		};
		for(int i = 0 ; param[i].pname; i++) {
			Param *w = &param[i];
			glTexParameteri(GL_TEXTURE_2D, w->pname, w->param);
			GL_DEBUG;
		}
	}
	
	void Create(int w, int h, int ms = 8) {
		printf("%s: Width=%d, Height=%d, Ms=%d\n", __FUNCTION__, w, h, ms);
		int status = 0;
		glGenTextures(1, &rttex);  //1ST Create Texture
		GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rttex);
		GL_DEBUG;
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
		GL_DEBUG;
		glSetupState();
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_DEBUG;
		
		glGenFramebuffers(1, &fbo);
		GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GL_DEBUG;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, rttex, 0);
		GL_DEBUG;
		
		glGenRenderBuffers(1, &rbo);
		GL_DEBUG;
		glBindRenderBuffer(GL_RENDERBUFFER, rbo);
		GL_DEBUG;
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_DEPTH_COMPONENT32, w, h);
		GL_DEBUG;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		GL_DEBUG;
		if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			printf("Bind Failed : status=%08X\n", status);
		} else {
			printf("Bind OK : %d, %d\n", rttex, fbo);
		}
		glBindRenderBuffer(GL_RENDERBUFFER, 0);
		GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_DEBUG;

		//2ND
		glGenTextures(1, &rttex2);
		GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, rttex2);
		GL_DEBUG;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
		GL_DEBUG;
		//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
		glSetupState();
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_DEBUG;

		glGenFramebuffers(1, &fbo2);
		GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
		GL_DEBUG;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rttex2, 0);
		GL_DEBUG;
		if( (status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			printf("Bind Failed : status=%08X\n", status);
		} else {
			printf("Bind OK : %d, %d\n", rttex2, fbo2);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_DEBUG;
		printf("%s, DONE\n", __FUNCTION__);

		Width  = w;
		Height = h;
		Sample = ms;
	}

	void SetTexture() {
		glActiveTexture(GL_TEXTURE0);
		GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, rttex2);
		GL_DEBUG;
		glGenerateMipmap(GL_TEXTURE_2D);
		GL_DEBUG;
	}
	
	void UnsetTexture() {
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_DEBUG;
	}

	void Begin() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GL_DEBUGEx(fbo);
	}

	void End() {
		Resolve();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_DEBUG;
	}

	void Resolve() {
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo);
		GL_DEBUG;
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo2);
		GL_DEBUG;
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		GL_DEBUG;
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0);
		GL_DEBUG;
		glBindFramebuffer( GL_READ_FRAMEBUFFER, 0);
		GL_DEBUG;
	}
};



//--------------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------------
struct Camera {
	Matrix view;
	Matrix proj;
	vec  Pos, At, Up;
	vec  PosStart;
	vec  PosEnd;
	float  tt;
	float dtt;
	int Width, Height;
	int state;
	float ffov, fnear, ffar;

	void SetScreen(int w, int h) {
		Width  = w;
		Height = h;
	}

	void SetView(vec &p, vec &a, vec &u) {
		if(!state) {
			Pos = p;
		}
		At  = a;
		Up  = u;
	}

	void SetProj(float fov, float fn, float ff) { ffov = fov, fnear = fn, ffar = ff; }
	float *GetView() { return (float *)&view; }
	float *GetProj() { return (float *)&proj; }

	void SetTracking(vec &p, float dt, int ty = 0) {
		Reset();
		PosStart = Pos;
		PosEnd   = p;
		dtt      = dt;
		state    = 1;
	}
	
	void Reset() { state = 0; tt    = 0; }
	void Update() {
		view.LookAt(Pos, At, Up);
		proj.Perspective(ffov, float(Width) / float(Height), fnear, ffar);
		if(state) {
			Pos.smoothstep(PosStart, PosEnd, tt);
			tt += dtt;
			if(tt > 1.0) {
				Pos   = PosEnd;
				tt    = 1.0;
				state = 0;
			}
		}
	}
};


//--------------------------------------------------------------------------------------
// OS Function
//--------------------------------------------------------------------------------------
BOOL ProcMsg();
int Init(int argc, char *argv[], void (*StartMain)(int argc, char *argv[], HDC hdc));

//--------------------------------------------------------------------------------------
// MISC Function
//--------------------------------------------------------------------------------------
int random();
float frand();
void show_fps();

//--------------------------------------------------------------------------------------
// OpenGL Function
//--------------------------------------------------------------------------------------
void glInitFunc();
void glSetInterval(int isinterval);
GLuint glLoadShader(const char *vsfile, const char *, const char *fsfile);

//--------------------------------------------------------------------------------------
// Input Function
//--------------------------------------------------------------------------------------
vec giGetDirection(float a = 1.0);
vec giGetButton(int a);


//--------------------------------------------------------------------------------------
// Mouse Function
//--------------------------------------------------------------------------------------
POINT GetMousePos();
BOOL GetMouseLeft()  ;
BOOL GetMouseRight() ;

//EVENT
void AddEvent_WM_SIZE(void (*proc)(int, int));


#endif //_UTIL_H_

