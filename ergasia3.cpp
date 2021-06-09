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

GLfloat sunPos[] = {-50,0,0};
GLfloat sunInten[] = {0.3, 0.3, 0.3};
GLfloat cam[] = {0, 40, 70};
GLfloat camAngle = M_PI/180 * 270;
bool s_enabled = true; // spotlight enabled

typedef GLfloat point3[3];

void myinit()
{
	//glLoadIdentity();
	glShadeModel (GL_SMOOTH);
	
   	GLfloat light_position[] = {-50, 0, 0, 1};

  	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunInten);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunInten);

   	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0);

	// spotlight
	GLfloat spot_position[] = {0, 5, 15, 1};
	GLfloat spot_diffuse[] = {0.5,0.5,0.5,1};	
	GLfloat spot_specular[] = {1,1,1,1};
	GLfloat spot_direction[] = {0, 0, 10};
	GLfloat spot_ambient[] = {1,0,1,1};


	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF, 7.0);
	glLightfv(GL_LIGHT1, GL_POSITION, spot_position);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spot_specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT1, GL_AMBIENT, spot_ambient);

	//glLightfv(GL_LIGHT1,GL_SPOT_EXPONENT, 0);
	glEnable(GL_LIGHT1);
	//end spotlight
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST); // to see back of the cube, has to do with 3Dness
	glClearColor(0, 0, 0, 0); 	
	glOrtho(-50.0, 50.0, -50.0, 50.0, 0, 100); // αν ειναι αρνητικά, πάει πίσω από τον παρατηρητή
	
	glMatrixMode(GL_MODELVIEW); // peirazoume ton xoro kai ta simeia tora, oxi tin kamera
	gluLookAt(cam[0],cam[1],cam[2],0,0,0,0,1,0);
	// lookat explanation: https://stackoverflow.com/questions/5717654/glulookat-explanation/5721110
}

float step = 0.0035; // total steps: 90 / 0.45 = 200 | 0.7 / 200 = 0.0035
void idleFunc() {
	angle += 0.45;
	if (angle > 180) {
		angle = 0;
	}
	if (angle <= 90) {
		sunInten[0] += step;
		sunInten[1] += step;
		sunInten[2] += step;
		//printf("%f", sunInten[0]);
	} else {
		sunInten[0] -= step;
		sunInten[1] -= step;
		sunInten[2] -= step;
		//printf("%f", sunInten[0]);
	}
	
	glutPostRedisplay();
}

// sxhmatizei thn mia pleura tou kubou apo ta 4 vertices pou dinoume os orisma
void cubeFace(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[]){
	glBegin(GL_POLYGON);
		// Vertex: 3D, float, array
		//float length = sqrt(0^2 + 0^2 + (-1)^2);
		glNormal3f(0, 0, -1);
		glVertex3fv(V0); 
		glNormal3f(0, 0, -1);
		glVertex3fv(V1);
		glNormal3f(0, 0, -1);
		glVertex3fv(V2);
		glNormal3f(0, 0, -1);
		glVertex3fv(V3);
	glEnd(); 
}

// recursive function. DEPTH == 0 return
void subDivision(GLfloat v0[], GLfloat v1[], GLfloat v2[], int depth) {
	if (depth == 0) {
		return;
	} else {
		--depth;
		// now plot the points
		glBegin(GL_TRIANGLES);
			glVertex3fv(v0); 
			glVertex3fv(v1);
			glVertex3fv(v2);
		glEnd(); 
		
		// now make new triangles
		point3 v[] = {	{(v0[0]+v1[0])/2, (v0[1]+v1[1])/2, (v0[2]+v1[2])/2},
						{(v0[0]+v2[0])/2, (v0[1]+v2[1])/2, (v0[2]+v2[2])/2},
						{(v2[0]+v1[0])/2, (v2[1]+v1[1])/2, (v2[2]+v1[2])/2}};
		
		// normalize points
		for (int i = 0; i < 3; i++) {
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

GLfloat initialSquare[4][3] = {	{-1.0, 1.0, 0},
								{1.0, 1.0, 0},
								{1.0, -1.0, 0},
								{-1.0, -1.0, 0}};

int menuoption = 0; // default 
void display() 
{	
	// Δημιουργία τετραγώνου πλευράς 2 πάνω στο επίπεδο Z=0
	glNewList(1, GL_COMPILE);
		cubeFace(initialSquare[0], initialSquare[1], initialSquare[2], initialSquare[3]);
	glEndList();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam[0],cam[1],cam[2],0,0,0,0,1,0);
	printf(" %f|%f|%f\n", cam[0], cam[1], cam[2]);

	// sun/sphere movement
	glPushMatrix();
		glRotatef(angle, 0, 0, -1);
		GLfloat light_position[] = {-50, 0, 0, 1};
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	// spotlight
	// glPushMatrix();
	// 	glLoadIdentity();
		
	// 	GLfloat spot_position[] = {0, 10, 10.1, 1};
	// 	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	// 	GLfloat specularLight[] = {0.5, 0.5, 0.5};
	// 	GLfloat diffuseLight[] = {0.5, 0.5, 0.5}; 

	// 	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	// 	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);

	// 	if (s_enabled) 
	// 		glEnable(GL_LIGHT1);
	// 	else
	// 		glDisable(GL_LIGHT1);
		
	// glPopMatrix();

	point3 v[] = {	{0.0, 0.0, 1.0}, 
					{0.0, 0.942809, -0.33333},
					{-0.816497, -0.471405, -0.333333}, 
					{0.816497, -0.471405, -0.333333}};

	glPushMatrix();
		GLfloat mat_emission5[] = { 1, 1.0, 0, 1.0 };
		GLfloat mat_diffuse5[] = { 0, 1.0, 1, 1.0 };
		GLfloat mat_specular5[] = { 0, 0, 0, 1.0 };
  		GLfloat mat_shininess5[] = { 0 };

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

	// switch(menuoption) {
	// 	case 0:
	// 		// cube's center is on (0, 0, b)
	// 		// rotate around b-center
	// 		glLoadIdentity();
	// 		//glTranslatef(0, 0, -b);
	// 		//glScalef(a/2, a/2, a/2);
	// 		//glRotatef(angle, xyz[0], xyz[1], xyz[2]);
	// 		break;
	// 	case 1:
	// 		// rotate around -8*b/10
	// 		glLoadIdentity();	
	// 		glTranslatef(0, 0, -8*b/10);
			
	// 		glRotatef(angle, xyz[0], xyz[1], xyz[2]);
	// 		glTranslatef(0, 0, -20); // aktina peristrofhs
	// 		glScalef(a/2*mult, a/2*mult, a/2*mult);

	// 		// -8*b/10 point, to see it visually
	// 		glPushMatrix();
	// 			glLoadIdentity();
	// 			glColor3f(1,1,1);
	// 			glPointSize(10);
	// 			glBegin(GL_POINTS);
	// 				// Vertex: 3D, float, array
	// 				glVertex3f(0, 0, -8*b/10); 
	// 			glEnd(); 
	// 		glPopMatrix();
	// 		break;
	// 	}
	

	// ----ΓΡΑΣΙΔΙ----
	glPushMatrix();
		//glColor3f(0,1,0);
		GLfloat mat_emission[] = { 0, 0, 0, 1.0 };
		GLfloat mat_ambient[] = { 0, 1.0, 0, 1.0 };
		GLfloat mat_diffuse[] = { 0, 1.0, 0, 1.0 };
		GLfloat mat_specular[] = { 0, 0, 0, 1.0 };
  		GLfloat mat_shininess[] = { 0 };

   		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
   		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		glScalef(15, 1, 20);
		glRotatef(90, 1, 0, 0);
		glCallList(1);
	glPopMatrix();

	// "δέντρο"
	glPushMatrix();
		glTranslatef(10, 15, 4);
		glutSolidSphere(2,100,100);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10, 11, 4);
		glutSolidSphere(3,100,100);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10, 7, 4);
		glutSolidSphere(3.5,100,100);
	glPopMatrix();

	
	// ----ΤΟΙΧΟΙ----
	// πόρτα
	glPushMatrix();
		//glColor3f(1,0,0); // red
		GLfloat mat_emissionp[] = { 0, 0, 0, 1.0 };
		GLfloat mat_specularp[] = { 0, 0, 0, 1.0 };
  		GLfloat mat_shininessp[] = { 0 };

   		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emissionp);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularp);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessp);
		glTranslatef(0, 2.5, 10.1);
		glScalef(1.5, 2.5, 1);
		glCallList(1);
	glPopMatrix();	

	// πλευρά μπροστινή
	glPushMatrix();
		//glColor3f(1,0,0); // red
		GLfloat mat_emission2[] = { 1, 0, 0, 1.0 };
		GLfloat mat_specular2[] = { 0, 0, 0, 1.0 };
  		GLfloat mat_shininess2[] = { 0 };
		GLfloat mat_diffuse2[] = { 1, 0, 0, 1.0 };

   		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(0, 5, 10);
		glScalef(5, 5, 1);
		glCallList(1);
	glPopMatrix();

	// πλευρά δεξιά
	glPushMatrix();
		//glColor3f(1,0,1); // mag
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_emission2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(5, 5, 0);
		glScalef(1, 5, 10);
		glRotatef(90, 0, -1, 0); 
		glCallList(1);
	glPopMatrix();

	// πλευρά αριστερά
	glPushMatrix();
		//glColor3f(1,0,1); // yellow
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(-5, 5, 0);
		glScalef(1, 5, 10);
		glRotatef(90, 0, 1, 0); 
		glCallList(1);
	glPopMatrix();

	// πλευρά πάνω
	glPushMatrix();
		//glColor3f(0,0,1); // blue
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(0, 10, 0);
		glScalef(5, 1, 10);
		glRotatef(90, 1, 0, 0);
		glCallList(1);
	glPopMatrix();

	// πλευρά κάτω
	glPushMatrix();
		//glColor3f(0,1,1); // cyan
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glScalef(5, 1, 10);
		glRotatef(90, 1, 0, 0);
		glCallList(1);
	glPopMatrix();

	// πλευρά πίσω
	glPushMatrix();
		//glColor3f(1,1,0); // magenda
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(0, 5, -10);
		glScalef(5, 5, 1);
		glCallList(1);
	glPopMatrix();

	// ----ΣΤΕΓΗ----
		// στέγη μπροστά
	glPushMatrix();
		//glColor3f(1,1,0); // magenda
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(0,10,10);
		glBegin(GL_TRIANGLES);
			// ισόπλευρο τρίγωνο
			glVertex3f(-5,0,0); 
			glVertex3f(5,0,0);
			glVertex3f(0,sqrt(75),0);
		glEnd(); 
	glPopMatrix();

	// στέγη πίσω
	glPushMatrix();
		//glColor3f(0,1,1); // magenda
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
		glTranslatef(0,10,-10);
		glBegin(GL_TRIANGLES);
			// ισόπλευρο τρίγωνο
			glVertex3f(-5,0,0); 
			glVertex3f(5,0,0);
			glVertex3f(0,sqrt(75),0);
		glEnd(); 
	glPopMatrix();

	// στέγη δεξιά
	glPushMatrix();
	
		//glColor3f(1,1,1); // white		
		GLfloat mat_ambient3[] = { 0.6, 0.6, 0.6, 1.0 };
		GLfloat mat_diffuse3[] = { 0.75,0.75,0.75, 1.0 };
		GLfloat mat_specular3[] = { 1, 1, 1, 1.0 };
  		GLfloat mat_shininess3[] = { 100 };

   		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient3);
   		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular3);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess3);
		glTranslatef(5,10,0);
		glRotatef(60, 0, 0, -1);
		glTranslatef(-5, 0, 0);
		glScalef(5, 1, 10);
		glRotatef(90, 1, 0, 0); 
		glCallList(1);
	glPopMatrix();

	// στέγη αριστερά
	glPushMatrix();
	
		//glColor3f(0,0,0); // black
   		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse3);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular3);
  		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess3);
		glTranslatef(-5,10,0);
		glRotatef(60, 0, 0, 1);
		glTranslatef(5, 0, 0);
		glScalef(5, 1, 10);
		glRotatef(90, 1, 0, 0); 
		glCallList(1);
	glPopMatrix();


	glutSwapBuffers();
	glFlush(); /* clear buffers */
}

void menu(int option) {
	switch (option) {
        case 0:
			menuoption = 0;
			break;
        case 1:
			menuoption = 1;
			break;
	}
}

void SpecialKeyHandler (int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT)
        camAngle += M_PI/180 * 0.45;
    if (key == GLUT_KEY_LEFT)
        camAngle -= M_PI/180 * 0.45;

	if (camAngle > M_PI/180 * 360) 
		camAngle -= M_PI/180 * 360;
	if (camAngle < 0)
		camAngle += M_PI/180 * 360;

	// now calculate new (x,z) for camera
	cam[0] =  cos(camAngle) * 70; // 70 = radius
	cam[2] =  -sin(camAngle) * 70;
	
    glutPostRedisplay();
}

// // επιστρέφει έναν πίνακα από κορυφές τριγώνων. array of array[3]
// void sphereVert(point3 initSphere) {
// 	list<point3[3]> listOfTriangles;

// 	// for (int i = 0; i < 10; i++) {
// 	// 	listOfTriangles.push_back({{(0,0,0)}, {(0,0,1)}, {(0,0,2)}});
// 	// }

//     // return listOfTriangles;
// }

// float radius = 1;
// void computeHalfVertex(const float v1[3], const float v2[3], float newV[3])
// {
//     newV[0] = v1[0] + v2[0];    // x
//     newV[1] = v1[1] + v2[1];    // y
//     newV[2] = v1[2] + v2[2];    // z
//     float scale = radius / sqrtf(newV[0]*newV[0] + newV[1]*newV[1] + newV[2]*newV[2]);
//     newV[0] *= scale;
//     newV[1] *= scale;
//     newV[2] *= scale;
// }


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
	glutSpecialFunc (SpecialKeyHandler);
	//glutCreateMenu(menu);
	//glutAddMenuEntry("Rotate center", 0);
    //glutAddMenuEntry("Rotate vector", 1);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

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
