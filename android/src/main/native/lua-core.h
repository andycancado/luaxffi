#ifndef LUA_CORE_H_   /* Include guard */
#define LUA_CORE_H_
#include "../../../../ios/lua-src/lua.h"
#include "../../../../ios/lua-src/lauxlib.h"
#include "../../../../ios/lua-src/lualib.h"
#include "../../../../ios/lua-src/lfs/lfs.h"
#include "../../../../ios/lua-src/cjson/lua_cjson.h"



#define MAX_VMS	100

static lua_State *vms[MAX_VMS] = {NULL};

static char log_buf[256] = {0};

void lua_writestring(const char *s, size_t l);
void lua_writeline(void);
int close(int id);
char** eval(int id, char* code);
int test();

#endif //
