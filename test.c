#include <stdio.h>
#include <fcntl.h>

int main(){
  int fd;
  fd = open("story.txt",O_RDONLY,0);
  while(read(fd,&i,sizeof(int)) != 0){
    printf("%d",i);
  }
  close(fd);
}
