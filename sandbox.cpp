#include <iostream>
#include <lua.hpp>

struct Foo {
    int x;
    Foo(int _x) : x(_x) {
        std::cout << "foo constructor " << std::endl;
    }
};

static int testFunc(lua_State *L) {
    double val = lua_tonumber(L, lua_upvalueindex(1));
    std::cout << "my val is: " << val << std::endl;

    auto n1 = luaL_checknumber(L, 1);
    auto n2 = luaL_checknumber(L, 2);
    lua_pushnumber(L, n1 + n2);
    return 1;
}

static int funcUserdata(lua_State *L ) {
    Foo* foo = (*reinterpret_cast<Foo**>(luaL_checkudata(L, 1, "luaL_Foo")));
    std::cout << "func userData x value: " << foo->x;
    return 0;
}

static int l_Foo_constructor(lua_State *L) {
    int vl = luaL_checkinteger(L, 1);
    std::cout << "lua constructor " << std::endl;
//    void* memory = lua_newuserdata(L, sizeof(Foo));
//    new (memory) Foo(vl);

    *reinterpret_cast<Foo**>(lua_newuserdata(L, sizeof(Foo*))) = new Foo(vl);

    luaL_getmetatable(L, "luaL_Foo");
    lua_setmetatable(L, -2);

    return 1;
}

static int l_Foo_index(lua_State *L) {
    Foo* foo = (*reinterpret_cast<Foo**>(luaL_checkudata(L, 1, "luaL_Foo")));

    std::string str = luaL_checkstring(L, 2);
    std::cout << "get attribute: " << str << std::endl;
    if (str == "x") {
        lua_pushnumber(L, foo->x);
        return 1;
    } else {
        return 0;
    }
}

static int l_Foo_newindex(lua_State *L) {
    Foo* foo = (*reinterpret_cast<Foo**>(luaL_checkudata(L, 1, "luaL_Foo")));

    std::string str = luaL_checkstring(L, 2);
    int vl = luaL_checkinteger(L, 3);
    std::cout << "set attribute: " << str << " value: " << vl << std::endl;
    foo->x = vl;
    return 1;
}

//static int l_Foo_test(lua_State *L) {
//    std::cout << "now the things start to be cool " << std::endl;
//    return 0;
//}

int execute() {
    lua_State *L;
    L = luaL_newstate();
    luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
    luaL_openlibs(L);


    lua_pushnumber(L, 777);
    lua_pushcclosure(L, testFunc, 1);
   // lua_pushcfunction(L, testFunc);
    lua_setglobal(L, "myFunction");

    lua_pushcfunction(L, funcUserdata);
    lua_setglobal(L, "funcUserdata");

    luaL_Reg sFooRegs[] =
            {
                    {"new", l_Foo_constructor},
                    {"__index", l_Foo_index},
                    {"__newindex", l_Foo_newindex},
                    { NULL, NULL }
            };

    luaL_newmetatable(L, "luaL_Foo");
    luaL_register(L, NULL, sFooRegs);
    //lua_pushvalue(L, -1);
    //lua_setfield(L, -1, "__index");
    lua_setglobal(L, "Foo");

    luaL_dostring(L, "print(myFunction(120, 120))");

//    luaL_dostring(L, " local foo = Foo.new(10) "
//                        " print(foo.x) "
//                        " foo.x = 321  "
//                        " print(foo.x) "
//                        " funcUserdata(foo) ");


    //function wrapper ( with name if it's possible)
    //funcion callback


    //lua_upvalueindex

    lua_close(L);

    return 0;
}
