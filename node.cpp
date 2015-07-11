//-----------------------------------------------------------------------------
//
// NAMU
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <map>

//-----------------------------------------------------------------------------
// NOs
//-----------------------------------------------------------------------------
class Node {
	std::string name;
	Node *child;
	Node *chain;
	static Node *root;
protected:
	Node *GetRoot() {
		return root;
	}
public:
	Node() {
		chain = child = 0;
		Node *nd = root;
		SetChain(root);
		root = this;
	}

	virtual ~Node()                     {}
	virtual void Update()               {}
	virtual void Draw()                 {}
	Node* SetChild(Node *nd)            { return child = nd; }
	Node* GetChild()                    { return child; }
	std::string& SetName(const char *s) { return name = s; }
	const char* GetName()               { return name.c_str(); }
	void Print()                        { printf("NAME = %s\n", name.c_str()); }
};
Node *Node::root = 0;

//-----------------------------------------------------------------------------
class NodeManager : public Node {
public:
	NodeManager() {}
	virtual ~NodeManager() {}
	Node *GetRoot() { return Node::GetRoot(); }
};


//-----------------------------------------------------------------------------
class Actor : public Node {
public:
	Actor() { printf("%s\n", __FUNCTION__); }
	virtual ~Actor() {}

};

//-----------------------------------------------------------------------------
class Bullet : public Actor {
public:
	Bullet() { printf("%s\n", __FUNCTION__); }
	virtual ~Bullet() {}
	int IsHit() { return 0; }
	void Fire() {
	}
	void Term()
	{
	}
	void GetParent()
	{

	}
	void Us()
	{

	}
};


//-----------------------------------------------------------------------------
class Player : public Bullet {
public:
	Player() { printf("%s\n", __FUNCTION__); }
	virtual ~Player() {}
};

//-----------------------------------------------------------------------------
class Enemy : public Bullet {
public:
	Enemy() { printf("%s\n", __FUNCTION__); }
	virtual ~Enemy() {}
};

//-----------------------------------------------------------------------------
enum {
	ACTOR_SYSTEM = 0,
	ACTOR_BG:
	ACTOR_BULLET,
	ACTOR_PLAYER,
	ACTOR_ENEMY,
	ACTOR
};

//-----------------------------------------------------------------------------
Actor *ActorCreate(int num) {
	switch(num) {
		case ACTOR_SYSTEM = 0:
			return new System();
			break;
		case ACTOR_BG:
			break;
		case ACTOR_BULLET:
			break;
		case ACTOR_PLAYER:
			return new Player();
			break;
		case ACTOR_ENEMY:
			return new Enemy();
			break;
		default:
			break;
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// Ney
//-----------------------------------------------------------------------------
int main() {
	NodeManager mng;
	Node *temp = mng.GetRoot();
	while(temp) {
		printf("%08X:%08X\n", temp, temp->GetChain());
		temp = temp->GetChain();
	}
	return 0;
}






