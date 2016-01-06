#include "luamain.h"
#include "luawindow.h"
#include "luagl.h"


void InitLua(lua_State *l)
{
	luawindow::Init(l);
	luagl::Init(l);
}

int main(int argc, char *argv[])
{
	const char *fname = argv[1];
	lua_State* l = luaL_newstate();
	luaL_openlibs(l);
	if (luaL_loadfile(l, fname))
	{
		printf("cannot open %s\n", fname);
		return 0;
	}

	InitLua(l);

	if (lua_pcall(l, 0, 0, 0)) {
		printf("cannot exec %s\n", fname);
		fprintf(stderr, "cannot exec add. %s\n", lua_tostring(l, -1));
		return 0;
	}
	return 0;
}

