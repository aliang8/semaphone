#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  int c;
  FILE *fp;
  fp = fopen("story.txt","r");
  if (fp){
    while ((c = getc(fp)) != EOF)
      putchar(c);
    fclose(fp);
  }
}
