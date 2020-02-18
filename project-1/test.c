#include <GL/freeglut.h>

void init() {
	gluOrtho2D(0.0, 800, 0.0, 600);
	glClearColor(1.0, 0.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void disp() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(960, 0);
	glutCreateWindow("Catch the Ball");
	init();
	glutDisplayFunc(disp);

	glutMainLoop();

	return 0;
}
