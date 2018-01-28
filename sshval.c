#include<stdio.h>
#include<stdlib.h>

long int numLines(FILE *FS){

  long int nl = 0L;
  int c = 0;
  while((c=getc(FS)) != EOF){
    if (c=='\n'){
      nl += 1;
      //printf("line %ld \n",nl);
    }
  }
  rewind(FS);
  return nl;
}

int Strlen(char *sa){
  int len = 0;
  while(*sa++) len += 1;
  
  return len;
}

int Strcmp(char *sa, char *sb){

  
  if (Strlen(sa) != Strlen(sb)){
    //printf("input strings are not of the same size\n");
    return 0;
  }
  while ((*sa == *sb) && *sa != '\0' && *sb != '\0' ){
    //printf("%c\t%c\n",*sa,*sb);
    sa++;
    sb++;
  }
  
  if (*sa == '\0'){
    //printf("string sa reached its end so it means that sa is equal sb\n");
    return 1;
  }
  else{
    //printf("search ended at %c\n",*sa);
    return 0;
  }
}

int main(void){
  
  
  char fname[]  = "saeee";
  char fullname[] = "saeed";
  char *lastname  = "sblahat";

  //printf("len is %d\n",Strlen(fname));
  //printf("len is %d\n",Strlen(fullname));

  Strcmp(fname,fullname);
  Strcmp(lastname,fullname);

  FILE *FS = NULL;
  FS = fopen("ssh_ns6_minh_4.csv","r");
  if (FS == NULL){
    printf("error in opening the file \n");
    return 1;
  }

  long int nline = numLines(FS);
  //printf("number of lines is %ld \n",nline);

  char cc = ' ';
  char field[128] ="";
  int ne = 0;
  int nfl = 0;
  int nl = 0;
  //Here we read the header
  while ((cc=fgetc(FS)) != EOF){
    if (cc != ','){
      if(cc=='\n'){
	//printf("=================end of line =========================\n");
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
      nfl += 1;
      //printf("%s \n",field);
      for(int ii=0; ii < ne; ++ii) field[ii] = ' ';
      ne = 0;
      }
    }

  printf("number of fields is %d \n", nfl);


  rewind(FS);

  char **cfl = malloc(sizeof(char *)*nfl);
  nfl = 0;
  nl = 0;
  ne = 0;

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

  
  //now we read date time;
  nfl = 0;
  ne = 0;
  for (int i=0; i < nfl; ++i){
    if (Strcmp(cfl[i],"datetime")){
      printf("%d \t %s\n",i,cfl[i]);
      while ((cc=fgetc(FS)) != EOF){
	if (cc != ','){
	  if(cc=='\n'){
	    printf("=================end of line =========================\n");
	    ne = 0;
	    nl += 1;
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

  }


  fclose(FS);

  free(cfl);

  return 0;
}
