#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cmath>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "gl_util.h"

const size_t WindowsWidth = 750;
const size_t WindowsHeight = 750;
const size_t mandelWidth = 1024;
const size_t mandelHeight = 1024;

unsigned int textureID;
uint8_t *mandel;

float colorGraph( float n , int c ) {
  int color = 0;
  // if (c == 0) color = (sin(self.freqRed * unit * mandelColor + self.phaseRed) + 1) / 2 * 255.0;
  // if (c == 1) color = (sin(self.freqGreen * unit * mandelColor + self.phaseGreen) + 1) / 2 * 255.0;
  // if (c == 2) color = (sin(self.freqBlue * unit * mandelColor + self.phaseBlue) + 1) / 2 * 255.0;
  if ( c == 0 ) color = n / 10 * 256;
  return color;
}


// x0 left, x1 right, y0 bottom, y1 top

void mandelGraph( uint8_t* I, int cols, int rows, int channels, float x0, float x1, float y0, float y1 ) {
  for ( int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      // (x/cols) -> 0...1       *(x1-x0)  -> 0..(x1-x0)       (+x0)  -> -x0..x1
      float Cx = ( ( x / (float)(cols-1) ) * ( x1 - x0 ) ) + x0;
      float Cy = ( ( y / (float)(rows-1) ) * ( y1 - y0 ) ) + y0;
      float Zx = 0;
      float Zy = 0;
      float xtemp = 0;
      bool conv = true;
      float nMax = 0;

      // Z = Z^2 + C
      // Z^2 = (a+bi)^2 = a^2 - b^2 + (2ab)i
      for( int n = 0; n < 1000; ++n) {
        xtemp = ( Zx * Zx ) - ( Zy * Zy ) + Cx;
        Zy = ( 2 * Zx * Zy ) + Cy;
        Zx = xtemp;
        if ( ( Zx * Zx + Zy * Zy ) >= 4 ){
            conv = false;
            nMax = n;
            break;
        }
      }

      // printf(" %d ---- cy = %f   cx = %f \n", conv, Cx, Cy);
      for( int c = 0; c < channels; ++c) {
        int idx = (y * cols + x) * channels + c;
        if ( conv == false ) {
          I[idx] = colorGraph( nMax, c );
        }
      }
    }
  }
}

// repeats 30x per second
void display() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mandelWidth, mandelHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mandel);


  // glTranslatef(50, 50, 0);

  glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
      glTexCoord2d(1, 1); glVertex2f(0.0,   0.0);
      glTexCoord2d(1, 0); glVertex2f(0.0,   WindowsHeight);
      glTexCoord2d(0, 0); glVertex2f(WindowsWidth, WindowsHeight);
      glTexCoord2d(0, 1); glVertex2f(WindowsWidth, 0.0);
    glEnd();
  glDisable(GL_TEXTURE_2D);




  glutSwapBuffers();
  glPopMatrix();
}

void init_display() {
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glLineWidth(0);
  glViewport(0, 0, 2 * WindowsWidth, 2 * WindowsHeight);
  set2DMode( WindowsWidth, WindowsHeight);

  glGenTextures(1, &textureID);
}

void reshape(int w, int h) {
  init_display();
}

void normal_keys(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    printf("exiting program---------\n");
    exit(0);
    break;
  case 32:
    printf("finding interesting section---------\n");
    mandelGraph(mandel, mandelWidth, mandelHeight, 3, -1, 0, -0.5, 0.5);
    display();
    break;
  default:
    break;
  }
}

void init_glut_window(int argc, char *argv[]) {
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( WindowsWidth, WindowsHeight );
  glutCreateWindow( "Guadas Window" );
  glutDisplayFunc( display );
  glutIdleFunc( display );
  glutKeyboardFunc( normal_keys );
  // set mouse func here
  glutReshapeFunc( reshape );
  glutMainLoop();
}

int main(int argc, char *argv[])
{
  mandel = new uint8_t[ mandelWidth * mandelHeight * 3 ];
  mandelGraph(mandel, mandelWidth, mandelHeight, 3, -2, 1, -1.5, 1.5);
  init_glut_window(argc, argv);
}


// void Mod(uint8_t* I, int cols,  int rows, int channels) {
//   for ( int y = 0; y < rows; ++y) {
//     for (int x = 0; x < cols; ++x) {
//        for( int c = 0; c < channels; ++c) {
//          int idx = (y * cols + x) * channels + c;
//          if ( x == y && c==0)
//            I[idx] = 255;
//        }
//     }
//   }
// }
