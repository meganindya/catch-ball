#include <GL/glut.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define WW 600
#define WH 600



// Initial Functions

void disp(void);
void insertImages(void);
int **loadData(FILE *);


// Response Functions

void spclKeyPress(int, int, int);
void spclKeyRelsd(int, int, int);
void normKeyPress(unsigned char, int, int);


// Action Functions

void moveMan(int, int);
void loop(int);
void throwBall(void);
void updateBall(void);


// Drawing Functions

void drawEnvironment(void);
void drawFigure(int, int, int);
void drawBall(void);
void drawLine(int, int, int, int);




// Global Variables

bool keyLeftDown = false, keyRightDown = false;
int **manLt, **manMn, **manRt;
int manOffset = 0, manPos = 0, manState = 0;
int groundOffset = 100;
int ballX = -99, ballY = -99;
bool ballThrown = false, enableBallThrow = true, enableManMove = true;
bool manPosChanged = false, ballPosChanged = false;
int refreshPeriod = 100, runtime = 0, manTimeX = 1, ballTimeX = 1;
int manLastMoved = 0, ballLastMoved = 0;
int handX = 53, handY = 81;
bool enableSuccessMessage = false, enableMissMessage = false;
int trials = 0, wins = 0;




// Main Function





// Initial Functions

void disp(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	drawEnvironment();
	drawFigure(manState, manOffset, groundOffset);
	drawBall();
}

void insertImages(void) {
	char **fn = (char **) malloc(sizeof(char *) * 3);
	fn[0] = "man-lt.txt"; fn[1] = "man-mn.txt"; fn[2] = "man-rt.txt";
	for(int i = 0; i < 3; i++) {
		FILE *fp = fopen(fn[i], "r");
		if(fp == NULL) {
			printf(" -- file \"%s\" missing\n", fn[i]);
			break; exit(0);
		}
		else {
			int **temp = loadData(fp);
			switch(i) {
				case 0:	manLt = temp; break;
				case 1:	manMn = temp; break;
				case 2:	manRt = temp; break;
			}
		}
	}
}

int **loadData(FILE *fp) {
	int x, y, c = 0;
	int arr[2][128];
	while(fscanf(fp, "%d", &x) == 1) {
		fscanf(fp, "%d", &y);
		arr[0][c] = x;
		arr[1][c] = y;
		c++;
	}

	int **temp;
	temp = (int **) malloc(sizeof(int *) * 2);
	temp[0] = (int *) malloc(sizeof(int) * c + 1);
	temp[1] = (int *) malloc(sizeof(int) * c + 1);
	for(int i = 0; i < c; i++) {
		temp[0][i] = arr[0][i];
		temp[1][i] = arr[1][i];
	}
	temp[0][c] = -1;
	temp[1][c] = -1;

	return temp;
}



// Response Functions

void normKeyPress(unsigned char key, int x, int y) {
	switch(key) {
		case 27:		// Key Esc
			exit(0);
			break;
		case 32:		// Key Space
			if(enableBallThrow == true) {
				throwBall();
				ballThrown = true;
				enableBallThrow = false;
			}
			break;
	}
}

void spclKeyPress(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT	:		// Key Arrow Left  Pressed
			keyLeftDown  = true;
			if(!keyRightDown && enableManMove == true) {
				printf("Left Key\n");
				if(manPos > 0)		moveMan(--manPos, 1);
			}
			break;
		case GLUT_KEY_RIGHT	:		// Key Arrow Right Pressed
			keyRightDown = true;
			if(!keyLeftDown && enableManMove == true)  {
				printf("Right Key\n");
				if(manPos < 29)		moveMan(++manPos, 2);
			}
			break;
	}
}

void spclKeyRelsd(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT	:		// Key Arrow Left  Released
			keyLeftDown  = false;
			break;
		case GLUT_KEY_RIGHT	:		// Key Arrow Right Released
			keyRightDown = false;
			break;
	}
}



// Action Functions

void moveMan(int p, int s) {
	switch(p % 4) {
		case 0:
			if(s == 1)	manOffset -= 17;
			else		manOffset += 20;
			manState = 0;
			break;
		case 1:
			if(s == 1)	manOffset -= 22;
			else		manOffset += 17;
			manState = 2;
			break;
		case 2:
			if(s == 1)	manOffset -= 15;
			else		manOffset += 22;
			manState = 0;
			break;
		case 3:
			if(s == 1)	manOffset -= 20;
			else		manOffset += 15;
			manState = 1;
			break;
	}
	glutPostRedisplay();
	manLastMoved  = runtime; 
	manPosChanged = true;
	enableManMove = false;
	manPosChanged = false;
}

void loop(int val) {
	runtime += refreshPeriod;

	if(runtime == ((manTimeX * refreshPeriod) + manLastMoved))
		enableManMove = true;
	if(ballThrown == true) {
		if(runtime == ((ballTimeX * refreshPeriod) + ballLastMoved))
		updateBall();
	}
	
	if(manPosChanged == true || ballPosChanged == true)
		glutPostRedisplay();

	glutTimerFunc(refreshPeriod, loop, 0);
}

void throwBall(void) {
	ballX = WW - 4;
	srand(time(0));
	ballY = (rand() % (WH / 2)) + (WH / 2);
	printf("Ball Released at height %d\n", ballY);
	ballLastMoved = runtime;
	ballPosChanged = true;
	enableManMove  = true;
	enableSuccessMessage = false;
	enableMissMessage    = false;
	trials++;
}

void updateBall(void) {
	if(ballY >= handY + groundOffset - 2 && ballY <= handY + groundOffset + 2)
		if(ballX >= handX + manOffset - 4 && ballX <= handX + manOffset + 4) {
			enableBallThrow = true;
			ballThrown      = false;
			ballPosChanged  = false;
			enableManMove   = false;
			enableSuccessMessage = true;
			printf("Ball Caught !!\n");
			wins++;
			glutPostRedisplay();
			goto skip;
		}

	srand(time(0));
	int randX = (rand() % 2), randY = (rand() % 2);
	if(randX == 0 && randY == 0) {
		randX = 0;	randY = 1;
	}

	if((ballX - 5 * randX) < 0 || ballY < (handY + groundOffset)) {
		enableBallThrow = true;
		ballThrown      = false;
		ballPosChanged  = false;
		enableManMove   = false;
		if(ballY < (handY + groundOffset)) {
			ballY -= 5;
			enableMissMessage = true;
			glutPostRedisplay();
			goto skip;
		}
	}
	else {
		ballX -= 5 * randX; ballY -= 5 * randY;
	}
	skip: printf("Ball\t%d\t%d\n", ballX, ballY);
	ballLastMoved = runtime;
}



// Drawing Functions

void drawEnvironment(void) {
	glColor3f(0.45f, 0.65f, 0.10f);
	glBegin(GL_POLYGON);
	glVertex2i(0,	0);
	glVertex2i(0,	groundOffset);
	glVertex2i(WW,	groundOffset);
	glVertex2i(WW,	0);
	glEnd();
	glFlush();
}

void drawFigure(int f, int xpos, int ypos) {
	glColor3f(0.6f, 0.1f, 0.1f);
	int xa, ya, xb, yb, i = 0, **ar;
	switch(f) {
		case 0:	ar = manMn; break;
		case 1:	ar = manLt; break;
		case 2:	ar = manRt; break;
	}
	while(1) {
		xa = ar[0][i + 0] + xpos;
		ya = ar[1][i + 0] + ypos;
		xb = ar[0][i + 1] + xpos;
		yb = ar[1][i + 1] + ypos;
		drawLine(xa, ya, xb, yb);

		if(ar[0][i + 2] == -1)	break;
		else i += 2;
	}
}

void drawBall(void) {
	glColor3f(0.17f, 0.17f, 0.33f);
	glBegin(GL_POLYGON);
		glVertex2i(ballX + 1, ballY + 4);
		glVertex2i(ballX + 3, ballY + 3);
		glVertex2i(ballX + 4, ballY + 1);
		glVertex2i(ballX + 4, ballY - 1);
		glVertex2i(ballX + 3, ballY - 3);
		glVertex2i(ballX + 1, ballY - 4);
		glVertex2i(ballX - 1, ballY - 4);
		glVertex2i(ballX - 3, ballY - 3);
		glVertex2i(ballX - 4, ballY - 1);
		glVertex2i(ballX - 4, ballY + 1);
		glVertex2i(ballX - 3, ballY + 3);
		glVertex2i(ballX - 1, ballY + 4);
	glEnd();
	glFlush();
}

void drawLine(int xa, int ya, int xb, int yb) {
	glBegin(GL_LINES);
		glVertex2i(xa, ya);
		glVertex2i(xb, yb);
	glEnd();
	glFlush();
}

void myinit(void) {
	gluOrtho2D(0.0, WW, 0.0, WH);
	glClearColor(0.76f, 0.84f, 0.93f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	insertImages();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WW, WH);
	glutInitWindowPosition(960, 0);
	glutCreateWindow("Catch the Ball");
	myinit();
	glutSpecialFunc(spclKeyPress);
	glutSpecialUpFunc(spclKeyRelsd);
	glutKeyboardFunc(normKeyPress);
	glutDisplayFunc(disp);
	glutTimerFunc(refreshPeriod, loop, 0);
	glutMainLoop();
}