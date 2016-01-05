#include <stdio.h>
#include <lua.hpp>

int main(int argc, char *argv[])
{
	const char *fname = argv[1];
	lua_State* l = luaL_newstate(); // lua context Žæ“¾
	luaL_openlibs(l);
	if (luaL_loadfile(l, fname))
	{
		printf("cannot open %s\n", fname);
		return 0;
	}

	if (lua_pcall(l, 0, 0, 0)) {
		printf("cannot exec %s\n", fname);
		return 0;
	}
	return 0;
}

