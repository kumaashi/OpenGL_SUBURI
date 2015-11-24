#ifndef _SHADER_H_
#define _SHADER_H_

#include "util.h"

class Shader {
	GLuint program;
	GLuint vs;
	GLuint gs;
	GLuint fs;
	std::string strvs;
	std::string strgs;
	std::string strfs;
public:
	Shader() {
		vs = 0;
		gs = 0;
		fs = 0;
		program = 0;
		strvs = "null";
		strgs = "null";
		strfs = "null";
	}

	~Shader() {
		Unload();
	}

	GLuint Get() {
		if(!program) {
			printf("%s:[[[Warning]]] invalid shader %s %s %s\n", __FUNCTION__, strvs.c_str(), strgs.c_str(), strfs.c_str());
		}
		return program;
	}

	void PrintShaderLog(const char *name, GLuint shader) {
		int size = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &size);
		if(size) {
			std::vector<char> buf(size);
			memset(&buf[0], 0, buf.size());
			glGetShaderInfoLog(shader, buf.size(), &size, &buf[0]);
			char *s = &buf[0];
			printf("%s START name=%s, ============================\n", __FUNCTION__, name);
			printf("%s\n", s);
			printf("%s END   name=%s, ============================\n", __FUNCTION__, name);
		}
	}

	GLuint CreateShaderFromFile(const char *fname, int type) {
		File fp;
		int size = fp.Open(fname, "rb");
		if(size <= 0) {
			printf("%s:fname=%s Cant load.\n", __FUNCTION__, fname);
			return 0;
		}
		unsigned char *b = fp.Buf();
		GL_DEBUG0;
		GLuint shader = glCreateShader(type); GL_DEBUG0;
		glShaderSource(shader, 1, (char **)&b, &size); GL_DEBUG0;
		glCompileShader(shader); GL_DEBUG0;
		PrintShaderLog(fname, shader);
		printf("%s:name=%s, Done!\n", __FUNCTION__, fname);
		return shader;
	}

	void Unload() {
		CleanShader();
		if(program) {
			glDeleteProgram(program);
			GL_DEBUG0;
			program = 0;
		}
	}

	void CleanShader() {
		GL_DEBUG0;

		if(fs) { glDeleteShader(fs); GL_DEBUG0; }
		if(gs) { glDeleteShader(gs); GL_DEBUG0; }
		if(vs) { glDeleteShader(vs); GL_DEBUG0; }
		vs = 0;
		gs = 0;
		fs = 0;
	}
	
	GLint GetUniform(const char *name) {
		if(!program) return -1;
		GL_DEBUG0;
		GLint loc = glGetUniformLocation(program,  name); GL_DEBUG0;
		return loc;
	}
	
	void Begin() {
		GLint prog = Get();
		if(prog <= 0) {
			GL_DEBUG0;
			printf("aaaaaaaaaa");
			return;
		}
		
		glUseProgram(prog); GL_DEBUG0;
	}
	
	void End() {
		glUseProgram(0); GL_DEBUG0;
	}

	GLint SetUniform1i(const char *name, int data) {
		GLint loc = GetUniform(name);
		if(loc < 0) {
			GL_DEBUG;
			return loc;
		}
		glUniform1i(loc, data); GL_DEBUG0;
		return loc;
	}

	GLint SetUniform4fv(const char *name, int n, void *data) {
		GLint loc = GetUniform(name);
		if(loc < 0) {
			GL_DEBUG;
			return loc;
		}
		glUniform4fv(loc, n, (float *)data); GL_DEBUG0;
		return loc;
	}
	
	
	GLint SetUniformMatrix4fv(const char *name, int n, int inv, void *data) {
		GLint loc = GetUniform(name);
		if(loc < 0) {
			GL_DEBUG;
			return loc;
		}
		glUniformMatrix4fv(loc, n, inv, (float *)data); GL_DEBUG0;
		return loc;
	}

	int LoadProgramFromFile(const char *vsfile, const char *gsfile, const char *fsfile) {
		Unload();
		vs = CreateShaderFromFile(vsfile, GL_VERTEX_SHADER);
		if(!vs) {
			printf("%s:Failed Compile Shader -> %s\n", __FUNCTION__, vsfile);
			Unload();
			return -1;
		}

		fs = CreateShaderFromFile(fsfile, GL_FRAGMENT_SHADER);
		if(!fs) {
			printf("%s:Failed Compile Shader -> %s\n", __FUNCTION__, fsfile);
			Unload();
			return -1;
		}
		
		//areba
		if(gsfile) {
			gs = CreateShaderFromFile(gsfile, GL_GEOMETRY_SHADER);
			if(!gs) {
				printf("%s:Failed Compile Shader -> %s\n", __FUNCTION__, gsfile);
				Unload();
				return -1;
			}
		}
		printf("%s:vs=%04d gs=%04d fs=%04d\n", __FUNCTION__, vs, gs, fs);

		if(vsfile) strvs = vsfile;
		if(gsfile) strgs = gsfile;
		if(fsfile) strfs = fsfile;

		program = glCreateProgram();
		GL_DEBUG0;
		if(vs) {
			glAttachShader(program, vs); GL_DEBUG0;
			PrintShaderLog(vsfile, vs);
		}

		if(gs) {
			glAttachShader(program, gs); GL_DEBUG0;
			PrintShaderLog(gsfile, gs);
		}

		if(fs) {
			glAttachShader(program, fs); GL_DEBUG0;
			PrintShaderLog(fsfile, fs);
		}

		glLinkProgram(program); GL_DEBUG0;
		int result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &result); GL_DEBUG0;
		if(result == GL_FALSE) {
			printf("%s:Failed glLinkProgram\n", __FUNCTION__);
			int maxLength;
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength); GL_DEBUG0;
			if(maxLength) {
				std::vector<char> vlog(maxLength);
				memset(&vlog[0], 0, vlog.size());
				glGetProgramInfoLog(program, maxLength, &length, &vlog[0]);
				printf("%s\n", &vlog[0]);
			}
			Unload();
			return -1;
		}
		
		//DEBUG
		PrintShaderLog("OK glLinkProgram", program);
		if(fsfile) PrintShaderLog(fsfile, fs);
		if(gsfile) PrintShaderLog(gsfile, gs);
		if(vsfile) PrintShaderLog(vsfile, vs);

		return 0;
	}
};


#endif
