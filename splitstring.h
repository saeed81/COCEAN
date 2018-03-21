#ifndef SPLISTRING_H
#define SPLISTRING_H
char **split(char *st, char delim, int *NF){
  
  int nc= 0;
  char *temp = st;

  while(*temp != '\0'){
    if ( *temp == delim){
      nc++;
      temp++;
    }
    else{
      temp++;
    }
  }
  *NF = (nc);
  char **cout = NULL;
  int ni = 0;
  int nj = -1;
  char *tt = st;
  if (nc > 0){
    cout = (char **) malloc(sizeof(char *)*(nc+1));
    temp = st;
    while(*temp != '\0'){
      if ( *temp == delim){
	nj++;
	cout[nj] = (char *)malloc(sizeof(char) *(ni+1));
	for (int i=0; i <ni;++i){
	  cout[nj][i] = *tt;
	  tt++;
	}
	cout[nj][ni] = '\0';
	ni=0;
	temp++;
	tt++;
      }
      else{
	temp++;
	ni++;
      }
    }
  }

  temp = st;
  nj = 0;
  ni = 0;
    
  while(*temp != '\0'){
      if ( *temp == delim){
	nj++;
	if (nj == nc){
	  tt = temp;
	  ni = 0;
	  tt++;
	  while(*tt != '\0'){
	    tt++;
	    ni++;
	  }
	  cout[nc] = (char*)malloc(sizeof(char)*(ni+1));
	  tt = temp;
	  ni = 0;
	  tt++;
	  while(*tt != '\0'){
	    cout[nc][ni] = *tt;
	    tt++;
	    ni++;
	  }
	  cout[nc][ni] = '\0';
	}
      }
      temp++;
  }
  return cout;
}
#endif   
