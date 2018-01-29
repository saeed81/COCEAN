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

int main(int argc, char *argv[]){
  
  
  if (argc != 3){
    printf("Usage %s filename (stationame)\n",argv[0]);
    return 1;
  }
    
  char *filename = argv[1];
  char *station  = argv[2];

  FILE *FS = NULL;
  FS = fopen(filename,"r");
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
  //Here we calculate the number of the field from the header
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
      nfl += 1;
      for(int ii=0; ii < 127; ++ii) field[ii] = ' ';
      ne = 0;
    }
  }

  for(int ii=0; ii < 127; ++ii) field[ii] = ' ';

  //now we read date time;
  ne = 0;
  int ifld = 0;
  nl = 0;
  int *icontent = NULL;
  float *fcontent = NULL;
  int ic = 0;
  for (int i=0; i < nfl; ++i){
    if (Strcmp(cfl[i],station)){
      ic = i;
      if (ic == 0){
	 icontent = malloc(sizeof(int) *(nline -1));
      }
      if (ic > 0) {
	fcontent = malloc(sizeof(float) *(nline -1));
      }
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
	    if (ic == 0)icontent[nl] = atoi(field);
	    if (ic >  0)fcontent[nl] = atof(field);
	  }
	  ifld += 1;
	  ne=0;
	}
      }
    }
  }
  fclose(FS);
  
  for(int i=0; i < (nline-1);++i){
    if (ic == 0 && icontent[i] != 0 )printf("%d\n",icontent[i]);
    if (ic > 0)printf("%f\n",fcontent[i]);
  }

  free(cfl);
  
  if (icontent != NULL )free(icontent);
  if (fcontent != NULL )free(fcontent);

  return 0;
}
