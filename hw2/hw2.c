#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>

FILE *file;
__attribute__((constructor)) void get_file_name(void){
	char *(*old_fun)(const char*) = NULL;
	void *handle = dlopen("libc.so.6", RTLD_LAZY);
	if(handle != NULL){
		old_fun = dlsym(handle, "getenv");
	}
    char *file_name=old_fun("MONITOR_OUTPUT");
	if(!strcmp(file_name,"stderr")){
		file=stderr;
	}
	else{
		file=fopen(file_name,"w+");
	}
}

#define old_function(output_type,fun_name,...)\
	static output_type (*old_##fun_name)(__VA_ARGS__) = NULL;
	
	
#define monitor_function(output_type,fun_name,parameter_type,output,...)\
	output_type r;\
	void *handle = dlopen("libc.so.6", RTLD_LAZY);\
	if(handle != NULL){\
		old_##fun_name = dlsym(handle, #fun_name);\
	}\
	if(old_##fun_name != NULL) {\
		r=old_##fun_name(__VA_ARGS__);\
		fprintf(file, "[monitor] "#fun_name"("parameter_type") = "output"\n",##__VA_ARGS__,r );\
	}\
	return r;
	
#define monitor_void_function(fun_name,parameter_type,...)\
	void *handle = dlopen("libc.so.6", RTLD_LAZY);\
	if(handle != NULL){\
		old_##fun_name = dlsym(handle, #fun_name);\
	}\
	if(old_##fun_name != NULL) {\
		old_##fun_name(__VA_ARGS__);\
		fprintf(file, "[monitor] "#fun_name"("parameter_type")\n",##__VA_ARGS__);\
	}\
	
old_function(uid_t,getuid,void);
old_function(uid_t,geteuid,void);
old_function(gid_t,getgid,void);
old_function(gid_t,getegid,void);
old_function(char*,getenv,const char*);
old_function(char*,getcwd,char*,size_t);
old_function(DIR*,opendir,const char*);
old_function(DIR*,fdopendir,int);
old_function(struct dirent*,readdir,DIR*);
old_function(int,readdir_r,DIR*,struct dirent*,struct dirent**);
old_function(int,closedir,DIR*);
old_function(int,mkdir,const char*,mode_t);
old_function(int,chdir,const char*);
old_function(int,fchdir,int);
old_function(int,rmdir,const char*);
old_function(void,rewinddir,DIR*);
old_function(void,seekdir,DIR*,long);
old_function(long,telldir,DIR*);
old_function(int,open,const char*,int,mode_t);
old_function(int,creat,const char*,mode_t);
old_function(int,remove,const char*);
old_function(int,rename,const char*,const char*);
old_function(void,setbuf,FILE*,char*);
old_function(int,setvbuf,FILE*,char*,int,size_t);
old_function(char*,tempnam,const char*,const char*);
old_function(FILE*,tmpfile,void);
old_function(char*,tmpnam,char*);
old_function(void,exit,int);
old_function(char*,mkdtemp,char*);
old_function(int,mkstemp,char*);
old_function(int,putenv,char*);
old_function(int,rand,void);
old_function(int,rand_r,unsigned int*);
old_function(void,srand,unsigned int);
old_function(int,setenv,const char*,const char*,int);
old_function(int,system,const char*);
old_function(int,chown,const char*,uid_t,gid_t);
old_function(int,fchown,int,uid_t,gid_t);
old_function(int,close,int);
old_function(int,dup,int);
old_function(int,dup2,int,int);
old_function(void,_exit,int);
old_function(int,execv,const char*, char*const[]);
old_function(int,execve,const char*, char*const[],char*const[]);
old_function(int,execvp,const char*, char*const[]);
old_function(pid_t,fork,void);
old_function(int,fsync,int);
old_function(int,ftruncate,int,off_t);
old_function(int,link,const char*,const char*);
old_function(int,pipe,int[2]);
old_function(ssize_t,pread,int,void*,size_t,off_t);
old_function(ssize_t,pwrite,int,const void*,size_t,off_t);
old_function(ssize_t,read,int,void*,size_t);
old_function(ssize_t,readlink,const char*,char*,size_t);
old_function(int,seteuid,uid_t);
old_function(int,setegid,gid_t);
old_function(int,setgid,gid_t);
old_function(int,setuid,uid_t);
old_function(unsigned int,sleep,unsigned int);
old_function(int,symlink,const char*,const char*);
old_function(int,unlink,const char*);
old_function(ssize_t,write,int,const void*,size_t);
old_function(int,chmod,const char*,mode_t);
old_function(int,fchmod,int,mode_t);
old_function(int,stat,const char*,struct stat*);
old_function(int,fstat,int,struct stat*);
old_function(int,lstat,const char*,struct stat*);
old_function(int,mkfifo,const char*,mode_t);
old_function(mode_t,umask,mode_t);
old_function(void*,realloc,void*,size_t);
old_function(int,__xstat,int,const char*,struct stat*);
old_function(int,__lxstat,int,const char*,struct stat*);
old_function(int,__fxstat,int,int,struct stat*);
//old_function(FILE*,fopen,const char*,const char*);


uid_t getuid(void) {	
	monitor_function(uid_t,getuid,"","%d");
}

uid_t geteuid(void) {
	monitor_function(uid_t,geteuid,"","%d");
}

gid_t getgid(void) {
	monitor_function(gid_t,getgid,"","%d");
}

gid_t getegid(void) {
	monitor_function(gid_t,getegid,"","%d");
}

char  *getenv(const char *name) {
	monitor_function(char*,getenv,"%s","%s",name);
}

char  *getcwd(char *buf,size_t size) {
	monitor_function(char*,getcwd,"%s,%d","%s",buf,size);
}

DIR  *opendir(const char *name) {
	monitor_function(DIR*,opendir,"%s","%p",name);
}

DIR  *fdopendir(int fd) {
	monitor_function(DIR*,fdopendir,"%d","%p",fd);
}

struct dirent  *readdir(DIR *dirp) {
	monitor_function(struct dirent*,readdir,"%p","%p",dirp);
}

int readdir_r(DIR *dirp,struct dirent *entry,struct dirent **result) {
	monitor_function(int,readdir_r,"%p,%p,%p","%d",dirp,entry,result);
}

int closedir(DIR *dirp) {
	monitor_function(int,closedir,"%p","%d",dirp);
}

int mkdir(const char *pathname, mode_t mode){
	monitor_function(int,mkdir,"%s,%d","%d",pathname,mode);
}

int chdir(const char *path){
	monitor_function(int,chdir,"%s","%d",path);
}

int fchdir(int fd){
	monitor_function(int,fchdir,"%d","%d",fd);
}

int rmdir(const char *pathname){
	monitor_function(int,rmdir,"%s","%d",pathname);
}

void rewinddir(DIR *dirp){
	monitor_void_function(rewinddir,"%p",dirp);
}

void seekdir(DIR *dirp, long loc){
	monitor_void_function(seekdir,"%p,%ld",dirp,loc);
}

long telldir(DIR *dirp){
	monitor_function(long,telldir,"%p","%ld",dirp);
}
/*
int open(const char *pathname, int flags){
	old_function(int,open,const char*,int);
	monitor_function(int,open,"%s,%d","%d",pathname,flags);
}
*/
int open(const char *pathname, int flags, mode_t mode){
	monitor_function(int,open,"%s,%d,%d","%d",pathname,flags,mode);
}

int creat(const char *pathname, mode_t mode){
	monitor_function(int,creat,"%s,%d","%d",pathname,mode);
}

int remove(const char *pathname){
	monitor_function(int,remove,"%s","%d",pathname);
}

int rename(const char *oldpath, const char *newpath){
	monitor_function(int,rename,"%s,%s","%d",oldpath,newpath);
}

void setbuf(FILE *stream, char *buf){
	monitor_void_function(setbuf,"%p,%s",stream,buf);
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size){
	monitor_function(int,setvbuf,"%p,%s,%d,%d","%d",stream,buf,mode,size);
}

char *tempnam(const char *dir, const char *pfx){
	monitor_function(char*,tempnam,"%s,%s","%s",dir,pfx);
}

FILE *tmpfile(void){
	monitor_function(FILE*,tmpfile,"","%p");
}

char *tmpnam(char *s){
	monitor_function(char*,tmpnam,"%s","%s",s);
}

void exit(int status){
	monitor_void_function(exit,"%d",status);
}

char *mkdtemp(char *template){
	monitor_function(char*,mkdtemp,"%s","%s",template);
}

int mkstemp(char *template){
	monitor_function(int,mkstemp,"%s","%d",template);
}

int putenv(char *string){
	monitor_function(int,putenv,"%s","%d",string);
}

int rand(void){
	monitor_function(int,rand,"","%d");
}

int rand_r(unsigned int *seedp){
	monitor_function(int,rand_r,"%p","%d",seedp);
}

void srand(unsigned int seed){
	monitor_void_function(srand,"%u",seed);
}

int setenv(const char *name, const char *value, int overwrite){
	monitor_function(int,setenv,"%s,%s,%d","%d",name,value,overwrite);
}

int system(const char *command){
	monitor_function(int,system,"%s","%d",command);
}

int chown(const char *path, uid_t owner, gid_t group){
	monitor_function(int,chown,"%s,%d,%d","%d",path,owner,group);
}

int fchown(int fd, uid_t owner, gid_t group){
	monitor_function(int,fchown,"%d,%d,%d","%d",fd,owner,group);
}

int close(int fd){
	monitor_function(int,close,"%d","%d",fd);
}

int dup(int oldfd){
	monitor_function(int,dup,"%d","%d",oldfd);
}

int dup2(int oldfd, int newfd){
	monitor_function(int,dup2,"%d,%d","%d",oldfd,newfd);
}

void _exit(int status){
	monitor_void_function(_exit,"%d",status);
}

//////////////////////////////
/////////////////////////
/////////////////////////
int execl(const char *path, const char *arg, ...){
	char *var_list[32];
	char *tmp;
	va_list args;
	va_start(args, arg);
	int s=0;
	tmp=arg;
	
	fprintf(file, "[monitor] execl(%s", path);
	while(tmp != NULL && s<32){
		var_list[s++] = tmp;
		fprintf(file, ",%s", tmp);
		
		tmp = va_arg(args,const char*);    // 指向下一個參數
    } 
    va_end(args);
	var_list[s] = (char *) 0;
	int r=execv(path, var_list);
	fprintf(file, ") = %d", r);
    return r;
}

int execlp(const char *f, const char *arg, ...){
	char *var_list[32];
	char *tmp;
	va_list args;
	va_start(args, arg);
	int s=0;
	tmp=arg;
	
	fprintf(file, "[monitor] execl(%s", f);
	while(tmp != NULL && s<32){
		var_list[s++] = tmp;
		fprintf(file, ",%s", tmp);
		
		tmp = va_arg(args,const char*);    // 指向下一個參數
    } 
    va_end(args);
	var_list[s] = (char *) 0;
	int r=execvp(f, var_list);
	fprintf(file, ") = %d", r);
    return r;
}

int execle(const char *path, const char *arg,...){
	char *var_list[32];
	char *var_list2[32];
	char *tmp;
	va_list args;
	va_start(args, arg);
	int s=0;
	tmp=arg;
	
	fprintf(file, "[monitor] execl(%s", path);
	while(tmp != NULL && s<32){
		var_list[s++] = tmp;
		fprintf(file, ",%s", tmp);
		
		tmp = va_arg(args,const char*);    // 指向下一個參數
    } 
	var_list[s] = (char *) 0;
	
	s=0;
	tmp = va_arg(args,const char*);
	while(tmp != NULL && s<32){
		var_list2[s++] = tmp;
		//fprintf(file, ",%s", tmp);
		
		tmp = va_arg(args,const char*);    // 指向下一個參數
    } 
	fprintf(file, ",%p", var_list2);
	var_list2[s] = (char *) 0;
	
    va_end(args);
	
	int r=execve(path, var_list,var_list2);
	fprintf(file, ") = %d", r);
    return r;
}

int execv(const char *path, char *const argv[]){
	monitor_function(int,execv,"%s,%p","%d",path,argv);
}

int execve(const char *filename, char *const argv[],char *const envp[]){
	monitor_function(int,execve,"%s,%p,%p","%d",filename,argv,envp);
}

int execvp(const char *f, char *const argv[]){
	monitor_function(int,execvp,"%s,%p","%d",f,argv);
}

pid_t fork(void){
	monitor_function(pid_t,fork,"","%d");
}

int fsync(int fd){
	monitor_function(int,fsync,"%d","%d",fd);
}

int ftruncate(int fd, off_t length){
	monitor_function(int,ftruncate,"%d,%ld","%d",fd,length);
}

int link(const char *oldpath, const char *newpath){
	monitor_function(int,link,"%s,%s","%d",oldpath,newpath);
}

int pipe(int pipefd[2]){
	monitor_function(int,pipe,"%p","%d",pipefd);
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset){
	monitor_function(ssize_t,pread,"%d,%p,%d,%ld","%d",fd,buf,count,offset);
}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset){
	monitor_function(ssize_t,pwrite,"%d,%p,%d,%ld","%d",fd,buf,count,offset);
}

ssize_t read(int fd, void *buf, size_t count){
	monitor_function(ssize_t,read,"%d,%p,%d","%d",fd,buf,count);
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz){
	monitor_function(ssize_t,readlink,"%s,%s,%d","%d",path,buf,bufsiz);
}

int seteuid(uid_t euid){
	monitor_function(int,seteuid,"%d","%d",euid);
}

int setegid(gid_t egid){
	monitor_function(int,setegid,"%d","%d",egid);
}

int setgid(gid_t gid){
	monitor_function(int,setgid,"%d","%d",gid);
}

int setuid(uid_t uid){
	monitor_function(int,setuid,"%d","%d",uid);
}

unsigned int sleep(unsigned int seconds){
	monitor_function(unsigned int,sleep,"%u","%u",seconds);
}

int symlink(const char *target, const char *linkpath){
	monitor_function(int,symlink,"%s,%s","%d",target,linkpath);
}

int unlink(const char *pathname){
	monitor_function(int,unlink,"%s","%d",pathname);
}

ssize_t write(int fd, const void *buf, size_t count){
	monitor_function(ssize_t,write,"%d,%p,%d","%d",fd,buf,count);
}

int chmod(const char *path, mode_t mode){
	monitor_function(int,chmod,"%s,%d","%d",path,mode);
}

int fchmod(int fd, mode_t mode){
	monitor_function(int,fchmod,"%d,%d","%d",fd,mode);
}

int stat(const char *path, struct stat *buf){
	monitor_function(int,stat,"%s,%p","%d",path,buf);
}

int fstat(int fd, struct stat *buf){
	monitor_function(int,fstat,"%d,%p","%d",fd,buf);
}

int lstat(const char *path, struct stat *buf){
	monitor_function(int,lstat,"%s,%p","%d",path,buf);
}

int mkfifo(const char *pathname, mode_t mode){
	monitor_function(int,mkfifo,"%s,%d","%d",pathname,mode);
}

mode_t umask(mode_t mask){
	monitor_function(mode_t,umask,"%d","%d",mask);
}

void *realloc(void *ptr, size_t size){
	monitor_function(void*,realloc,"%p,%d","%p",ptr,size);
}

int __xstat(int ver, const char * path, struct stat * stat_buf){
	monitor_function(int,__xstat,"%d,%s,%p","%d",ver,path,stat_buf);
}

int __lxstat(int ver, const char * path, struct stat * stat_buf){
	monitor_function(int,__lxstat,"%d,%s,%p","%d",ver,path,stat_buf);
}

int __fxstat(int ver, int fildes, struct stat * stat_buf){
	monitor_function(int,__fxstat,"%d,%d,%p","%d",ver,fildes,stat_buf);
}
/*
FILE *fopen(const char *pathname, const char *mode){
	monitor_function(FILE*,fopen,"%s,%s","%p",pathname,mode);
}*/