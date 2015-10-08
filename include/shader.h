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
		//printf("%s:name=%s, shader=%d\n", __FUNCTION__, name, shader);
		int size = 0;
		/*
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled != GL_FALSE) {
			printf("%s:name=%s, Compiled!\n", __FUNCTION__, name);
			return;
		}
		*/
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
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, (char **)&b, &size);
		glCompileShader(shader);
		PrintShaderLog(fname, shader);
		printf("%s:name=%s, Done!\n", __FUNCTION__, fname);
		return shader;
	}

	void Unload() {
		CleanShader();
		if(program) {
			glDeleteProgram(program);
			program = 0;
		}
	}

	void CleanShader() {
		if(fs) glDeleteShader(fs);
		if(gs) glDeleteShader(gs);
		if(vs) glDeleteShader(vs);
		vs = 0;
		gs = 0;
		fs = 0;
	}
	
	GLint GetUniformLocation(const char *name) {
		if(!program) return -1;
		return glGetUniformLocation(program,  name);
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
		if(vs) {
			glAttachShader(program, vs);
			PrintShaderLog(vsfile, vs);
		}

		if(gs) {
			glAttachShader(program, gs);
			PrintShaderLog(gsfile, gs);
		}

		if(fs) {
			glAttachShader(program, fs);
			PrintShaderLog(fsfile, fs);
		}

		glLinkProgram(program);
		int result = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if(result == GL_FALSE) {
			printf("%s:Failed glLinkProgram\n", __FUNCTION__);
			int maxLength;
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
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
