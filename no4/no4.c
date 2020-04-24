#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
static const char * dirpath = "/home/bagasimmanuel/Documents/Fuse";

void toLogFile(char* level,char* arg,int n,char* path){

  FILE *f;
  char log[1000];
  char waktu[100];
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  sprintf(waktu,"%d%02d%02d-%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  if(n == 1){
    sprintf(log,"%s::%s::%s\n",waktu,arg,path);
  }else if(n == 2){
    char bufferPath[1000];
    strcpy(bufferPath,path);
    char *token = strtok(path,"!");
    sprintf(log,"%s::%s",waktu,arg);
    while(token != NULL){
      char buffer[1000];
      strcpy(buffer,token);
      sprintf(log,"::%s",token);
      token = strtok(NULL, "!");
    }
    sprintf(log,"\n");
  }

  f = fopen("/home/bagasimmanuel/fs.log","a");
  if(f == NULL)
    return;
  fputs(log,f);

  fclose(f);



}


void reverse(char * x, int begin, int end) {
  char c;
  if (begin >= end) return;
  c = * (x + begin);
  *(x + begin) = * (x + end);
  *(x + end) = c;
  reverse(x, ++begin, --end);
}

void reversePath(const char *path, char bufferKeluar[]) {
  char bufferPath[1000];
  strcpy(bufferPath, path);
  char bufferOut[1000] = "";
  char *token = strtok(bufferPath, "/");
  while(token != NULL) {
    char buffer[1000];
    strcpy(buffer, token);
    reverse(buffer, 0, strlen(buffer)-1);
    sprintf(bufferOut, "%s/%s", bufferOut, buffer);

    token = strtok(NULL, "/");
  }
  strcpy(bufferKeluar, bufferOut);
}

// static int _write(const char* pathname, char *buf, size_t size, off_t offset, struct fuse_file_info* fi){
//
//
//   char fpath[1000];
//   sprintf(fpath,"%s/%s",dirpath,pathname);
//
//   int res = 0;
//   int fd1;
//   if (fi == NULL)
//     fd1 = open(fpath, O_WRONLY);
//   else
//     fd1 = fi->fh;
//   int fd2 = open(fpath2, O_WRONLY | O_CREAT);
//
//   int res = pwrite(fd1, buf, size, offset);
//   pwrite(fd2, buf, size, offset);
//   chmod(fpath2, 0);
//   if (fi == NULL)
//     close(fd1);
//   close(fd2);
//
//   if (res == -1) res = -errno;
//   return res;
//
//   char pathName[1000];
//   sprintf(pathName,"%s",pathname);
//   toLogFile("INFO","WRITE",1,pathName);
//   return 0;
//
//
//
//
//
//
//
// }

static int _write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{

  // Dari Passthroguh.c
	int fd;
	int res;

  char fpath[1000];
  sprintf(fpath,"%s/%s",dirpath,path);

	(void) fi;
	if(fi == NULL)
		fd = open(fpath, O_WRONLY);
	else
		fd = fi->fh;

	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	if(fi == NULL)
		close(fd);
    char pathName[1000];
    sprintf(pathName,"%s",path);
    toLogFile("INFO","WRITE",1,pathName);
	return res;
}

static int _create(const char *path, mode_t mode, // Passthroguh BEBIIIII
		      struct fuse_file_info *fi)
{


  char fpath[1000];
  sprintf(fpath,"%s/%s",dirpath,path);
	int res;
	res = open(fpath, fi->flags, mode);
	if (res == -1)
		return -errno;

  char pathName[1000];
  sprintf(pathName,"%s",path);
  toLogFile("INFO","CREATE",1,pathName);

	fi->fh = res;
	return 0;
}

static int _utimens(const char *path, const struct timespec ts[2]// Passthroguh BEBIIII
		       )
{

	int res;
  char fpath[1000];
  sprintf(fpath,"%s/%s",dirpath,path);
	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}

static int _mkdir(const char *path, mode_t mode){

  char fpath[1000];
  sprintf(fpath,"%s/%s",dirpath,path);

  int res = 0;
  res = mkdir(fpath,mode);
  if(res == -1){
    return errno;
  }
  char pathName[1000];
  sprintf(pathName,"%s",path);
  toLogFile("INFO","MKDIR",1,pathName);
  return 0;
}

static int _rename(const char* from, const char* to){

  int res;
  char fpathFrom[1000],fpathTo[1000];
  sprintf(fpathFrom,"%s/%s",dirpath,from);
  sprintf(fpathTo,"%s/%s",dirpath,to);
	res = rename(fpathFrom, fpathTo);
	if (res == -1)
		return -errno;

  char pathName[1000];
  sprintf(pathName,"%s!%s!",from,to);
  toLogFile("INFO","RENAME",2,pathName);

	return 0;

}

static int _unlink(const char *path)
{
	int res;
  char fpath[1000];
  sprintf(fpath,"%s/%s",dirpath,path);
	res = unlink(fpath);
	if (res == -1)
		return -errno;

  char pathName[1000];
  sprintf(pathName,"%s",path);
  toLogFile("WARNING","UNLINK",1,pathName);

	return 0;
}



static int _rmdir(const char* path){


  char rmPath[1000];
  sprintf(rmPath,"%s/%s",dirpath,path);

  int res = 0;
  res = rmdir(rmPath);
  if(res == -1){
    return errno;
  }
  char pathName[1000];
  sprintf(pathName,"%s",path);
  toLogFile("WARNING","RMDIR",1,pathName);
  return 0;

}

static int _getattr(const char * path, struct stat * stbuf) {

  int res;
  char fpath[1000];

  if (strcmp(path, "/") == 0) {
    path = dirpath;
    sprintf(fpath, "%s", path);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }

  res = lstat(fpath, stbuf);

  if (res == -1) return -errno;

  char pathName[100];
  sprintf(pathName,"%s",path);
  toLogFile("INFO","ATTR",1,pathName);

  return 0;

}

static int _readdir(const char * path, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi) {

  char fpath[1000];

  if (strcmp(path, "/") == 0) {
    path = dirpath;
    sprintf(fpath, "%s", path);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }

  int res = 0;

  DIR * dp;
  struct dirent * de;
  (void) offset;
  (void) fi;

  dp = opendir(fpath);
  if (dp == NULL) return -errno;

  while ((de = readdir(dp)) != NULL) {

    struct stat st;
    memset( & st, 0, sizeof(st));

    st.st_ino = de -> d_ino;
    st.st_mode = de -> d_type << 12;

    char str[1000];
    strcpy(str, de -> d_name);

    res = (filler(buf, str, & st, 0));

    toLogFile("INFO","READDIR",1,str);
    if (res != 0) break;

  }

  closedir(dp);

  return 0;

}

static int _read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi) {

  char fpath[1000];

  if (strcmp(path, "/") == 0) {
    path = dirpath;
    sprintf(fpath, "%s", path);
  } else {
    sprintf(fpath, "%s%s", dirpath, path);
  }

  int res = 0;
  int fd = 0;
  (void) fi;

  fd = open(fpath, O_RDONLY);

  if (fd == -1) return -errno;

  res = pread(fd, buf, size, offset);

  if (res == -1)res = -errno;

  close(fd);

  char pathName[100];
  sprintf(pathName,"%s",path);
  toLogFile("INFO","READ",1,pathName);

  return res;

}

static struct fuse_operations xmp_oper = {
  .getattr = _getattr,
  .readdir = _readdir,
  .read = _read,
  .mkdir = _mkdir,
  .rmdir = _rmdir,
  .create = _create,
  .utimens = _utimens,
  .write = _write,
  .unlink = _unlink,
  .rename = _rename,
//  .write = _write,
};

int main(int argc, char * argv[]){
  umask(0);
  return fuse_main(argc, argv, & xmp_oper, NULL);
}
