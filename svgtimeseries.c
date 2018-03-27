#include<stdio.h>
#include<stdlib.h>
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

  float height= 492.0;
  float width=761.0;
  
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
  vmin -= (0.25*vmin);
  vmax += (0.25*vmax);
  vmin  = -0.4;
  vmax  = 1.0;

  printf("min is %f and max is %f \n", vmin,vmax);
  for (int i=0; i < nl; ++i){
    ssh[i] = height-((height / (vmax - vmin)) * ssh[i] - ((height *vmin) / (vmax - vmin)));
    xa[i] = (width / (xa[nl-1] - xa[0]) )* xa[i] - ((width *xa[0]) / (xa[nl-1] - xa[0]));
  }
  
  #if 0
  for (int i=0; i < nl; ++i){
    printf("ssh[%d] %f\n",i,ssh[i]);
  }
  #endif
  create_svg(nl,xa,ssh);

  free(ssh);

  fclose(fin);

  return 0;

}

void create_svg(int N, float *x, float *y){
  
  FILE *fin = fopen("obs.svg","w");
  fprintf(fin,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n");
  fprintf(fin,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  fprintf(fin,"<svg height=\"492pt\" version=\"1.1\" viewBox=\"0 0 761 492\" width=\"761pt\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
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
  fprintf(fin," style=\"fill:none;stroke:#0000ff;\"");
  fprintf(fin,"/>\n");
  fprintf(fin,"</g>\n");
  fprintf(fin,"</svg>");
  fclose(fin);

}
