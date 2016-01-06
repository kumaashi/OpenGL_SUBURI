#ifndef _LUAWINDOW_H_
#define _LUAWINDOW_H_

#include <lua.hpp>
#include <windows.h>

namespace luawindow
{
	void Init(lua_State *l);
	HWND GetHwnd();
}

#endif

