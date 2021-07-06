#include <string.h>
#include <stdlib.h>
#include "lua-core.h"


void lua_writestring(const char *s, size_t l){
	size_t sblen = strlen(log_buf);
	if(sblen+l>=sizeof(log_buf)){
		lua_writeline();
		sblen = 0;
		if(l>=sizeof(log_buf)){
			char *buf = alloca(l+1);
			strncpy(buf,s,l);
			buf[l] = 0;
			l = 0;
		}
	}
	if(l>0){
		strncat(log_buf,s,l);
		log_buf[sblen+l] = 0;
	}
}

void lua_writeline(void){
	memset(log_buf,0,sizeof(log_buf));
}

int open() {
	for(int i=0;i<MAX_VMS;i++){
		if(!vms[i]){
			lua_State *L = luaL_newstate();
			if(L){
				luaL_openlibs(L);
    			luaL_requiref(L, "lfs", luaopen_lfs, 1);
				lua_pop(L,1);
    			luaL_requiref(L, "cjson", luaopen_cjson, 1);
				lua_pop(L,1);
    			luaL_requiref(L, "cjson_safe", luaopen_cjson_safe, 1);
				lua_pop(L,1);
                // luaL_requiref(L, "vmplugin", luaopen_vmplugin, 1);
				// lua_pop(L,1);
				vms[i] = L;
				return i;
			}
		}
	}
	return -1;
}

int close(int id) {
	if(id>=0&&id<MAX_VMS){
		lua_State *L = vms[id];
		if(L){
			lua_close(L);
			vms[id] = NULL;
			return 0;
		}
	}
	return -1;
}

char** eval(int id, char* code){
	const char *restr = "Fail";
	char** rets;
	if(id>=0&&id<MAX_VMS){
		lua_State *L = vms[id];
		if(L){
  		int base = lua_gettop(L);
			int res = luaL_dostring(L, code);
			int top = lua_gettop(L);
			
			if(res>0){
				restr = lua_tostring(L,-1);
				lua_pop(L,1);
				top = lua_gettop(L);
			}else{
				restr = "OK";
			}
			if(top>0){
				rets = malloc((top +1) * sizeof(char*));
				
				for(int i=0;i<top;i++){
					const char *str = lua_tostring(L, i-top);
					if(str){
						 rets[i+1] = malloc(strlen(str) * sizeof(char));
						strcpy(rets[i+1], str);
					}
				}
			}
			if(top>base){
  				lua_pop(L, top-base);
			}
		}else{
			restr = "VM Not exist";
		}
	}else{
		restr = "VM ID out of range";
	}
	if(rets==NULL){
		rets = malloc(strlen(restr) * sizeof(char));
		strcpy(rets[0], restr);
	} else{
		rets[0] = malloc(strlen(restr) * sizeof(char));
		strcpy(rets[0], restr);
	}
	return rets;
}


int test() {

	printf("teste");
}

// JNIEXPORT jobjectArray JNICALL Java_com_github_tgarm_luavm_LuaJNI_eval
// (JNIEnv *env, jclass cls, jint id, jstring jcode){
// 	const char *restr = "Fail";
// 	jobjectArray rets = NULL;
// 	if(id>=0&&id<MAX_VMS){
// 		lua_State *L = vms[id];
// 		if(L){
// 			const char *code = (*env)->GetStringUTFChars(env,jcode, NULL);
//   			int base = lua_gettop(L);
// 			int res = luaL_dostring(L, code);
// 			int top = lua_gettop(L);
// 			if(res>0){
// 				restr = lua_tostring(L,-1);
// 				lua_pop(L,1);
// 				top = lua_gettop(L);
// 			}else{
// 				restr = "OK";
// 			}
// 			if(top>0){
// 				rets = (*env)->NewObjectArray(env, top+1,(*env)->FindClass(env,"java/lang/String"),(*env)->NewStringUTF(env,""));
// 				for(int i=0;i<top;i++){
// 					const char *str = lua_tostring(L, i-top);
// 					if(str){
// 						(*env)->SetObjectArrayElement(env,rets,i+1,(*env)->NewStringUTF(env,str));
// 					}
// 				}
// 			}
// 			if(top>base){
//   				lua_pop(L, top-base);
// 			}
// 		}else{
// 			restr = "VM Not exist";
// 		}
// 	}else{
// 		restr = "VM ID out of range";
// 	}
// 	if(rets==NULL){
// 		rets = (*env)->NewObjectArray(env, 1,(*env)->FindClass(env,"java/lang/String"),(*env)->NewStringUTF(env,restr));
// 	}else{
// 		(*env)->SetObjectArrayElement(env,rets,0,(*env)->NewStringUTF(env,restr));

// 	}
// 	return rets;
// }
