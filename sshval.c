#include<stdio.h>
#include<stdlib.h>

long int numLines(FILE *FS){

  long int nl = 0L;
  int c = 0;
  while((c=getc(FS)) != EOF){
    if (c=='\n'){
      nl += 1;
    }
  }
  rewind(FS);
  return nl;
}

int Strlen(char *sa){
  int len = 0;
  while(*sa != '\0'){ 
    len += 1;
    sa++;
  }
  return len;
}

int Strcmp(char *sa, char *sb){
  
  if (Strlen(sa) != Strlen(sb)){
    return 0;
  }
  while ((*sa == *sb) && *sa != '\0' && *sb != '\0' ){
    sa++;
    sb++;
  }
  
  if (*sa == '\0'){
    return 1;
  }
  else{
    return 0;
  }
}

int main(void){
  
  FILE *FS = NULL;
  FS = fopen("ssh_ns6_minh_4.csv","r");
  if (FS == NULL){
    printf("error in opening the file \n");
    return 1;
  }
  long int nline = numLines(FS);
  
  char cc = ' ';
  char field[128] ="";
  int ne = 0;
  int nfl = 0;
  int nl = 0;
  //Here we calculate the number of the field
  while ((cc=fgetc(FS)) != EOF){
    if (cc != ','){
      if(cc=='\n'){
	nl += 1;
	if (nl == 1) break;
      }
    }
    else{
      nfl += 1;
    }
  }

  printf("number of fields is %d \n", nfl);

  rewind(FS);

  char **cfl = malloc(sizeof(char *)*nfl);
  
  nfl = 0;
  nl  = 0;
  ne  = 0;

  while ((cc=fgetc(FS)) != EOF){
    if (cc != ','){
      if(cc=='\n'){
	printf("=================end of line =========================\n");
	ne = 0;
	nl += 1;
	if (nl == 1) break;
      }
      else {
	field[ne] = cc;
	ne += 1;
      }
    }
    else{
      cfl[nfl] = malloc(sizeof(char) *ne);
      for(int ii=0; ii < ne; ++ii) cfl[nfl][ii] = field[ii];
      printf("%s \n",cfl[nfl]);
      nfl += 1;
      for(int ii=0; ii < ne; ++ii) field[ii] = ' ';
      ne = 0;
    }
  }

  for(int ii=0; ii < 128; ++ii) field[ii] = ' ';

  //now we read date time;
  ne = 0;
  int ifld = 0;
  float *content = malloc(sizeof(float) *(nline -1));
  nl = 0;
  for (int i=0; i < nfl; ++i){
    if (Strcmp(cfl[i],"ystad_sjov")){
      int ic = i;
      printf("%d \t %s\n",i,cfl[i]);
      while ((cc=fgetc(FS)) != EOF){
	if (cc != ','){
	  if(cc=='\n'){
	    ne = 0;
	    nl += 1;
	    ifld = 0;
	  }
	  else{
	    if (ic == ifld ) {field[ne] = cc;ne += 1;}
	  }
	}
	else{
	  if (ic == ifld ) {
	    content[nl] = atof(field);
	    printf("%f\n",atof(field));
	  }
	  ifld += 1;
	  ne=0;
	}
      }
    }
  }
  fclose(FS);
  
  for(int i=0; i < (nline-1);++i){
   printf("%f\n",content[i]);
  }

  free(cfl);
  
  free(content);

  return 0;
}
