#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

/*
	全部のオブジェクトはNodeに集約されている。
	[Node]
	->Actor
		->Camera
	->Data[Shader, Mesh]
	->

*/

#define DEBUG printf("%s(%s) : %08X\n", __FUNCTION__,  this)

//-----------------------------------------------------------------------
// Node
//-----------------------------------------------------------------------
class Node {
	static Node *root;
	static int   gid;
	Node *next;
	int id;
public:
	Node() {
		root = this;
		this->next = root;
		SetId(gid++);
	}
	virtual ~Node(){}
	Node *GetNext()   { return next;   }
	int  GetId()      { return id;     }
	int  SetId(int i) { return id = i; }
	void Del()        { delete this;   }
	virtual void Draw(){}
	virtual void Move(){}
};
Node *Node::root = NULL;
int Node::gid   = 1;

bool FuncSort(Node *a, Node *b) {
	return a->GetId() > b->GetId();
}

//-----------------------------------------------------------------------
// SceneManager
//-----------------------------------------------------------------------
class SceneManager {
	std::map<std::string, Node *> vNodeMap;
	std::vector<Node *> vmove;
	std::vector<Node *> vdraw;
public:
	void AddNode(const char *name,  Node *node) {
		std::string n(name);
		vNodeMap[n] = node;
	}
	
	void Draw() {
		auto it  = vNodeMap.begin();
		auto ite = vNodeMap.end();
		while(it != ite) {
			(*it).second->Draw();
			it++;
		}
	}
	
	void Move() {
		DEBUG;
		auto it  = vNodeMap.begin();
		auto ite = vNodeMap.end();
		while(it != ite) {
			(*it).second->Move();
			it++;
		}
	}
};


//-----------------------------------------------------------------------
// Actor
//-----------------------------------------------------------------------
class Actor : public Node {
public:
	Actor() { }
	virtual ~Actor() { }
	virtual void Move() {
		DEBUG;
	}
	virtual void Draw() {
		DEBUG;
	}
};


void main() {
	SceneManager mng;
	for(int i = 0 ; i < 100; i++) {
		std::string name = std::string("test") + std::to_string(i);
		mng.AddNode(name.c_str(), new Actor);
	}

	mng.Move();
}



