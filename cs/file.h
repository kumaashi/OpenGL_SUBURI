#ifndef _FILE_H_
#define _FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>

class File {
	std::vector<unsigned char> vBuf;
public:
	int Size()  { return vBuf.size(); }
	void *Buf() { return &vBuf[0]; }
	File(const char *name) {
		Open(name);
	}
	~File() {
		Close();
	}
	void Close() {}
	int Open(const char *name) {
		vBuf.clear();
		FILE *fp = fopen(name, "rb");
		if(fp) {
			fseek(fp, 0, SEEK_END);
			int Size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			vBuf.resize(Size);
			memset(&vBuf[0], 0, vBuf.size());
			fread(&vBuf[0], 1, Size, fp);
			fclose(fp);
		}
		return Size();
	}
};

#endif
