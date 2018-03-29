#include<stdio.h>
#include<stdlib.h>

void sorti(int *x, int N){

  int *ind  = malloc(sizeof(int) *N);
  int *save = malloc(sizeof(int) *N);
  int *y    = malloc(sizeof(int) *N);
  
  for (int i=0; i < N; ++i){
    ind[i] = -1;
    save[i] = -1;
  }
  
  int zm = 0;
  int im = 0;
  int ni = 0;

  zm = x[0];
  for(int i=0; i < N;++i){
    if (x[i] <= zm){
      zm = x[i];
      im = i;
    }
  }  
  ind[im] = im;
  save[ni] = im;
  ++ni;
   
  for (int j=1; j < N; ++j){
    for (int i=0; i < N; ++i){
      if (ind[i] == -1){
	zm = x[i];
	im = i;
	break;
      }
    }
    for(int i=0; i < N;++i){
      if ((x[i] <= zm) && (ind[i] == -1)){
	zm = x[i];
	im = i;
      }
    }
    ind[im] = im;
    save[ni] = im;
    ++ni;
  }
  for (int i=0; i < N; ++i){
    y[i] = x[save[i]];
  }
  for (int i=0; i < N; ++i){
    x[i] = y[i];
  }
 
  free(ind);
  free(save);
  free(y);
}

int main(void){

  int x[]   = {2,2,2,1,6,5,7,9,25,23,21,19,2,17,17,100,202,1000,105,-1,-1,-1,-5,2050}; 
  
  int nx = sizeof(x) / sizeof(x[0]);

  for(int i=0; i <nx;++i) printf("%d\t",x[i]);
  printf("\n");

  sorti(x,nx);

  for(int i=0; i <nx;++i) printf("%d\t",x[i]);
  printf("\n");

  return 0;
}

