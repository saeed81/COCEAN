#ifndef SVG_H
#define SVG_H

void create_svg(float *y, int N){
  
  float xmin, xmax, ymin, ymax;

  //N = 50;

  xmin = 0; xmax = (float)N;
  ymin = -0.5; ymax = 0.5;

  char filename[] = "ssh.html";

  int w=640, h=400; // pixel size on screen
  
// Add a little empty space around the plot
   float left, right, top, bottom;
   left   = xmin - 0.1*(xmax-xmin);
   right  = xmax + 0.1*(xmax-xmin);
   bottom = ymin - 0.1*(ymax-ymin);
   top    = ymax + 0.1*(ymax-ymin);
  
// Begin SVG file
   FILE *f = fopen(filename, "w");
   fprintf(f, "<html><body><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"");
   fprintf(f, " width=\"%dpx\" height=\"%dpx\"", w, h);
   fprintf(f, " viewBox=\"%f %f %f %f\"", left, bottom, right-left, top-bottom);
   fprintf(f, " preserveAspectRatio=\"none\">\n");
   fprintf(f, "<g transform=\"scale(1.0 -1.0)\">\n\n");
  
// Create axes
   fprintf(f, "<polyline points=\"%f,%f %f,%f\"", left, 0.0, right, 0.0);
   fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:%f\" />\n", (top-bottom)/h);
   fprintf(f, "<polyline points=\"%f,%f %f,%f\"", 0.0, bottom, 0.0, top);
   fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:%f\" />\n\n", (right-left)/w);
  
// Plot data using a polyline element
   fprintf(f, "<polyline points=\"");
   for (int n=0; n<N ; ++n) fprintf(f, "%d,%f ", n, y[n]);
   fprintf(f, "\" style=\"fill:none;stroke:black;stroke-width:%f\" />", (right-left)/w);
   // Complete the HTML file
   fprintf(f, "\n\n</g></svg></body></html>\n");
   fclose(f);

   return;
}
#endif /* end of SVG_H */
