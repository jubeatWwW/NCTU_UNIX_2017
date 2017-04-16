#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#define DECLARE(TYPE, NAME, ARG) static TYPE (*old_##NAME)(ARG) = NULL;
#define DLSYM(NAME) old_##NAME = dlsym(handle, #NAME);

DECLARE(uid_t, getuid, void);
DECLARE(char*, getenv, const char*);

__attribute__((constructor)) void HijackInit() {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);

    if(NULL != handle){
        DLSYM(getenv);
        DLSYM(getuid);
    }
}

char* getenv(const char* name){

    fprintf(stderr, "getenv injected\n");

    if(old_getenv != NULL)
        fprintf(stderr, "%s\n", old_getenv(name));
    return old_getenv(name) ;
}

uid_t getuid(void){
    fprintf(stderr, "RRRRRRRRRRRRRRRRRRRRRRRR\n");

    return old_getuid();
}
