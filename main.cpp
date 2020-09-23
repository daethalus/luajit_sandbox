#include <iostream>
#include <lua.hpp>

int main() {
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dostring(L, "print('hello world lua JIT!')");


    return 0;
}
