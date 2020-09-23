#ifndef LUAJIT_SANDBOX_LUA_SERVICE_HPP
#define LUAJIT_SANDBOX_LUA_SERVICE_HPP

#include <lua.hpp>
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <iostream>

struct Foo {

};


namespace naquadria {

    struct FunctionWrapper {

        static unsigned int counter;
        static std::vector<std::function<int()>> functions;

        static int call(lua_State *L) {
            return functions[lua_tonumber(L, lua_upvalueindex(1))]();
        }

        static void set(lua_State *L, std::function<int()> func) {
            auto id = counter++;
            if (id >= functions.size()) {
                functions.resize(id + 1);
            }
            functions[id] = std::move(func);
            lua_pushnumber(L, id);
            lua_pushcclosure(L, FunctionWrapper::call, 1);
        }
    };

    struct UserTypeWrapper {

        static int call(lua_State *L) {
            std::cout << "call userType: " << lua_tonumber(L, lua_upvalueindex(1)) <<  std::endl;

            *reinterpret_cast<Foo**>(lua_newuserdata(L, sizeof(Foo*))) = new Foo();

            luaL_getmetatable(L, "meta_Test");
            lua_setmetatable(L, -2);


            return 1;
        }
    };

    struct UserTypeRegister{

    };

    struct LuaTable {

        lua_State *L;
        int ref;

        LuaTable(lua_State * _L) :  L(_L) {
            lua_newtable(L);
            ref  = luaL_ref(L, LUA_REGISTRYINDEX);
        }

        void setFunction(const std::string&name, std::function<int()> func) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            lua_pushstring(L, name.c_str());
            FunctionWrapper::set(L, std::move(func));
            lua_settable(L, -3);
        }

        void setValue(const std::string&name, int vl) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            lua_pushstring(L, name.c_str());
            lua_pushnumber(L, vl);
            lua_settable(L, -3);
        }

    };

    unsigned int FunctionWrapper::counter = 0;
    std::vector<std::function<int()>> FunctionWrapper::functions = {};

    class LuaService {
    public:
        LuaService() {
            L = luaL_newstate();
            luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
            luaL_openlibs(L);
        }


        void setFunction(const std::string&name, std::function<int()> func) {
            FunctionWrapper::set(L, std::move(func));
            lua_setglobal(L, name.c_str());
        }

        void doString(const std::string&script) {
            luaL_dostring(L, script.c_str());
        }

        LuaTable newTable(const std::string&name) {
            LuaTable table(L);
            lua_rawgeti(L, LUA_REGISTRYINDEX, table.ref);
            lua_setglobal(L, name.c_str());
            return table;
        };

        void newType(const std::string&name) {
            luaL_newmetatable(L, "meta_Test");

            lua_pushnumber(L, 421);

            lua_pushcclosure(L, UserTypeWrapper::call, 1);
            lua_setfield(L, -2, "new");
            lua_setglobal(L, "Test");
        }

    private:
        lua_State *L;

    };
}




#endif //LUAJIT_SANDBOX_LUA_SERVICE_HPP
