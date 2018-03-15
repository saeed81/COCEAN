#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"svgplot.h"

#define LF 32

long int numLines(FILE *FS){
  long int nl = 0L;
  int c = 0;
  while((c=fgetc(FS)) != EOF){
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
    
  if (argc != 5){
    printf("Usage %s filename (stationame)\n",argv[0]);
    return 1;
  }
    
  char *filename = argv[1];
  char *station  = argv[2];
  char *datebeg  = argv[3];
  char *datend   = argv[4];
  int idbeg = atoi(datebeg);
  int idend = atoi(datend);

  FILE *FS = NULL;
  FS = fopen(filename,"r");
  if (FS == NULL){
    printf("error in opening the file %s\n", filename);
    return 1;
  }

  long int nline = numLines(FS);
  
  char cc = ' ';
  char field[LF] ="";
  char ifield[LF] ="";
  int ne = 0;
  int nfl = 0;
  int nl = 0;
  //Here we calculate the number of the field from the CSV header
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
  //printf("number of fields is %d \n", nfl);
  rewind(FS);

  char **cfl = malloc(sizeof(char *)*nfl);  // each field is a array of character array
  
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
      for(int ii=0; ii < (LF-1); ++ii) field[ii] = ' ';
      ne = 0;
    }
  }

  for(int ii=0; ii < (LF -1); ++ii) field[ii] = ' ';

  //now we read date time;
  ne = 0;
  nl = 0;
  int nit = 0;
  int ifld = 0;
  int *icontent = NULL;
  float *fcontent = NULL;
  int ic = 0;
  for (int i=0; i < nfl; ++i){
    if (Strcmp(cfl[i],station)){
      ic = i;
      icontent = malloc(sizeof(int) *(nline -1));
      fcontent = malloc(sizeof(float) *(nline -1));
      while ((cc=fgetc(FS)) != EOF){
	if (cc != ','){
	  if(cc=='\n'){
	    ne = 0;
	    nit = 0;
	    nl += 1;
	    ifld = 0;
	  }
	  else{
	    if (ic == ifld)  {field[ne]   = cc; ne  += 1;}
	    if (ifld == 0)   {ifield[nit] = cc; nit += 1;}
	  }
	}
	else{
	  if (ifld == 0)  icontent[nl] = atoi(ifield);
	  if (ic == ifld )fcontent[nl] = atof(field);
	  ifld += 1;
	  ne=0;
	}
      }
    }
  }
  
  if (icontent == NULL || fcontent == NULL){ 
    printf("something went wrong. Perhaps the station %s is not in the csv file %s\n",station,filename);
    return 1;
    fclose(FS);
  }
  fclose(FS);

  int ii = 0;
  int ia = 0, ib = 0;
  for(ii=0; ii < (nline-1);++ii){
    if (icontent[ii] == 0 ) break; 
  }

  for(ia=0; ia < (ii+1);++ia){
    if (icontent[ia] == idbeg) break; 
  }
  
  for(ib=0; ib < (ii+1);++ib){
    if (icontent[ib] == idend) break; 
  }

  printf("%d\t%ld\t%d\t%d\n",ii,(nline -1),ia,ib); 


  for (int i=ia; i <= ib;++i){
    printf("%d\t%f\n",icontent[i],fcontent[i]);
  }


  float rmax = sfmax(fcontent,ii+1);
  float rmin = sfmin(fcontent,ii+1);
 
  //printf("%f\t%f\n",rmin,rmax); 
  svgmeta svgm;                                                                                                                                                                                              
  
  FILE * filsvg = svg_init(900, 600, 0.0,(float)(ii+1),rmin,rmax,"ssh.html",&svgm);

  float *xax = (float *) malloc(sizeof(float) *(ii+1));

  for (int i=0; i < (ii+1);++i) xax[i] = (float)(i);
  
  if ( filsvg != NULL){                                                                                                                                                                                        
    svg_add(filsvg,xax,fcontent,(ii+1),&svgm);
    svg_close(filsvg);                                                                                                                                                                                            
  }                                     
      
  free(cfl);
  free(icontent);
  free(fcontent);
    
  return 0;
}
