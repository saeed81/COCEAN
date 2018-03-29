#ifndef SORT_H
#define SORT_H
void sortpart(int *x, float *z, int ia, int ib){
  int N = ib -ia +1;
  int *ind  = malloc(sizeof(int) *N);
  int *save = malloc(sizeof(int) *N);
  int *y    = malloc(sizeof(int) *N);
  float *yz = malloc(sizeof(float)*N);
  
  for (int i=0; i < N; ++i){
    ind[i] = -1;
    save[i] = -1;
  }
  
  int zm = 0;
  int im = 0;
  int ni = 0;

  zm = x[0];
  for(int i=ia; i <= ib;++i){
    if (x[i] <= zm){
      zm = x[i];
      im = i;
    }
  }  
  ind[im] = im;
  save[ni] = im;
  ++ni;
   
  for (int j=(ia+1); j <= ib; ++j){
    for (int i=ia; i <= ib; ++i){
      if (ind[i-ia] == -1){
	zm = x[i];
	im = i;
	break;
      }
    }
    for(int i=ia; i <= ib;++i){
      if ((x[i] <= zm) && (ind[i-ia] == -1)){
	zm = x[i];
	im = i;
      }
    }
    ind[im] = im;
    save[ni] = im;
    ++ni;
  }
  for (int i=ia; i <= ib; ++i){
    y[i-ia]  = x[save[i-ia]];
    yz[i-ia] = z[save[i-ia]];
    
  }
  for (int i=ia; i <= ib; ++i){
    x[i] = y[i-ia];
    z[i] = yz[i-ia];
  }
 
  free(ind);
  free(save);
  free(y);
  free(yz);
}
void sortfull(int *x, float *z, int N){

  int   *ind  = malloc(sizeof(int)  *N);
  int   *save = malloc(sizeof(int)  *N);
  int   *y    = malloc(sizeof(int)  *N);
  float *yz   = malloc(sizeof(float)*N);
  
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
    y[i]  = x[save[i]];
    yz[i] = z[save[i]];
  }
  for (int i=0; i < N; ++i){
    x[i] = y[i];
    z[i] = yz[i];
    
  }
 
  free(ind);
  free(save);
  free(y);
  free(yz);
}
#endif
