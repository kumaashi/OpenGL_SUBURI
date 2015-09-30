#ifndef _READER_H_
#define _READER_H_

#include "file.h"
#include "boundingbox.h"
#include <vector>

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

#endif
