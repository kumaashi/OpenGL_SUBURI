#ifndef _NODE_H_
#define _NODE_H_


struct node {
	enum {
		Pos = 0,
		PosDelta,
		Rotate,
		RotateDelta,
		Scale,
		Color,
		MinMax,
		MAX,
	};

	struct {
		float x, y, z, w;
	} info[Max];

	void AddInfo(int index, int inv = false) {
		int sign = inv ? -1 : 1;
		info[index].x +=

	}
};






#endif

