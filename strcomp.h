#ifndef STRCOMP_H
#define STRCOMP_H

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


void ltrim(char *st, char delim){

  while(*st != '\0'){
    if (*st == delim) break;
    st++;
  }
  char *tm = st;
  while( *tm != '\0'){
  *st = *(tm+1);
  st++;
  tm++;
  }
}




void rtrim(char *st, char delim){

  while(*st != '\0'){
    if (*st == delim) *st = '\0';
    st++;
  }
} 


#endif




