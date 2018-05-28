#include<stdio.h>
#include<stdlib.h>
#include<X11/Xlib.h>
#include<unistd.h>
#include<math.h>
#include"sflib.h"

long int numberoflines(FILE *file){

  long int nl = 0L;

  char c = ' ';
  while ((c = fgetc(file)) != EOF){
    if (c == '\n') nl++;
  }

  rewind(file);

  return nl;

}


float minval(float *a, int n){
  float mv = a[0];
  for (int i=1; i < n;++i){
    if (a[i] <= mv) mv = a[i];
  }
  return mv;
}

float maxval(float *a, int n){
  float mv = a[0];
  for (int i=1; i < n;++i){
    if (a[i] >= mv) mv = a[i];
  }
  return mv;
}


float minvall(float *a, long int n){
  float mv = a[0];
  for (long int i=1; i < n;++i){
    if (a[i] <= mv) mv = a[i];
  }
  return mv;
}

float maxvall(float *a, long int n){
  float mv = a[0];
  for (long int i=1; i < n;++i){
    if (a[i] >= mv) mv = a[i];
  }
  return mv;
}

#define infiniteloop for(;;)
#define LEN(a) (sizeof(a) / sizeof(a[0]))

int main(void){

  float a[] = {-1.0,1.0,2.20,3.0,4.1,5.0,5.1,5.0,6.8};
  printf("%f\n",minval(a,LEN(a)));
  printf("%f\n",maxval(a,LEN(a)));
  Display *dsp = NULL;
  int screen_num;
  Window win;
  unsigned int display_width;
  unsigned int display_height;
  unsigned int width;
  unsigned int height;
  GC gc;  // we need this one for drawing
  dsp = XOpenDisplay((char *)0);
  if (dsp == NULL){
    fprintf(stderr,"Cannot open the display\n");
  }
  screen_num     = DefaultScreen(dsp);
  display_width  = DisplayWidth(dsp,screen_num);
  display_height = DisplayWidth(dsp,screen_num);
  width  = (display_width / 3);
  height = (display_height / 3);
  printf("Window width %u; height %u\n",width, height);

  XColor xcolour;
  Colormap cmap;
  XColor black_col,white_col,red_col,green_col,blue_col,yellow_col,magenta_col,cyan_col;
  Colormap colormap;
  char black_bits[]   = "#000000";
  char white_bits[]   = "#FFFFFF";    // Mix red, green and blue to get white
  char red_bits[]     = "#FF0000";
  char green_bits[]   = "#00FF00";
  char blue_bits[]    = "#0000FF";
  char yellow_bits[]  = "#FFFF00";   // Mix red and green to get yellow 
  char magenta_bits[] = "#FF00FF";  // A sort of purple color                                                                                                                 
  char cyan_bits[]    = "#00FFFF";     // A blue-green color                                                                                                                  
  // Define the colors we want to use                                                                                                                                         
   
  
  colormap = DefaultColormap(dsp, 0);
  XParseColor(dsp, colormap, black_bits, &black_col); XAllocColor(dsp, colormap, &black_col);
  XParseColor(dsp, colormap, white_bits, &white_col); XAllocColor(dsp, colormap, &white_col);
  XParseColor(dsp, colormap, red_bits, &red_col); XAllocColor(dsp, colormap, &red_col);
  XParseColor(dsp, colormap, green_bits, &green_col);XAllocColor(dsp, colormap, &green_col);
  XParseColor(dsp, colormap, blue_bits, &blue_col);XAllocColor(dsp, colormap, &blue_col);
  XParseColor(dsp, colormap, yellow_bits, &yellow_col);XAllocColor(dsp, colormap, &yellow_col);
  XParseColor(dsp, colormap, magenta_bits, &magenta_col);XAllocColor(dsp, colormap, &magenta_col);
  XParseColor(dsp, colormap, cyan_bits, &cyan_col);XAllocColor(dsp, colormap, &cyan_col);

  width = 500;
  height= 500;
  win = XCreateSimpleWindow(dsp,RootWindow(dsp,screen_num),0,0,width,height,2,BlackPixel(dsp,screen_num),WhitePixel(dsp,screen_num));
  XSelectInput(dsp, win, StructureNotifyMask);        // We want to get MapNotify events
  XMapWindow(dsp, win);         // "Map" the window (that is, make it appear on the screen) 
                                                                                                                    
  for(;;){XEvent e; XNextEvent(dsp,&e); if(e.type == MapNotify) break;} //Wait for the MapNotify event                                                                                 
                          
  
  // which means that the window has appeared on the screen.                       

  //XMapWindow(dsp,win);

  XFlush(dsp);
  
  unsigned long valuemask = 0;          /* which values in 'values' to  */
                                        /* check when creating the GC.  */
  XGCValues values;                     /* initial values for the GC.   */
  
  //gc = XCreateGC(dsp, win, valuemask, &values);
  gc = XCreateGC(dsp, win, 0, NULL);

  //XSetForeground(dsp, gc, WhitePixel(dsp, screen_num));
  //XSetBackground(dsp, gc, BlackPixel(dsp, screen_num));
  
  //XSync(dsp, False);

  unsigned int line_width = 2;          /* line width for the GC.       */
  int line_style = LineSolid;           /* style for lines drawing and  */
  int cap_style = CapButt;              /* style of the line's edje and */
  int join_style = JoinBevel;           /*  joined lines.               */
  XSetForeground(dsp, gc, green_col.pixel);


  //XDrawLine(dsp, win, gc, 10, 70, 180, 30);      // Draw theline                                                                                                                      
                         
  //XSetForeground(dsp, gc, green_col.pixel);  // Tell the GC we draw using the GREEN color                                                                                         
                             
  //XDrawLine(dsp, win, gc, 10, 80, 180, 40);      // Draw the line                                                                                                                    
                          
  //XSetForeground(dsp, gc, blue_col.pixel);  // Tell the GC we draw using the BLUE color                                                                                        
                                
  //XDrawLine(dsp, win, gc, 10, 90, 180, 50);      // Draw the line 
  //XSetForeground(dpy, gc, whiteColor);  // Tell the GC we draw using the WHITE color (Method 1)                                                                                   
                              
  //XSetForeground(dsp, gc, white_col.pixel);  // Tell the GC we draw using the WHITE color (Method 2) 
  //XDrawLine(dsp, win, gc, 10, 60, 180, 20);      // Draw the line 
  /* define the style of lines that will be drawn using this GC. */
  XSetLineAttributes(dsp, gc,line_width, line_style, cap_style, join_style);
  /* define the fill style for the GC. to be 'solid filling'. */
  XSetFillStyle(dsp, gc, FillSolid);
  //xcolour.red = 32000; xcolour.green = 65000; xcolour.blue = 32000;
  //xcolour.flags = DoRed | DoGreen | DoBlue;
  //XAllocColor(dsp, cmap, &xcolour);
  //XSetForeground(dsp, gc, green_col.pixel);
  //XFillRectangle(d, w, gc, 0, 0, winatt.width, 30);
  //XFlush(d);
  /* draw one pixel near each corner of the window */
  //XDrawPoint(dsp, win, gc, 50, 50);
  //XFlush(dsp);
  //XDrawPoint(dsp, win, gc, 50, height-50);
  //XFlush(dsp);
  //XDrawPoint(dsp, win, gc, width-50, 50);
  //XFlush(dsp);
  //XDrawPoint(dsp, win, gc, width-50, height-50);
  //XFlush(dsp);
  /* draw two intersecting lines, one horizontal and one vertical, */
  /* which intersect at point "50,100".                            */
  //XDrawLine(dsp, win, gc, 50, 0, 50, 200);
  //XFlush(dsp);
  //XDrawLine(dsp, win, gc, 0, 100, 200, 100);

  int pi = 4.0 * atan(1.0);
  int nx = width;
  float dx = (2.0 * pi) / (nx -1);
  float *ap = (float *)malloc(nx *sizeof(float));
  for (int i=0; i < nx; i++){
    ap[i] = 10.0*sin(2.0*pi *i *dx);
  }
  float vmin = minval(ap,nx);
  float vmax = maxval(ap,nx);
  int yb, yn, xb, xn;

  FILE *file = NULL;
  file = fopen("viken.dat","r");
  if (file == NULL){
    printf("problem in reading the file \n");
    return 1;
  }
  long int nl = numberoflines(file);
  printf("the number of lines is %ld\n",nl);
  long int nii = 0L;
  float *vecval         = (float *)malloc(nl *sizeof(float));
  float value = 0.0f;
  long long int itime = 0LL;
  while (fscanf(file,"%lld%f",&itime,&value) && !feof(file)){
    vecval[nii] = value;
    nii++;
  }
  fclose(file);

  vmin = minvall(vecval,nl);
  vmax = maxvall(vecval,nl);

  printf("%f\t%f\n",vmin,vmax);
  


  #if 1
  for (long int i=0; i < nl;i++){
    int y = (int)(height - height*(vecval[i] -vmin) / (vmax -vmin));  
    int x = (int)(width*((float)i) / (nl));
    if (i == 0){
      xb = x;
      xn = x;
      yb = y;
      yn = y;
    }
    else {
      xb = xn;
      yb = yn;
      xn = x;
      yn = y;
    }
    //printf("%d\t%d\n",x,y);
    //XDrawPoint(dsp, win, gc, x, y);
    XDrawLine(dsp, win, gc, xb, yb, xn, yn);
  }
  #endif
 
  while (1) {
    XEvent e;
    XNextEvent (dsp, & e);
    
    /* Respond to ConfigureNotify, the type of event sent by the                                                                                                                         
       server if the window is resized. */

    if (e.type == ConfigureNotify) {
      XConfigureEvent xce = e.xconfigure;
      
      width  = xce.width;
      height = xce.height;
      for (long int i=0; i < nl;i++){
	int y = (int)(height- height*(vecval[i] -vmin) / (vmax -vmin));  
	int x = (int)(width*((float)i) / (nl));
	//printf("%d\t%d\n",x,y);
	//XDrawPoint(dsp, win, gc, x, y);
	if (i == 0){
	  xb = x;
	  xn = x;
	  yb = y;
	  yn = y;
	}
	else {
	  xb = xn;
	  yb = yn;
	  xn = x;
	  yn = y;
	}
    //printf("%d\t%d\n",x,y);
    //XDrawPoint(dsp, win, gc, x, y);
    XDrawLine(dsp, win, gc, xb, yb, xn, yn);
    
      }
    }
  }
  
  XFlush(dsp);

  infiniteloop;

  XCloseDisplay(dsp);

  free(ap);
  free(vecval);

  return 0;
}
