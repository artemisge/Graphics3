#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <cmath>
#include <list>
#define M_PI 3.14159265358979323846
using namespace std;

// constantly changing angle
GLfloat angle = 0;

GLfloat sunPos[] = {-50, 0, 0};
GLfloat sunInten[] = {0.3, 0.3, 0.3};
GLfloat cam[] = {0, 40, 70};
GLfloat camAngle = M_PI / 180 * 270;
bool s_enabled = true; // spotlight enabled
int grass = 2;         // default (2-3)

typedef GLfloat point3[3];

// Συνάρτηση για υπολογισμό εξωτερικού γινομένου
// δύο διανυσμάτων
float *cross_product(float *a, float *b)
{
  float *result = new float[3]{a[1] * b[2] - a[2] * b[1],
                               a[0] * b[2] - a[2] * b[0],
                               a[0] * b[1] - a[1] * b[0]};
  return result;
}

// Συνάρτηση για κανονικοποίηση
void normalize(float *v)
{
  // std::cout << v[0];
  // std::cout << v[1];
  // std::cout << v[2];
  float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  //std::cout << length;
  for (int i = 0; i < 3; i++)
  {
    v[i] = v[i] / length;
  }
}

// Συνάρτηση για τον υπολογισμό νόρμας 3ων κορυφών
float *calculate_normal(float *a, float *b, float *c)
{
  // x = b - a, y = c - a
  float x[] = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
  float y[] = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};

  float *result = cross_product(x, y);
  normalize(result);
  return result;
}

void myinit()
{
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  GLfloat light_position[] = {-50, 0, 0, 1};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_SPECULAR, sunInten);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sunInten);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_NORMALIZE);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  glMatrixMode(GL_PROJECTION);
  glEnable(GL_DEPTH_TEST); // to see back of the cube, has to do with 3Dness
  glClearColor(0, 0, 0, 0);
  glOrtho(-50.0, 50.0, -50.0, 50.0, 0, 100); // αν ειναι αρνητικά, πάει πίσω από τον παρατηρητή

  glMatrixMode(GL_MODELVIEW); // peirazoume ton xoro kai ta simeia tora, oxi tin kamera
  gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);

  // glulookat explanation: https://stackoverflow.com/questions/5717654/glulookat-explanation/5721110
}

float step = 0.0035; // total steps: 90 / 0.45 = 200 | 0.7 / 200 = 0.0035
void idleFunc()
{
  angle += 0.45;
  if (angle > 180)
  {
    angle = 0;
  }
  if (angle <= 90)
  {
    sunInten[0] += step;
    sunInten[1] += step;
    sunInten[2] += step;
    //printf("%f", sunInten[0]);
  }
  else
  {
    sunInten[0] -= step;
    sunInten[1] -= step;
    sunInten[2] -= step;
    //printf("%f", sunInten[0]);
  }
  glutPostRedisplay();
}

// sxhmatizei thn mia pleura tou kubou apo ta 4 vertices pou dinoume os orisma
void cubeFace(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[])
{
  glBegin(GL_POLYGON);
  // Vertex: 3D, float, array
  glNormal3fv(calculate_normal(V0, V1, V2));
  glVertex3fv(V0);
  glVertex3fv(V1);
  glVertex3fv(V2);
  glVertex3fv(V3);
  glEnd();
}

// recursive function. DEPTH == 0 return
void subDivision(GLfloat v0[], GLfloat v1[], GLfloat v2[], int depth)
{
  if (depth == 0)
  {
    return;
  }
  else
  {
    --depth;
    // now plot the points
    glBegin(GL_TRIANGLES);
    glVertex3fv(v0);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glEnd();

    // now make new triangles
    point3 v[] = {{(v0[0] + v1[0]) / 2, (v0[1] + v1[1]) / 2, (v0[2] + v1[2]) / 2},
                  {(v0[0] + v2[0]) / 2, (v0[1] + v2[1]) / 2, (v0[2] + v2[2]) / 2},
                  {(v2[0] + v1[0]) / 2, (v2[1] + v1[1]) / 2, (v2[2] + v1[2]) / 2}};

    // normalize points
    for (int i = 0; i < 3; i++)
    {
      float length = sqrt(pow(v[i][0], 2) + pow(v[i][1], 2) + pow(v[i][2], 2));
      v[i][0] /= length;
      v[i][1] /= length;
      v[i][2] /= length;
    }

    subDivision(v0, v[0], v[1], depth);
    subDivision(v[0], v1, v[2], depth);
    subDivision(v[1], v2, v[2], depth);
    subDivision(v[0], v[1], v[2], depth);
  }
}

GLfloat initialSquare[4][3] = {{-1.0, 1.0, 0},
                               {1.0, 1.0, 0},
                               {1.0, -1.0, 0},
                               {-1.0, -1.0, 0}};

int shading = 0; // default
void display()
{
  // Δημιουργία τετραγώνου πλευράς 2 πάνω στο επίπεδο Z=0
  glNewList(1, GL_COMPILE);
  cubeFace(initialSquare[0], initialSquare[1], initialSquare[2], initialSquare[3]);
  glEndList();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);
  printf(" %f|%f|%f\n", cam[0], cam[1], cam[2]);

  // SUN/sphere movement
  glPushMatrix();
  glRotatef(angle, 0, 0, -1);
  GLfloat light_position[] = {-50, 0, 0, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glPopMatrix();

  // SPOTLIGHT
  glPushMatrix();
  //-------spotlight-------

  // για να βλέπουμε που είναι και που φωτίζει το spotlight
  glPushMatrix();
  glTranslatef(5.1, 10, 7);
  glutSolidSphere(0.5, 100, 100);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(6, 0, 11);
  glutSolidSphere(0.5, 100, 100);
  glPopMatrix();

  GLfloat spot_position[] = {5.1, 10, 7, 1};
  GLfloat spot_diffuse[] = {1, 1, 1, 1};
  GLfloat spot_specular[] = {1, 1, 1, 1};
  GLfloat spot_direction[] = {6 - 5.1, 0 - 10, 11 - 7};
  //GLfloat spot_ambient[] = {1, 1, 1, 1};

  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
  glLightfv(GL_LIGHT1, GL_POSITION, spot_position);
  //glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, spot_specular);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
  //glLightfv(GL_LIGHT1, GL_AMBIENT, spot_ambient);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15);
  glEnable(GL_LIGHT1);
  //-------end spotlight--------
  glPopMatrix();

  if (s_enabled)
    glEnable(GL_LIGHT1);
  else
    glDisable(GL_LIGHT1);

  point3 v[] = {{0.0, 0.0, 1.0},
                {0.0, 0.942809, -0.33333},
                {-0.816497, -0.471405, -0.333333},
                {0.816497, -0.471405, -0.333333}};


  // σφαίρα ήλιου
  glPushMatrix();
  GLfloat mat_emission5[] = {1, 1.0, 0, 1.0};
  GLfloat mat_diffuse5[] = {0, 0, 0, 1.0};
  GLfloat mat_specular5[] = {0, 0, 0, 1.0};
  GLfloat mat_shininess5[] = {0};

  glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission5);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse5);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular5);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess5);

  glRotatef(angle, 0, 0, -1);
  glTranslatef(-50, 0, 0);

  subDivision(v[0], v[1], v[2], 4);
  subDivision(v[0], v[1], v[3], 4);
  subDivision(v[0], v[2], v[3], 4);
  subDivision(v[1], v[2], v[3], 4);
  glPopMatrix();

  switch (shading)
  {
  case 0:
    // SHADING: SMOOTH
    glShadeModel(GL_SMOOTH);
    break;
  case 1:
    // SHADING: FLAT
    glShadeModel(GL_FLAT);
    break;
  }

  // ----ΓΡΑΣΙΔΙ----
  glPushMatrix();
  glColor3f(0, 1, 0); // GREEN
  GLfloat mat_emission[] = {0, 0, 0, 1.0};
  GLfloat mat_ambient[] = {0, 1.0, 0, 1.0};
  GLfloat mat_diffuse[] = {0, 1.0, 0, 1.0};
  GLfloat mat_specular[] = {0, 0, 0, 1.0};
  GLfloat mat_shininess[] = {0};

  //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  switch (grass)
  {
  case 2:
    glScalef(20, 1, 20);
    glRotatef(90, 1, 0, 0);
    glCallList(1);
    break;
  case 3:
    // Αν η προηγούμενη επιφάνεια είναι 40x40
    // τότε, αν το χωρίσουμε σε 100 τετράγωνα (10x10)
    // το κάθε τετράγωνο θα είναι 4x4.
    int size = 4;
    // Επίσης, το πέρα αριστερά vertex είναι: (-20, 0, -20)
    int x = -20, y = 0, z = -20;
    for (int i = 0; i < 10; i++)
    {
      for (int j = 0; j < 10; j++)
      {
        // Για κάθε τετράγωνο θα βρούμε τις 4 κορυφές του
        // Τα μετράμε ως εξής:
        // v0---v1
        // Ι	Ι
        // v2---v3
        GLfloat v0[] = {x + (size * i), y, z + (size * j)};
        GLfloat v1[] = {x + size + (size * i), y, z + (size * j)};
        GLfloat v2[] = {x + (size * i), y, z + size + (size * j)};
        GLfloat v3[] = {x + size + (size * i), y, z + size + (size * j)};
        glBegin(GL_POLYGON);
        glNormal3fv(calculate_normal(v0, v1, v2));
        glVertex3fv(v0);
        glVertex3fv(v2);
        glVertex3fv(v3);
        glVertex3fv(v1);
        glEnd();
      }
    }
    break;
  }
  glPopMatrix(); // Τέλος το γρασίδι

  // "δέντρο"
  glPushMatrix();
  glTranslatef(10, 15 - 5, 4);
  glutSolidSphere(2, 100, 100);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(10, 11 - 5, 4);
  glutSolidSphere(3, 100, 100);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(10, 7 - 5, 4);
  glutSolidSphere(3.5, 100, 100);
  glPopMatrix();

  // ----ΤΟΙΧΟΙ----
  // πόρτα
  glPushMatrix();
  glColor3f(0, 0, 0); // black
  GLfloat mat_difambp[] = {0, 0, 0, 1.0};
  GLfloat mat_specularp[] = {0, 0, 0, 1.0};
  GLfloat mat_shininessp[] = {0};

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_difambp);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularp);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessp);
  glTranslatef(0, 2.5, 10.1);
  glScalef(1.5, 2.5, 1);
  glRotatef(180, 1, 0, 0);
  glCallList(1);
  glPopMatrix();

  // πλευρά μπροστινή
  glPushMatrix();
  glColor3f(1, 0, 0); // red
  GLfloat mat_dif[] = {1, 0, 0, 1.0};
  GLfloat mat_specular2[] = {0, 0, 0, 1.0};
  GLfloat mat_shininess2[] = {0};
  GLfloat mat_diffuse2[] = {1, 0, 0, 1.0};

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
  glTranslatef(0, 5, 10);
  glScalef(5, 5, 1);
  glRotatef(180, 1, 0, 0);
  glCallList(1);
  glPopMatrix();

  // πλευρά δεξιά
  glPushMatrix();
  glTranslatef(5, 5, 0);
  glScalef(1, 5, 10);
  glRotatef(90, 0, -1, 0);
  glCallList(1);
  glPopMatrix();

  // πλευρά αριστερά
  glPushMatrix();
  glTranslatef(-5, 5, 0);
  glScalef(1, 5, 10);
  glRotatef(90, 0, 1, 0);
  glCallList(1);
  glPopMatrix();

  // πλευρά πάνω
  glPushMatrix();
  glTranslatef(0, 10, 0);
  glScalef(5, 1, 10);
  glRotatef(90, 1, 0, 0);
  glCallList(1);
  glPopMatrix();

  // πλευρά κάτω
  glPushMatrix();
  glScalef(5, 1, 10);
  glRotatef(90, 1, 0, 0);
  glCallList(1);
  glPopMatrix();

  // πλευρά πίσω
  glPushMatrix();
  glTranslatef(0, 5, -10);
  glScalef(5, 5, 1);
  glCallList(1);
  glPopMatrix();

  // ----ΣΤΕΓΗ----
  // στέγη μπροστά
  glPushMatrix();
  glTranslatef(0, 10, 10);
  //
  glBegin(GL_TRIANGLES);
  // ισόπλευρο τρίγωνο
  float a[] = {-5, 0, 0};
  float b[] = {5, 0, 0};
  float c[] = {0, sqrt(75), 0};
  glNormal3fv(calculate_normal(a, b, c));
  glVertex3f(-5, 0, 0);
  glVertex3f(5, 0, 0);
  glVertex3f(0, sqrt(75), 0);
  glEnd();
  glPopMatrix();

  // στέγη πίσω
  glPushMatrix();
  glTranslatef(0, 10, -10);
  glRotatef(180, 0, 1, 0);
  glBegin(GL_TRIANGLES);
  // ισόπλευρο τρίγωνο
  glNormal3fv(calculate_normal(a, b, c));
  glVertex3f(-5, 0, 0);
  glVertex3f(5, 0, 0);
  glVertex3f(0, sqrt(75), 0);
  glEnd();
  glPopMatrix();

  // στέγη δεξιά
  glPushMatrix();
  glColor3f(0.6, 0.6, 0.6);
  //GLfloat mat_ambient3[] = {0.6, 0.6, 0.6, 1.0};
  GLfloat mat_diffuse3[] = {0.75, 0.75, 0.75, 1.0};
  GLfloat mat_specular3[] = {1, 1, 1, 1.0};
  GLfloat mat_shininess3[] = {100};

  //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient3);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular3);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess3);
  glTranslatef(5, 10, 0);
  glRotatef(60, 0, 0, -1);
  glTranslatef(-5, 0, 0);
  glScalef(5, 1, 10);
  glRotatef(90, 1, 0, 0);
  glCallList(1);
  glPopMatrix();

  // στέγη αριστερά
  glPushMatrix();
  //glColor3f(0,0,0); // black
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular3);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess3);
  glTranslatef(-5, 10, 0);
  glRotatef(60, 0, 0, 1);
  glTranslatef(5, 0, 0);
  glScalef(5, 1, 10);
  glRotatef(90, 1, 0, 0);
  glCallList(1);
  glPopMatrix();

  glutSwapBuffers();
  glFlush(); /* clear buffers */
}

void menu(int option)
{
  switch (option)
  {
  case 0:
    shading = 0;
    break;
  case 1:
    shading = 1;
    break;
  case 2:
  case 3:
    grass = option;
    break;
  case 4:
    s_enabled = true;
    break;
  case 5:
    s_enabled = false;
    break;
  }
}

void SpecialKeyHandler(int key, int x, int y)
{
  if (key == GLUT_KEY_RIGHT)
    camAngle += M_PI / 180 * 0.45;
  if (key == GLUT_KEY_LEFT)
    camAngle -= M_PI / 180 * 0.45;

  if (camAngle > M_PI / 180 * 360)
    camAngle -= M_PI / 180 * 360;
  if (camAngle < 0)
    camAngle += M_PI / 180 * 360;

  // now calculate new (x,z) for camera
  cam[0] = cos(camAngle) * 70; // 70 = radius
  cam[2] = -sin(camAngle) * 70;

  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(50, 150);
  glutCreateWindow("Little House");
  myinit();
  glutDisplayFunc(display);
  glutIdleFunc(idleFunc);
  glutSpecialFunc(SpecialKeyHandler);
  glutCreateMenu(menu);
  glutAddMenuEntry("Smooth Shading", 0);
  glutAddMenuEntry("Flat Shading", 1);

  glutAddMenuEntry("One tile floor", 2);
  glutAddMenuEntry("100 tiles floor", 3);

  glutAddMenuEntry("Spotlight On", 4);
  glutAddMenuEntry("Spotlight Off", 5);

  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 0;
}

//ModelView matrix is the concatenation of Model matrix and
//View Matrix. View Matrix defines the position(location and orientation)
//of the camera, while model matrix defines the frame’s position
//of the primitives you are going to draw.
//Projection matrix defines the characteristics of your
//camera, such as clip planes, field of view, projection method etc.

// The "normal" of a vertex is the vector which is "perpendicular" to
// the vertex. In mathematics "normal" is a generalization of "perpendicular".
// For a polygon, this "normal vector" is perpendicular to the polygon and
// is the same for all of its vertices. One reason you might assign
// different normal vectors to each vertex of a polygon is if you are
// covering a curved surface with very small triangles. In this case,
// you don't want the normal vectors of the three vertices of the
// triangle to all be the same.

// Now what is this normal vector used for? The typical application
// is used for coloring calculations when lighting is enabled in OpenGL.
// The normal vector can determine whether the light from a light source
// hits a surface and what angle a light ray makes with the surface.
// This can then be used to determine whether the surface is shadowed
// or contains a specular highlight, for instance.
