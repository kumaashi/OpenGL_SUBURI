#ifndef _LUADEF_H_
#define _LUADEF_H_

#include <lua.hpp>

#pragma comment(lib, "lua.lib")

#define LUAPUSHGFN(env, fnname) \
	lua_pushcfunction(env, fnname); \
	lua_setglobal(env, #fnname);


#define LUARETURN_NIL(env, value) \
	return 0;

#define LUARETURN(env, value) \
	lua_pushnumber(env, value); \
	return 1;


#endif

