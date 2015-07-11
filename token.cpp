#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <string>
#include <map>

int main(int argc, char *argv[]) {
	std::vector<char> buf;
	std::map<std::string, std::string> symtable;
	FILE *fp = fopen(argv[1], "r");
	int size = 0;
	if(fp) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buf.resize(size + 1);
		fread(&buf[0], 1, size, fp);
		fclose(fp);
	}
	buf[size] = 0;
	char *p = &buf[0];
	
	while(*p) {
		while(isspace(*p)) p++;
		if(*p == 0) break;
		std::vector<char> tok;
		while(!isspace(*p)) {
			tok.push_back(*p++);
		}
		tok.push_back(0);
		symtable[std::string(&tok[0])] = std::string(&tok[0]);
		//printf("'%s'\n", &tok[0]);
	}
	
	auto it = symtable.begin();
	auto ite = symtable.end();
	while(it != ite) {
		printf("%s\n", it->second.c_str());
		it++;
	}
	return 0;
}

