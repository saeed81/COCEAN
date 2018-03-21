#ifndef STRTOINT_H
#define STRTOINT_H
int pow10(int n){
  int d = 1;
  if (n > 0){ 
    for (int i=0; i < n;++i) d *= 10;
  }
  return d;
}

int stoi(char *st){


  int sign = 0;

  int nc=0;
  char *temp = st;

  while (*temp != '\0' ){
    if ( *temp != ' ') nc++;
    temp++;
  }
  nc--;
  int d = 0;
  int c = 1;
  while (*st != '\0'){
    if (*st == '-'){
      c = -1;
      nc--; 
      st++;
      sign += 1;
      if (sign > 1 ) return 0;
    }
    else if (*st == '+'){
      c = 1;
      nc--; 
      st++;
      sign += 1;
      if (sign > 1 ) return 0;
    }
    else if( *st != ' ') {
      d += ((int)(*st - '0')*pow10(nc));
      st++;
      nc--;
    }
    else {
      st++;
    }
  }

  return (c*d);
}
#endif  
