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

void create_svg_line_chart_line_chart(int, float*, float*);
void create_svg_contour_init(FILE **);
void create_svg_contour_add(FILE *, float *, float *, int *);
void create_svg_contour_close(FILE *);

int main(void){

  float height= 600.0;
  float width = 900.0;
  //we create topography contour like a circular seamount 
  // x * x + y * y
  int npoint  = 40001;
  float *ax   = (float*)malloc(npoint *sizeof(float));
  float *ay   = (float*)malloc(npoint *sizeof(float));
  float **az  = NULL;
  float delta = (4.0) / (npoint -1);
  for (int i=0; i < npoint;++i){                                                                                                                                                       
    ax[i] = i*delta;                                                                                                                                                                
    ay[i] = i*delta;                                                                                                                                                       
  }                                
  az = (float **)malloc(sizeof(float*)*npoint);
  for (int i=0; i < npoint;++i){
    az[i] = (float*)malloc(sizeof(float)*npoint);
  }
  for (int i=0; i < npoint; ++i){
    for (int j=0; j < npoint; ++j){
      az[i][j] = ax[i] * ax[i] + ay[j] * ay[j];
    }
  }
  for (int i=0; i < npoint; ++i){
    for (int j=0; j < npoint; ++j){
      if ( az[i][j] == 4.0f){
	printf("%f\t%f\t%f\n",ax[i],ay[j],az[i][j]);
      }
    }
  }
  
  for (int i=0; i < npoint; ++i){
    ax[i] = (width /(ax[npoint-1] -ax[0]))*ax[i] - ((width *ax[0]) / (ax[npoint-1] - ax[0]));
    ay[i] = (height/(ay[npoint-1]-ay[0]))* ay[i] - ((height *ay[0])/ (ay[npoint-1] - ay[0]));
  }
  int begin = 0;
  FILE *fin = NULL;
  create_svg_contour_init(&fin);
  create_svg_contour_add(fin,ax,ay,&begin);  
  for (int i=0; i < npoint; ++i){
    for (int j=0; j < npoint; ++j){
      if ( az[i][j] == 4.0f){
	create_svg_contour_add(fin,(ax+i),(ay+j),&begin);  
      }
    }
  }
  create_svg_contour_close(fin);

  for (int i=0; i < npoint; i++){
    free(az[i]);
  }
  free(ax);
  free(ay);
  free(az);
#if 0
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
  fclose(fin);
  float vmin = sfmin(ssh, nl), vmax = sfmax(ssh, nl); 
  
  height -= 50.0;
  width  -= 50.0;
  vmin += (0.2*vmin);
  vmax += (0.2*vmax);

  printf("min is %f and max is %f \n", vmin,vmax);
  for (int i=0; i < nl; ++i){
    ssh[i] = height-((height / (vmax - vmin)) * ssh[i] - ((height *vmin) / (vmax - vmin)));
    xa[i] = (width / (xa[nl-1] - xa[0]) )* xa[i] - ((width *xa[0]) / (xa[nl-1] - xa[0]));
  }
  
  create_svg_line_chart(nl,xa,ssh);
  #endif 
  #if 0
  for (int i=0; i < nl; ++i){
    printf("ssh[%d] %f\n",i,ssh[i]);
  }
  
  float PI  = 4.0*atan(1.0);                                                                                                                                                            
  int   nx  = 1001;
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
  
  create_svg_line_chart(nx,xx,yy);

  free(ssh);
  free(xa);
  free(xx);
  free(yy);

  #endif
  

  return 0;

}

void create_svg_line_chart(int N, float *x, float *y){
  
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

void create_svg_contour_init(FILE **fin){

  *fin = fopen("circle.svg","w");
  fprintf(*fin,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n");
  fprintf(*fin,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  fprintf(*fin,"<svg height=\"600px\" version=\"1.1\" viewBox=\"0 0 900 600\" width=\"900px\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
  fprintf(*fin,"<defs><style type=\"text/css\">*{stroke-linecap:square;stroke-linejoin:round;}</style></defs>");
  fprintf(*fin,"<g id=\"line2d_1\"> <path d=\"\n");
}

void create_svg_contour_add(FILE *fin , float *x, float *y, int *beg){
 
  if (*beg == 0){
    fprintf(fin,"M%f %f\n",*x,*y);
  }
  else{
    fprintf(fin,"L%f %f\n",*x,*y);
  }
  *beg = 1;
}
 
void create_svg_contour_close(FILE *fin){
  
  fprintf(fin,"Z\" style=\"fill:none;stroke:#ff0000;\"");
  fprintf(fin,"/>\n");
  fprintf(fin,"</g>\n");
  fprintf(fin,"</svg>");
  fclose(fin);
}

