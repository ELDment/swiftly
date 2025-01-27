#include "../core.h"

LoadLuaScriptingComponent(
    precacher,
    [](LuaPlugin* plugin, lua_State* state)
    {
        luabridge::getGlobalNamespace(state)
            .beginClass<PluginPrecacher>("Precacher")
            .addConstructor<void (*)(std::string)>()
            .addFunction("PrecacheModel", &PluginPrecacher::PrecacheModel)
            .addFunction("PrecacheSound", &PluginPrecacher::PrecacheSound)
            .addFunction("PrecacheItem", &PluginPrecacher::PrecacheItem)
            .endClass();

        luaL_dostring(state, "precacher = Precacher(GetCurrentPluginName())");
    }
)