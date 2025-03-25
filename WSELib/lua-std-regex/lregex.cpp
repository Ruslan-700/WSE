// Copyright 2023-2025 Mitchell. See LICENSE.

#include "string_view.hpp"
#include <regex>

extern "C" {
#include "lauxlib.h"
}

/** Constructs and returns a std::regex from the second argument on the Lua stack.
 * Raises a Lua error if std::regex throws an exception.
 */
static std::regex get_regex(lua_State *L) {
	try {
		return std::regex{luaL_checkstring(L, 2)};
	} catch (const std::regex_error &e) {
		luaL_argerror(L, 2, (lua_pushfstring(L, "invalid regex: %s", e.what()), lua_tostring(L, -1)));
		return std::regex{};
	}
}

/* Returns the search offset from the third argument on the Lua stack, or 0 if none was given.
 * A negative offset counts from the end of the string. The offset is clamped at string boundaries.
 */
static int get_init(lua_State *L) {
	int init = luaL_optinteger(L, 3, 1), len = lua_objlen(L, 1);
	if (init > 0) return init <= len ? init - 1 : len;
	if (init < 0) init += len;
	return init > 0 && init < len ? init : 0;
}

using svmatch = std::match_results<nonstd::string_view::const_iterator>;

/** Returns a regex search result using arguments on the Lua stack. */
static svmatch search(lua_State *L) {
	const nonstd::string_view s{luaL_checkstring(L, 1)};
	svmatch results;
	return (std::regex_search(s.cbegin() + get_init(L), s.cend(), results, get_regex(L)), results);
}

/** Pushes the given regex search result onto the Lua stack.
 * If there are no captures, pushes the entire match (except if *only_subs* is `true`).
 * If there are captures, pushes all of them and excludes the entire match.
 */
static int push_results(lua_State *L, const svmatch &m, bool only_subs = false) {
	if (m.size() == 0) return (lua_pushnil(L), 1);
	if (m.size() == 1 && !only_subs) return (lua_pushlstring(L, &*m[0].first, m[0].length()), 1);
	luaL_checkstack(L, m.size() - 1, "too many captures");
	for (size_t i = 1; i < m.size(); i++) lua_pushlstring(L, &*m[i].first, m[i].length());
	return m.size() - 1;
}

/** regex.find() Lua function. */
static int find(lua_State *L) {
	const svmatch &m = search(L);
	const int init = get_init(L), n = push_results(L, m, true);
	if (n == 1 && lua_isnil(L, -1)) return n;
	lua_pushinteger(L, init + m.position(0) + 1), lua_insert(L, -n - 1);
	lua_pushinteger(L, init + m.position(0) + m.length(0)), lua_insert(L, -n - 1);
	return 2 + n;
}

/** Generator class for the `for match in regex.gmatch() do ... end` construction.
 * This class is the "initial state" used in Lua's generic for loop.
 */
class Generator {
	using iterator = std::regex_iterator<nonstd::string_view::const_iterator>;

public:
	Generator(const char *s_, const std::regex &re_, int init)
			: s{s_}, sv{s.c_str()}, re{re_}, it{sv.cbegin() + init, sv.cend(), re} {}
	iterator next() { return it++; }
	iterator &end() { return last; }

private:
	const std::string s;
	const nonstd::string_view sv;
	const std::regex re;
	iterator it, last;
};

/** Deletes the generator class when Lua's garbage collector collects it. */
static int generator_gc(lua_State *L) {
	return (reinterpret_cast<Generator *>(lua_touserdata(L, 1))->~Generator(), 0);
}

/** Generator function returned by `regex.gmatch()`.
 * It accepts a Generator class argument for iterating through regex search matches.
 */
static int generator(lua_State *L) {
	auto gen = reinterpret_cast<Generator *>(luaL_checkudata(L, 1, "regex_gmatch"));
	const auto &m = gen->next();
	if (m != gen->end()) return push_results(L, *m);
	return (lua_pushnil(L), 1);
}

/** regex.gmatch() Lua function. */
static int gmatch(lua_State *L) {
	const int init = get_init(L); // needs to be read before anything is pushed
	lua_pushcfunction(L, generator);
	new (reinterpret_cast<Generator *>(lua_newuserdata(L, sizeof(Generator))))
		Generator(luaL_checkstring(L, 1), get_regex(L), init);
	if (luaL_newmetatable(L, "regex_gmatch"))
		lua_pushcfunction(L, generator_gc), lua_setfield(L, -2, "__gc");
	lua_setmetatable(L, -2);
	lua_pushnil(L);
	return 3;
}

/** regex.gsub() Lua function. */
static int gsub(lua_State *L) {
	const nonstd::string_view s{luaL_checkstring(L, 1)};
	const std::regex re = get_regex(L);
	if (!lua_isstring(L, 3) && !lua_isfunction(L, 3) && !lua_istable(L, 3))
		return luaL_typerror(L, 3, "string/function/table");
	int n = 0, N = luaL_optinteger(L, 4, 0), repl = lua_type(L, 3);
	std::string result;
	auto it = std::regex_iterator<nonstd::string_view::const_iterator>{s.cbegin(), s.cend(), re},
			 last_it = it;
	for (decltype(it) last; it != last && (!N || n < N); last_it = it++) {
		result.append(&*it->prefix().first, it->prefix().length());
		if (repl == LUA_TTABLE || repl == LUA_TFUNCTION) {
			if (repl == LUA_TTABLE)
				lua_pop(L, push_results(L, *it) - 1), lua_gettable(L, 3); // only first result matters
			else
				lua_pushvalue(L, 3), lua_call(L, push_results(L, *it), 1);
			if (lua_toboolean(L, -1)) {
				if (!lua_isstring(L, -1))
					return luaL_error(L, "invalid replacement value (a %s)", luaL_typename(L, -1));
				result += lua_tostring(L, -1);
			} else
				result.append(&*(*it)[0].first, it->length(0)); // keep the original match
			lua_pop(L, 1), n++;
		} else {
			const nonstd::string_view r{lua_tostring(L, 3)};
			it->format(std::back_inserter(result), r.data(), r.data() + r.size()), n++;
		}
	}
	if (last_it != it && last_it->suffix().length() > 0)
		result.append(&*last_it->suffix().first, last_it->suffix().length());
	return (n > 0 ? static_cast<void>(lua_pushstring(L, result.c_str())) : lua_pushvalue(L, 1),
		lua_pushinteger(L, n), 2);
}

/** regex.match() Lua function. */
static int match(lua_State *L) { return push_results(L, search(L)); }

static const luaL_Reg lib[] = {
	{"find", find}, {"gmatch", gmatch}, {"gsub", gsub}, {"match", match}, {nullptr, nullptr}};

extern "C" {
LUA_API int luaopen_regex(lua_State *L) { return (luaL_newlib(L, lib), 1); }
}
