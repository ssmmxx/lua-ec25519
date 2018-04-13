// Copyright (c) 2018 Maxim Spiridonov  -- see LICENSE file
// ---------------------------------------------------------------------

#define LUAEC25519_VERSION "luaec25519-0.1"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "tweetnacl.h"


//=========================================================
// compatibility with Lua 5.2  --and lua 5.3
#if (LUA_VERSION_NUM >= 502)

#undef lua_equal
#define lua_equal(L,idx1,idx2)  lua_compare(L,(idx1),(idx2),LUA_OPEQ)

#undef lua_getfenv
#define lua_getfenv	lua_getuservalue
#undef lua_setfenv
#define lua_setfenv	lua_setuservalue

#undef lua_objlen
#define lua_objlen	lua_rawlen

#undef luaL_register
#define luaL_register(L,n,f) \
	{ if ((n) == NULL) luaL_setfuncs(L,f,0); else luaL_newlib(L,f); }

#endif
//=========================================================

# define LERR(msg) return luaL_error(L, msg)

typedef unsigned char u8;
typedef unsigned long u32;
typedef unsigned long long u64;

extern int randombytes(unsigned char *x,unsigned long long xlen);

static int tw_randombytes(lua_State *L) {
    size_t bufln;
	unsigned char buf[256];
	lua_Integer li = luaL_checkinteger(L, 1);  // 1st arg
	if ((li > 256 ) || (li < 0)) {
		lua_pushnil (L);
		lua_pushliteral(L, "invalid byte number");
		return 2;
	}
	int r = randombytes(buf, li);
	if (r != 0) {
		lua_pushnil (L);
		lua_pushliteral(L, "random generator error");
		return 2;
	}
    lua_pushlstring (L, buf, li);
	return 1;
}

static int tw_public_key(lua_State *L) {
	size_t skln;
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	const char *sk = luaL_checklstring(L,1,&skln); // secret key
	if (skln != crypto_box_SECRETKEYBYTES) LERR("bad sk size");
	int r = crypto_scalarmult_base(pk, sk);
	lua_pushlstring (L, pk, crypto_box_PUBLICKEYBYTES);
	return 1;
}

static int tw_shared_key(lua_State *L) {
    size_t pkln, skln;
    unsigned char sh[crypto_box_SECRETKEYBYTES];
    const char *pk = luaL_checklstring(L,1,&pkln);
    const char *sk = luaL_checklstring(L,2,&skln);
    if (pkln != crypto_box_PUBLICKEYBYTES) LERR("shared_key: bad pk size");
    if (skln != crypto_box_SECRETKEYBYTES) LERR("shared_key: bad sk size");
    int r = crypto_scalarmult(sh, pk, sk);
    lua_pushlstring (L, sh, crypto_box_SECRETKEYBYTES);
    return 1;
}

static const struct luaL_Reg luaec25519lib[] = {
	{"randombytes", tw_randombytes},
	{"public_key", tw_public_key},
	{"shared_key", tw_shared_key},

	{NULL, NULL},
};

int luaopen_luaec25519 (lua_State *L) {
	luaL_register (L, "luaec25519", luaec25519lib);
    lua_pushliteral (L, "VERSION");
	lua_pushliteral (L, LUAEC25519_VERSION);
	lua_settable (L, -3);
	return 1;
}