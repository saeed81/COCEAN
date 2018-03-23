#ifndef STRTOF
#define STRTOF
int pow10(int n){
  int d = 1;
  if (n > 0){ 
    for (int i=0; i < n;++i) d *= 10;
  }
  return d;
}

float npow10(int n){
  float d = 1.0;
  if (n > 0){ 
    for (int i=0; i < n;++i) d /= 10.0;
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

float stof(char *st){
  
  int sign = 0;
  int nc=0;
  char *temp = st;
  char *tint = st;

  while (*temp != '\0'){
    if ( *temp != ' '){
      if (*temp == '.'){
	break;
      }
      else{
	nc++;
      }
    }
    temp++;
  }
  nc--;
  float d = 0.0;
  int c = 1;
  while (*tint != '\0'){
    if (*tint == '-'){
      c = -1;
      nc--; 
      tint++;
      sign += 1;
      if (sign > 1 ) return 0.0;
    }
    else if (*tint == '+'){
      c = 1;
      nc--; 
      tint++;
      sign += 1;
      if (sign > 1 ) return 0.0;
    }
    else if( *tint != ' ') {
      if (*tint == '.') break;
      d += (float)((int)(*tint - '0')*pow10(nc));
      tint++;
      nc--;
    }
    else {
      tint++;
    }
  }

  char *tm = st;

  while (*tm != '\0'){

    if (*tm == '.') break;
    tm++;
  }
  char *tm1 = (++tm);
  nc=0;
  float df = 0.0;
  while (*tm1 != '\0'){
    if ( *tm1 != ' ') {
    nc++; 
    df += (float)((int)(*tm1 - '0')*npow10(nc));
    tm1++;
    }
  }
  
  return (c*(d + df));
}
#endif
