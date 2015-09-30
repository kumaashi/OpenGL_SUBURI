#ifndef _FILE_H_
#define _FILE_H_

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

#endif

