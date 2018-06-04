#ifndef SFLIB_H
#define SFLIB_H

char *readlinesfromfile(char *fname, long int *len){

  char *content = NULL;
  FILE *fin = NULL;
  fin = fopen(fname,"r");
  *len = -1;
  if (fin != NULL){
    long int fs = 0L;
    fseek(fin,0,SEEK_END);
    fs = ftell(fin);
    fseek(fin,0,SEEK_SET);
    content = (char *)malloc(fs+1);
    fread(content,1,fs,fin);
    content[fs] = '\0';
    *len = (fs +1);
    fclose(fin);
  }
  
  return content;
}

#endif
