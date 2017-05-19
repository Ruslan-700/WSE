#if !defined( __lanes_h__)
#define __lanes_h__ 1

#include "lua.h"

#if (defined PLATFORM_WIN32) || (defined PLATFORM_POCKETPC)
#define LANES_API __declspec(dllexport)
#else
#define LANES_API
#endif // (defined PLATFORM_WIN32) || (defined PLATFORM_POCKETPC)

/* wse mod */
typedef void(*WSECallback)(lua_State *L);

#ifdef __cplusplus
extern "C"{
#endif 

	extern int LANES_API luaopen_lanes_core(lua_State* L);

	// Call this to work with embedded Lanes instead of calling luaopen_lanes_core()
	/* wse mod */
	extern void LANES_API luaopen_lanes_embedded(lua_State* L, WSECallback laneNewCB, lua_CFunction _luaopen_lanes);

#ifdef __cplusplus
}
#endif

#endif // __lanes_h__