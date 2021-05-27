#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#define M_PI 3.14159265358979323846
using namespace std;

// constantly changing angle
GLfloat angle = 0;

GLfloat sunPos[] = {-50,0,0};
GLfloat sunInten[] = {0.3, 0.3, 0.3};
GLfloat cam[] = {0, 40, 70};
GLfloat camAngle = M_PI/180 * 270;

void myinit()
{
	//glLoadIdentity();
	glShadeModel (GL_SMOOTH);
	
   	GLfloat light_position[] = {-50, 0, 0, 1};
   	GLfloat whiteSpecularLight[] = {0.5, 0.5, 0.5};
	//GLfloat blackAmbientLight[] = {0.5, 0.5, 0.5};
	GLfloat whiteDiffuseLight[] = {0.5, 0.5, 0.5}; 

  	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunInten);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunInten);

   	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_NORMALIZE);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST); // to see back of the cube, has to do with 3Dness
	glClearColor(0, 0, 0, 0); 	

	glOrtho(-50.0, 50.0, -50.0, 50.0, 0, 100); // αν ειναι αρνητικά, πάει πίσω από τον παρατηρητή
	// nearVal, farVal documentation
    //Specify the distances to the nearer and farther depth clipping planes. These values are negative if the plane is to be behind the viewer. 
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
		glVertex3fv(V0); 
		glVertex3fv(V1);
		glVertex3fv(V2);
		glVertex3fv(V3);
	glEnd(); 
}

GLfloat initialSquare[4][3] = {	{-1.0, 1.0, 0},
								{1.0, 1.0, 0},
								{1.0, -1.0, 0},
								{-1.0, -1.0, 0}};

int menuoption = 0; // default rotate cube center
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

	glPushMatrix();
		glLoadIdentity();
		glRotatef(-angle, 0, 0, -1);
		// if (angle > 30 && angle < 31) {
		// 	printf(" %f\n", angle);
		// }
		//printf(" %f ", angle);
		GLfloat light_position[] = {-50,0,0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
	glPopMatrix();

	glPushMatrix();
		//glLoadIdentity();
		glColor3f(1,1,1);
		glPointSize(10);
		glRotatef(-angle, 0, 0, 1);
		glBegin(GL_POINTS);
			// Vertex: 3D, float, array
			glVertex3f(-50,0,0); 
		glEnd(); 
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
		GLfloat mat_ambient[] = { 0, 1.0, 0, 1.0 };
		GLfloat mat_diffuse[] = { 0, 1.0, 0, 1.0 };
		GLfloat mat_specular[] = { 0, 0, 0, 1.0 };
  		GLfloat mat_shininess[] = { 0 };

   		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

   		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

		glScalef(15, 1, 15);
		glRotatef(90, 1, 0, 0);
		glCallList(1);
	glPopMatrix();

	// ----ΤΟΙΧΟΙ----
	// πλευρά μπροστινή
	glPushMatrix();
		//glColor3f(1,0,0); // red
		GLfloat mat_emission2[] = { 1,0,0, 1.0 };
		GLfloat mat_specular2[] = { 0, 0, 0, 1.0 };
  		GLfloat mat_shininess2[] = { 0 };

   		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		glTranslatef(0, 5, 10);
		glScalef(5, 5, 1);
		glCallList(1);
	glPopMatrix();

	// πλευρά δεξιά
	glPushMatrix();
		//glColor3f(1,0,1); // mag
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		glTranslatef(5, 5, 0);
		glScalef(1, 5, 10);
		glRotatef(90, 0, 1, 0); 
		glCallList(1);
	glPopMatrix();

	// πλευρά αριστερά
	glPushMatrix();
		//glColor3f(1,0,1); // yellow
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		glTranslatef(-5, 5, 0);
		glScalef(1, 5, 10);
		glRotatef(90, 0, 1, 0); 
		glCallList(1);
	glPopMatrix();

	// πλευρά πάνω
	glPushMatrix();
		//glColor3f(0,0,1); // blue
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		glTranslatef(0, 10, 0);
		glScalef(5, 1, 10);
		glRotatef(90, 1, 0, 0);
		glCallList(1);
	glPopMatrix();

	// πλευρά κάτω
	glPushMatrix();
		//glColor3f(0,1,1); // cyan
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		glScalef(5, 1, 10);
		glRotatef(90, 1, 0, 0);
		glCallList(1);
	glPopMatrix();

	// πλευρά πίσω
	glPushMatrix();
		//glColor3f(1,1,0); // magenda
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
		glTranslatef(0, 5, -10);
		glScalef(5, 5, 1);
		glCallList(1);
	glPopMatrix();

	// ----ΣΤΕΓΗ----
		// στέγη μπροστά
	glPushMatrix();
		//glColor3f(1,1,0); // magenda
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
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
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_emission2);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
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
		GLfloat mat_diffuse3[] = { 0.75,0.75,0.75, 1.0 };
		GLfloat mat_specular3[] = { 1, 1, 1, 1.0 };
  		GLfloat mat_shininess3[] = { 100 };

   		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse3);
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
	cam[2] =  sin(camAngle) * 70;
	
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
	glutIdleFunc(idleFunc);	// called after display is finished	
	glutSpecialFunc (SpecialKeyHandler);
	//glutCreateMenu(menu);
	//glutAddMenuEntry("Rotate center", 0);
    //glutAddMenuEntry("Rotate vector", 1);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop(); /* enter event loop */
	return 0;
}

//ModelView matrix is the concatenation of Model matrix and 
//View Matrix. View Matrix defines the position(location and orientation) 
//of the camera, while model matrix defines the frame’s position 
//of the primitives you are going to draw.
//Projection matrix defines the characteristics of your 
//camera, such as clip planes, field of view, projection method etc.
