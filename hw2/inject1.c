#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DECLARE(TYPE, NAME, ...) static TYPE (*old_##NAME)(__VA_ARGS__) = NULL;
#define DLSYM(NAME) old_##NAME = dlsym(handle, #NAME);

#define DLSYM8(s1, s2, s3, s4, s5, s6, s7, s8) \
    DLSYM(s1); \
    DLSYM(s2); \
    DLSYM(s3); \
    DLSYM(s4); \
    DLSYM(s5); \
    DLSYM(s6); \
    DLSYM(s7); \
    DLSYM(s8);
        

#define PRINT(FUNC, FORMAT, RET, RETTYPE, ...) \
    if(NULL == old_##FUNC) \
        fprintf(stderr, #FUNC " is down\n"); \
    else { \
        RETTYPE tmp = old_##FUNC(__VA_ARGS__); \
        fprintf(stderr, "[monitor] " #FUNC #FORMAT " = " #RET "\n", __VA_ARGS__, tmp); \
        return tmp; \
    }

#define PRINT_NO_ARGS(FUNC, RET, RETTYPE) \
    RETTYPE tmp = old_##FUNC(); \
    fprintf(stderr, "[monitor] " #FUNC "() = " #RET "\n", tmp); \
    return tmp;

#define PRINT_RET_VOID(FUNC, FORMAT, ...) \
    fprintf(stderr, "[monitor] " #FUNC #FORMAT "\n", __VA_ARGS__); \
    FUNC(__VA_ARGS__);


DECLARE(int,            closedir,   DIR*);
DECLARE(DIR*,           fdopendir,  int);
DECLARE(DIR*,           opendir,    const char*);
DECLARE(struct dirent*, readdir,    DIR*)
DECLARE(int,            readdir_r,  DIR*, struct dirent*, struct dirent**);
DECLARE(void,           rewinddir,  DIR*);
DECLARE(void,           seekdir,    DIR*, long);
DECLARE(long,           telldir,    DIR*)

DECLARE(int,            creat,      const char*, mode_t);
DECLARE(int,            open,       const char*, int, mode_t);
DECLARE(int,            remove,     const char*);
DECLARE(int,            rename,     const char*, const char*);
DECLARE(void,           setbuf,     FILE*, char*);
DECLARE(int,            setvbuf,    FILE*, char*, int, size_t);
DECLARE(char*,          tempnam,    const char*, const char*);
DECLARE(FILE*,          tmpfile,    void);


DECLARE(char*,          tmpnam,     char*);
DECLARE(void,           exit,       int);
DECLARE(char*,          getenv,     const char*);
DECLARE(char*,          mkdtemp,    char*);
DECLARE(int,            mkstemp,    char*);
DECLARE(int,            putenv,     char*);
DECLARE(int,            rand,       void);
DECLARE(int,            rand_r,     unsigned int*);

DECLARE(int,            setenv,     const char*, const char*, int);
DECLARE(void,           srand,      unsigned int);
DECLARE(int,            system,     const char*);
DECLARE(int,            chdir,      const char*);
DECLARE(int,            chown,      const char*, uid_t, gid_t);
DECLARE(int,            close,      int);
DECLARE(int,            dup,        int);
DECLARE(int,            dup2,       int, int);

DECLARE(void,           _exit,      int);
DECLARE(int,            execl,      const char*, const char*, ...);
DECLARE(int,            execle,     const char*, const char*, ...);
DECLARE(int,            execlp,     const char*, const char*, ...);
DECLARE(int,            execv,      const char*, char *const argv[]);
DECLARE(int,            execve,     const char*, char *const argv[], char *const envp[]);
DECLARE(int,            execvp,     const char*, char *const argv[]);
DECLARE(int,            fchdir,     int);

DECLARE(int,            fchown,     int, uid_t, gid_t);
DECLARE(pid_t,          fork,       void);
DECLARE(int,            fsync,      int);
DECLARE(int,            ftruncate,  int, off_t);
DECLARE(char*,          getcwd,     char*, size_t);
DECLARE(gid_t,          getegid,    void);
DECLARE(uid_t,          geteuid,    void);
DECLARE(gid_t,          getgid,     void);

DECLARE(uid_t,          getuid,     void);
DECLARE(int,            link,       const char*, const char*);
DECLARE(int,            pipe,       int[2]);
DECLARE(ssize_t,        pread,      int,       void*, size_t, off_t);
DECLARE(ssize_t,        pwrite,     int, const void*, size_t, off_t);
DECLARE(ssize_t,        read,       int, void*, size_t);
DECLARE(ssize_t,        readlink,   const char*, char*, size_t);
DECLARE(int,            rmdir,      const char*);

DECLARE(int,            setegid,    gid_t);
DECLARE(int,            seteuid,    uid_t);
DECLARE(int,            setgid,     gid_t);
DECLARE(int,            setuid,     uid_t);
DECLARE(unsigned int,   sleep,      unsigned int);
DECLARE(int,            symlink,    const char*, const char*);
DECLARE(int,            unlink,     const char*);
DECLARE(ssize_t,        write,      int, const void*, size_t);

DECLARE(int,            chmod,      const char*, mode_t);
DECLARE(int,            fchmod,     int, mode_t);
DECLARE(int,            fstat,      int,         struct stat*);
DECLARE(int,            lstat,      const char*, struct stat*);
DECLARE(int,            mkdir,      const char*, mode_t);
DECLARE(int,            mkfifo,     const char*, mode_t);
DECLARE(int,            stat,       const char*, struct stat*);
DECLARE(mode_t,         umask,      mode_t);

__attribute__((constructor)) void HijackInit() {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);

    if(NULL != handle){
        DLSYM8(closedir, fdopendir, opendir, readdir, readdir_r, rewinddir, seekdir, telldir);
        DLSYM8(creat, open, remove, rename, setbuf, setvbuf, tempnam, tmpfile);
        DLSYM8(tmpnam, exit, getenv, mkdtemp, mkstemp, putenv, rand, rand_r);       
        DLSYM8(setenv, srand, system, chdir, chown, close, dup, dup2);
        DLSYM8(_exit, execl, execle, execlp, execv, execve, execvp, fchdir);
        DLSYM8(fchown, fork, fsync, ftruncate, getcwd, getegid, geteuid, getgid);
        DLSYM8(getuid, link, pipe, pread, pwrite, read, readlink, rmdir);
        DLSYM8(setegid, seteuid, setgid, setuid, sleep, symlink, unlink, write);
        DLSYM8(chmod, fchmod, fstat, lstat, mkdir, mkfifo, stat, umask);
    }

    fprintf(stderr, "end dlopen\n");
}

int closedir(DIR* dirp){
    PRINT(closedir, (%p), %d, int, dirp);
}

DIR* fdopendir(int fd){
    PRINT(fdopendir, (%d), %p, DIR*, fd);
}

DIR* opendir(const char* name){
    PRINT(opendir, ('%s'), %p, DIR*, name)
}

struct dirent* readdir(DIR* dirp){
    PRINT(readdir, (%p), %p, struct dirent*, dirp);
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result){
    PRINT(readdir_r, (%p, %p, %p), %d, int, dirp, entry, result);
}

void rewinddir(DIR *dirp){
    PRINT_RET_VOID(rewinddir, (%p), dirp);
}

void seekdir(DIR* dirp, long doc){
    PRINT_RET_VOID(seekdir, (%p, %ld), dirp, doc);
}

long telldir(DIR* dirp){
    PRINT(telldir, (%p), %ld, long, dirp);
}

//DLSYM8(creat, open, remove, rename, setbuf, setvbuf, tempnam, tmpfile);
int creat(const char *path, mode_t mode){
    PRINT(creat, ('%s', %d), %d, int, path, mode);
}

int open(const char *pathname, int flags, mode_t mode){
    PRINT(open, ('%s', %d, %d), %d, int, pathname, flags, mode);
}

int remove(const char *pathname){
    PRINT(remove, ('%s'), %d, int, pathname);
}

void setbuf(FILE *stream, char *buf){
    PRINT_RET_VOID(setbuf, (%p, %s), stream, buf);
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size){
    PRINT(setvbuf, (%p, '%s', %d, %ld), %d, int, stream, buf, mode, size);
}

char* tempnam(const char *dir, const char *pfx){
    PRINT(tempnam, ('%s', '%s'), '%s', char*, dir, pfx);
}

FILE* tmpfile(void){
    PRINT_NO_ARGS(tmpfile, %p, FILE*);
}

//DLSYM8(tmpnam, exit, getenv, mkdtemp, mkstemp, putenv, rand, rand_r);       
char* tmpnam(char *s){
    PRINT(tmpnam, ('%s'), '%s', char*, s);
}

void exit(int status){
    PRINT_RET_VOID(exit, %d, status);
}

char* getenv(const char* name){
    PRINT(getenv, ('%s') , '%s', char*, name);
}

char* mkdtemp(char* template){
    PRINT(mkdtemp, ('%s'), '%s', char*, template);
}

int mkstemp(char* template){
    PRINT(mkstemp, ('%s'), %d, int, template);
}

int putenv(char* string){
    PRINT(putenv, ('%s'), %d, int, string);
}

int rand(void){
    PRINT_NO_ARGS(rand, %d, int);
}

int rand_r(unsigned int *seedp){
    PRINT(rand_r, (%p), %d, int, seedp);
}

//DLSYM8(setenv, srand, system, chdir, chown, close, dup, dup2);
int setenv(const char *name, const char *value, int overwrite){
    PRINT(setenv, ('%s', '%s', %d), %d, int, name, value, overwrite);
}

void srand(unsigned int seed){
    PRINT_RET_VOID(srand, (%u), seed);
}

int system(const char *command){
    PRINT(system, ('%s'), %d, int, command);
}

int chdir(const char *path){
    PRINT(chdir, ('%s'), %d, int, path);
}

int chown(const char *pathname, uid_t owner, gid_t group){
    PRINT(chown, ('%s', %d, %d), %d, int, pathname, owner, group);
}

int close(int fd){
    PRINT(close, (%d), %d, int, fd);
}

int dup(int oldfd){
    PRINT(dup, (%d), %d, int, oldfd);
}
int dup2(int oldfd, int newfd){
    PRINT(dup2, (%d, %d), %d, int, oldfd, newfd);
}


//DLSYM8(fchown, fork, fsync, ftruncate, getcwd, getegid, geteuid, getgid);
int fchown(int fd, uid_t owner, gid_t group){
    PRINT(fchown, (%d, %d, %d), %d, int, fd, owner, group);
}

pid_t fork(void){
    PRINT_NO_ARGS(fork, %d, pid_t);
}

int fsync(int fd){
    PRINT(fsync, (%d), %d, int, fd);
}

int ftruncate(int fd, off_t length){
    PRINT(ftruncate, (%d, %ld), %d, int, fd, length);
}

char* getcwd(char *buf, size_t size){
    PRINT(getcwd, ('%s', %ld), '%s', char*, buf, size);
}

gid_t getegid(void){
    PRINT_NO_ARGS(getegid, %d, gid_t);
}

uid_t geteuid(void){
    PRINT_NO_ARGS(geteuid, %d, uid_t);
}

gid_t getgid(void){
    PRINT_NO_ARGS(getgid, %d, gid_t);
}

//DLSYM8(getuid, link, pipe, pread, pwrite, read, readlink, rmdir);

uid_t getuid(void){
    PRINT_NO_ARGS(getuid, %d, uid_t);
}

int link(const char *oldpath, const char *newpath){
    PRINT(link, ('%s', '%s'), %d, int, oldpath, newpath);
}

int pipe(int pipefd[2]){
    PRINT(pipe, (%p), %d, int, pipefd);
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset){
    PRINT(pread, (%d, %p, %lu, %lu), %ld, ssize_t, fd, buf, count, offset);
}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset){
    PRINT(pwrite, (%d, %p, %lu, %lu), %ld, ssize_t, fd, buf, count, offset);
}

ssize_t read(int fd, void *buf, size_t count){
    PRINT(read, (%d, %p, %lu), %ld, ssize_t, fd, buf, count);
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz){
    PRINT(readlink, ('%s', '%s', %lu), %ld, ssize_t, path, buf, bufsiz);
}

int rmdir(const char *pathname){
    PRINT(rmdir, ('%s'), %d, int, pathname);
}

//DLSYM8(setegid, seteuid, setgid, setuid, sleep, symlink, unlink, write);

int setegid(gid_t egid){
    PRINT(setegid, (%d), %d, int, egid);
}

int seteuid(uid_t euid){
    PRINT(seteuid, (%d), %d, int, euid);
}

int setgid(gid_t gid){
    PRINT(setgid, (%d), %d, int, gid);
}

int setuid(uid_t uid){
    PRINT(setuid, (%d), %d, int, uid);
}

unsigned int sleep(unsigned int seconds){
    PRINT(sleep, (%u), %u, unsigned int, seconds);
}

int symlink(const char *path1, const char *path2){
    PRINT(symlink, ('%s', '%s'), %d, int, path1, path2);
}

int unlink(const char *pathname){
    PRINT(unlink, ('%s'), %d, int, pathname);
}

ssize_t write(int fd, const void *buf, size_t count){
    PRINT(write, (%d, %p, %lu), %ld, size_t, fd, buf, count)
}

//DLSYM8(chmod, fchmod, fstat, lstat, mkdir, mkfifo, stat, umask);

int chmod(const char *path, mode_t mode){
    PRINT(chmod, ('%s', %d), %d, int, path, mode);
}

int fchmod(int fd, mode_t mode){
    PRINT(fchmod, (%d, %d), %d, int, fd, mode);
}

int mkdir(const char *path, mode_t mode){
    PRINT(mkdir, ('%s', %d), %d, int, path, mode);
}

int mkfifo(const char *pathname, mode_t mode){
    PRINT(mkfifo, ('%s', %d), %d, int, pathname, mode);
}

mode_t umask(mode_t mask){
    PRINT(umask, (%d), %d, mode_t, mask);
}
