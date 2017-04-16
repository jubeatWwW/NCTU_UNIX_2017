#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/types.h>

#define DECLARE(TYPE, NAME, ...) static TYPE (*old_##NAME)(__VA_ARGS) = NULL;
#define DLSYM(NAME) old_##NAME = dlsym(handle, #NAME);
#define RETURN(NAME) return old_##NAME(arg);

DECLARE(char*,          getenv,     const char*);

DECLARE(int,            closedir,   DIR*);
DECLARE(DIR*,           fdopendir,  int);
DECLARE(DIR*,           opendir,    const char*);
DECLARE(struct dirent*, readdir,    DIR*)
DECLARE(int,            readdir_r,  DIR*, struct dirent*, struct dirent*);
DECLARE(void,           rewinddir,  DIR*);
DECLARE(void,           seekdir,    DIR*, long);
DECLARE(long,           telldir,    DIR*)

DECLARE(int,            creat,      const char*, mode_t);
DECLARE(int,            open,       const char*, int);
DECLARE(int,            remove,     const char*);
DECLARE(int,            rename,     const char*, const char*);
DECLARE(void,           setbuf,     FILE*, char*);
DECLARE(int,            setvbuf,    FILE*, char*, int, size_t);
DECLARE(char*,          tempnam,    const char*, const char*);
DECLARE(FILE*,          tmpfile,    void);

__attribute__((constructor)) void HijackInit() {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);

    if(NULL != handle){
        DLSYM(closedir);
        DLSYM(fdopendir);
        DLSYM(opendir);
        DLSYM(readdir);
        DLSYM(readdir_r);
        DLSYM(rewinddir);
        DLSYM(seekdir);
        DLSYM(telldir);

        DLSYM(getenv);
    }
}

char* getenv(const char* arg){

    fprintf(stderr, "[monitor] getenv('%s') = '%s'\n", arg, old_getenv(arg));
    RETURN(getenv);
}

int closedir(DIR* arg){
    fprintf(stderr, "closedir injected\n");
    
    RETURN(closedir);
}

DIR* fdopendir(int arg){
    fprintf(stderr, "fdopendir injected\n");
    
    RETURN(fdopendir);
}

DIR* opendir(const char* arg){
    fprintf(stderr, "opendir YAYAYA\n");
    RETURN(opendir);
}

struct dirent* readdir(DIR* arg){
    fprintf(stderr, "readdir fuck u\n");
    RETURN(readdir);
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result){
    fprintf(stderr, "readdir\n");
    return old_readdir_r(dirp, entry, result);
}

void rewinddir(DIR *dirp){
    fprintf(stderr, "[monitor] rewinddir\n");
    old_rewinddir(dirp);
}

void seekdir(DIR* dirp, long doc){
    fprintf(stderr, "[monitor] seekdir\n");
    old_seekdir(dirp, doc);
}

long telldir(DIR* arg){
    fprintf(stderr, "[monitor] telldir = %ld", old_telldir(arg));
    RETURN(telldir);
}
