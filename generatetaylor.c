#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<stdarg.h>
#include"stringutil.c"


int getLen(char *str){

  int len = 0;
  while(*str++){
    len += 1;
  }
  return len;
}


void copycontent(char *filename, FILE *file){
  if (filename == NULL || file == NULL) return;
  FILE *fin = fopen(filename,"r");
  if (fin == NULL) return;
  int cc = 0;
  while((cc = fgetc(fin)) != EOF){
    fprintf(file,"%c",cc);
  }
  fclose(fin);
}

int stringfind(char *src, char *sub){
  
  int isexist = 0;
  if ((src == NULL) || (sub == NULL) )return isexist;

  int len1  = 0, len2 = 0;
  char *t1  = src;
  char *t2  = sub;
  
  while(*t1 != '\0'){
    len1++;
    t1++;
  }
  while(*t2 != '\0'){
    len2++;
    t2++;
  }
  if (len2 > len1 )return isexist;
  t1 = src;
  t2 = sub;
  
  while(*t1 != '\0'){
    t2 = sub;
    if (*t1 == *t2){
      isexist = 1;
      char *next =  t2+1;
      char *ts   =  t1+1;
      while (*next != '\0' && *ts != '\0'){
        if (*next != *ts ){
          isexist = 0;
          break;
        }
	next++;
        ts++;
      }
      if (isexist == 1) break;
    }
    
    t1++;
  }
  
  return isexist; 
}



char *HEADER = "<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<style>\n.slidecontainer{margin:0px auto;width:10%;height:100px;}\n</style>\n</head>\n<body>";


void list_dir(char *path){
  struct dirent *entry;
  DIR *dir = opendir(path);
  if (dir == NULL) {
    return;
  }
  
  FILE *file = NULL;
  file = fopen("indexs.html","w+");
  fprintf(file,"%s\n",HEADER);
  fprintf(file,"<div id=\"demo\" style=\"margin:0 auto;width:70%%;height:700px;\">\n");
  
  int ncount = 0;
  while ((entry = readdir(dir)) != NULL) {
    if ( stringfind(entry->d_name,".png") && stringfind(entry->d_name,"ssh")){
      char imgid[8] = {'\0'};
      intostr(ncount, imgid,0);
      char idimage[128] = {'\0'};
      writetostring(idimage,128,"im",imgid,NULL);
      if (ncount == 0)fprintf(file,"<img id=\"%s\" src=\"%s\" alt=\"taylor\" style=\"width:100%%;height:100%%;display:block;\">\n",idimage,entry->d_name);
      if (ncount >  0)fprintf(file,"<img id=\"%s\" src=\"%s\" alt=\"taylor\" style=\"width:100%%;height:100%%;display:none;\">\n",idimage,entry->d_name);
      printf("%s\n",idimage);
      ncount++;
    }
  }
  fprintf(file,"</div>\n");
  fprintf(file,"<div class=\"slidecontainer\">\n");
  fprintf(file,"<input type=\"range\" min=\"1\" max=\"%d\" value=\"1\" id=\"myrange\" oninput=\"doit(%d)\">\n",ncount-1,ncount);
  fprintf(file,"</div>\n");
  copycontent("jsfile",file);
  fprintf(file,"</body>\n");
  fprintf(file,"</html\n");
  
  fclose(file);
  closedir(dir);
}

int main(void){
  
  
  list_dir(".");

  

  return 0;
}





