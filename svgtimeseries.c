#include<stdio.h>
#include<stdlib.h>
#include<math.h>
float sfmin(float *y, int N){
  float zm = y[0];
  for (int i=1; i < N; ++i){
    if (y[i] < zm) zm = y[i];
  }
  return zm;
}

float sfmax(float *y, int N){
  float zm = y[0];
  for (int i=1; i < N; ++i){
    if (y[i] > zm) zm = y[i];
  }
  return zm;
}

void create_svg(int, float*, float*);

int main(void){

  float height= 600.0;
  float width = 900.0;
  FILE *fin = fopen("barseback_ssh.dat","r");

  char c = 0;
  int nl = 0;

  while ((c = fgetc(fin)) != EOF){
    if (c == '\n') nl++; 
  }

  rewind(fin);

  float *ssh = (float *)malloc(sizeof(float) * nl);
  float *xa =  (float *)malloc(sizeof(float) * nl);
  float tmp = 0.0f;
  int ni = 0;
  while(fscanf(fin,"%f",&tmp) && !feof(fin)){
    ssh[ni] = tmp;
    printf("%f\n",ssh[ni]);
    xa[ni] = (float)(ni+1);
    ni++;
  }

  float vmin = sfmin(ssh, nl), vmax = sfmax(ssh, nl); 
  
  height -= 50.0;
  width -= 50.0;
  vmin += (0.2*vmin);
  vmax += (0.2*vmax);

  printf("min is %f and max is %f \n", vmin,vmax);
  for (int i=0; i < nl; ++i){
    ssh[i] = height-((height / (vmax - vmin)) * ssh[i] - ((height *vmin) / (vmax - vmin)));
    xa[i] = (width / (xa[nl-1] - xa[0]) )* xa[i] - ((width *xa[0]) / (xa[nl-1] - xa[0]));
  }
  
  create_svg(nl,xa,ssh);

  #if 0
  for (int i=0; i < nl; ++i){
    printf("ssh[%d] %f\n",i,ssh[i]);
  }
  
  float PI  = 4.0*atan(1.0);                                                                                                                                                            
  int   nx  = 100001;
  float dx  = (2.0*PI)/(nx -1);                                                                                                                                                         
  float *xx = (float*)malloc(nx *sizeof(float));                                                                                                                                      
  float *yy = (float*)malloc(nx *sizeof(float));                                                                                                                                     
  for (int i=0; i < nx;++i){                                                                                                                                                           
  xx[i]  = i*dx;                                                                                                                                                                        
  yy[i]  = sin(i*dx) * cos(i*dx);                                                                                                                                                       
}                                

  vmin = sfmin(yy, nx), vmax = sfmax(yy, nx); 
  vmin += (0.2*vmin);
  vmax += (0.2*vmax);
  
  for (int i=0; i < nx; ++i){
  yy[i] = height-((height / (vmax - vmin)) * yy[i] - ((height *vmin) / (vmax - vmin)));
  xx[i] = (width / (xx[nx-1] - xx[0]) )* xx[i] - ((width *xx[0]) / (xx[nx-1] - xx[0]));
}
  
  create_svg(nx,xx,yy);

  free(ssh);
  free(xa);
  free(xx);
  free(yy);

  #endif
  fclose(fin);

  return 0;

}

void create_svg(int N, float *x, float *y){
  
  FILE *fin = fopen("obs.svg","w");
  fprintf(fin,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n");
  fprintf(fin,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  fprintf(fin,"<svg height=\"600px\" version=\"1.1\" viewBox=\"0 0 900 600\" width=\"900px\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
  fprintf(fin,"<defs><style type=\"text/css\">*{stroke-linecap:square;stroke-linejoin:round;}</style></defs>");
  fprintf(fin,"<g id=\"line2d_1\"> <path d=\"\n");
  
  fprintf(fin,"M%f %f\n",x[0],y[0]);
  for (int i=1; i < N;++i){
    if (i == (N-1)){
      fprintf(fin,"L%f %f\"",x[i],y[i]);
    }
    else{
      fprintf(fin,"L%f %f\n",x[i],y[i]);
    }
  }
  fprintf(fin," style=\"fill:none;stroke:#ff0000;\"");
  fprintf(fin,"/>\n");
  fprintf(fin,"</g>\n");
  fprintf(fin,"</svg>");
  fclose(fin);

}
