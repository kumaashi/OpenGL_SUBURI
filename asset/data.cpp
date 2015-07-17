#include <stdio.h>
#include <windows.h>
#include <vector>
#include <algorithm>


#define FUNC_CHECK printf("%s:%08X\n", __FUNCTION__, this)
struct DataBase {
	int type;
public:
	DataBase() {
		FUNC_CHECK;
	}
	virtual ~DataBase() {
		FUNC_CHECK;
	}
	virtual void Update() = 0;
	void SetType(int a) {
		type = a;
	}
	int GetType() {
		return type;
	}
};

struct A : public DataBase {
public:
	A() {
		FUNC_CHECK;
		SetType(1);
	}
	virtual ~A() {
		FUNC_CHECK;
	}
	void Update() {
		FUNC_CHECK;
	}
};
struct B : public DataBase {
public:
	B() {
		FUNC_CHECK;
		SetType(2);
	}
	virtual ~B() {
		FUNC_CHECK;
	}
	void Update() {
		FUNC_CHECK;
	}
};
struct C : public DataBase {
public:
	C() {
		FUNC_CHECK;
		SetType(3);
	}
	virtual ~C() {
		FUNC_CHECK;
	}
	void Update() {
		FUNC_CHECK;
	}
};

bool data_sort(DataBase *a, DataBase *b) {
	return a->GetType() < b->GetType();
}


bool data_sort_int(int a, int b) {
	return a < b;
}

int main()
{
	std::vector<DataBase *> vdata;
	for(int i = 0 ; i < 10; i++) {
		int tosuke = rand() % 3;
		if(tosuke == 0) vdata.push_back(new A);
		if(tosuke == 1) vdata.push_back(new B);
		if(tosuke == 2) vdata.push_back(new C);
	}
	
	std::sort(vdata.begin(), vdata.end(), data_sort);
	
	{
		auto it = vdata.begin();
		auto ite = vdata.end();
		while(it != ite) {
			(*it)->Update();
			it++;
		}
	}
	
	{
		auto it = vdata.begin();
		auto ite = vdata.end();
		while(it != ite) {
			auto next = it + 1;
			delete (*it);
			it = next;
		}
	}
	
	
	std::vector<int> vint;
	for(int i = 0 ; i < 5000000; i++) {
		vint.push_back(rand());
	}
	unsigned long start = timeGetTime();
	std::sort(vint.begin(), vint.end(), data_sort_int);
	printf("SORT = %d [ms]\n", timeGetTime() - start);
	
}
