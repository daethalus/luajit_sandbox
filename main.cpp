#include "lua_service.hpp"
#include <iostream>


int main() {
    naquadria::LuaService lua;


    lua.setFunction("test1", [](){
        std::cout << "wrapped 1" << std::endl;
        return 0;
    });

    lua.setFunction("test2", [](){
        std::cout << "wrapped 2" << std::endl;
        return 0;
    });

    //lua.newType("testType");

//    lua.doString(" test1()"
//                 " test2()"
//                 " local test = Test.new() ");

    auto table = lua.newTable("registry");

    table.setFunction("view", [](){
        std::cout << "view table" << std::endl;
        return 0;
    });

    table.setValue("asd", 10);

    lua.doString(" registry.view() ");

    return 0;
}