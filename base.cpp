#include <stdio.h>
#include <vector>





class Base {
public:
	Base() {
		printf("%08X%s:cons\n", this, __FUNCTION__);
	}
	virtual ~Base() {
		printf("%08X:%s:dest\n", this, __FUNCTION__);
	}
	virtual void Update() = 0;
};


class A : public Base {
public:
	A() {
		printf("%08X%s:cons\n", this, __FUNCTION__);
	}
	
	~A() {
		printf("%08X:%s:dest\n", this, __FUNCTION__);
	}
	
	void Update() {
		printf("%08X:%s:Update\n", this, __FUNCTION__);
	}
};

	
	
class B : public Base {
public:
	B() {
		printf("%08X%s:cons\n", this, __FUNCTION__);
	}
	
	~B() {
		printf("%08X:%s:dest\n", this, __FUNCTION__);
	}
	
	void Update() {
		printf("%08X:%s:Update\n", this, __FUNCTION__);
	}
};



int main() {
	std::vector<Base *> vbase;
	for(int i = 0; i < 10; i++) {
		vbase.push_back(new A);
	}
	for(int i = 0; i < 10; i++) {
		vbase.push_back(new B);
	}
	
	{
		auto ib = vbase.begin();
		auto ibe = vbase.end();
		while(ib != ibe) {
			(*ib)->Update();
			ib++;
		}
	}
	{
		auto ib = vbase.begin();
		auto ibe = vbase.end();
		while(ib != ibe) {
			auto next = ib + 1;
			delete (*ib);
			ib = next;
		}
	}
}



