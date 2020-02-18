#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int** manleft, **manmid, **manright;
int leftdown= 0,rightdown=0;
int man_left=0,pos=0, man_state=0,ground  = 100;


int ball_x=-99,ball_y= -99,ballthrown=0,allowballthrow=1, allowmanmov = 0;
//int manmoved = 0,ballmoved = 0;
int runtime=0;
int man_lasttime=0,  ball_lasttime = 0;
int hand_x =53, hand_y = 81;

void background()
{
	glColor3f(0.45f, 0.65f, 0.10f);
	glBegin(GL_POLYGON);
	glVertex2i(0,0);
	glVertex2i(0,100);
	glVertex2i(800,100);
	glVertex2i(800,0);
	glEnd();
	glFlush();
}

void drawman(int x,int y, int f)
{
	glColor3f(0.6f, 0.1f, 0.1f);
	int x1,y1,x2,y2, i= 0;
	int **ar;
	if(f==0)	ar =manmid;
	else if(f ==1)	ar=manleft;
	else if(f==2)   ar=manright;
	while(1) {
		x1=ar[0][i]+x; 	y1=ar[1][i] +y;
		x2=ar[0][i+1] +x;	y2=ar[1][i+1] +y;
		glBegin(GL_LINES);
		glVertex2f(x1,  y1);
		glVertex2f(x2,y2);
	glEnd();
	glFlush();

		if(ar[0][i+2]== -1)	break;
		else i = i+2;
	}
}


void drawball()
{
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POLYGON);
	glVertex2i(ball_x+1,ball_y+4);
	glVertex2i(ball_x+3,ball_y+3);
	glVertex2i(ball_x+4,ball_y+1);
	glVertex2i(ball_x+4,ball_y-1);
	glVertex2i(ball_x+3,ball_y-3);
	glVertex2i(ball_x+1,ball_y-4);
	glVertex2i(ball_x-1,ball_y-4);
	glVertex2i(ball_x-3,ball_y-3);
	glVertex2i(ball_x-4,ball_y-1);
	glVertex2i(ball_x-4,ball_y+1);
	glVertex2i(ball_x-3,ball_y+3);
	glVertex2i(ball_x-1,ball_y+4);
	glEnd();
	glFlush();
}







 void throw_ball()
{
	ball_x=800;
	srand(time(0));
	ball_y=(rand() %200) + 400;
	
	ball_lasttime=runtime;
	allowmanmov  = 1;
}

void newballposition()
{
	if(ball_y >= hand_y+ground-2 && ball_y <= hand_y+ground + 2 && ball_x >= hand_x+man_left-4 && ball_x <= hand_x+man_left+ 4)
		{
			allowballthrow = 1;
			ballthrown   = 0;
			//ballmoved= 0;
			allowmanmov=0;
			printf("Ball Caught\n");
			//mydisplay();
		}

	else if((ball_x-5) < 0 || ball_y < (hand_y+ground))
	{
		allowballthrow = 1;
		ballthrown   = 0;
		//ballmoved= 0;
		allowmanmov=0;
		
		if(ball_y < (hand_y+ground))
		{
			printf("Ball missed\n");
			//mydisplay();
		}
	}
	else{		ball_x -= 5; ball_y -= 5;}
	
		ball_lasttime=runtime;
}


void mydisplay()
{

		glClear(GL_COLOR_BUFFER_BIT);
	background();
	drawman(man_left, ground, man_state );
	drawball();


	runtime += 100;

	if(runtime == (100 + man_lasttime))
		{
			if(ballthrown ==1)			allowmanmov = 1;
		}
	if(ballthrown == 1) {
		if(runtime == (100 + ball_lasttime))     {
		newballposition();
	}}

	glutTimerFunc(100, mydisplay, 0);
  }
  void move_man (int p,int s)
{
	switch(p % 4)
	{
		case 0:
			if(s==1) man_left -=17;
			else man_left += 20;
			man_state = 0;
		break;
		case 1:
			if(s==1) man_left -=22;
			else man_left += 17;
			man_state = 2;
		break;
		
		case 2:
			if(s==1) man_left -=15;
			else man_left += 22;
			man_state = 0;
		break;
		case 3:
			if(s==1) man_left -=20;
			else man_left += 15;
			man_state = 1;
		break;
	}
	//mydisplay();
	man_lasttime  =runtime; 
	//manmoved = 1;
	allowmanmov=0;
	//manmoved = 0;
}

void mykeyboard(unsigned char key,int x,int y)
{
	
	if(key == 32)
		{
			if(allowballthrow==1)
			{
				throw_ball();
				ballthrown=1;
				allowballthrow=0;
			}
		}
}

void specialkey(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			leftdown  = 1;
			if(!rightdown && allowmanmov == 1)
			{
				if(pos > 0)		move_man(--pos, 1);
			}
			break;
		case GLUT_KEY_RIGHT:
			rightdown = 1;
			if(!leftdown && allowmanmov == 1)
			{
				if(pos < 29)
						move_man(++pos, 2);
			}
			break;
	}
}

void specialkeyup(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:						
				leftdown  = 0;
			break;
		case GLUT_KEY_RIGHT:
			rightdown = 0;
			break;
	}
}





void myinit() {
	gluOrtho2D(0.0,800.0,0.0,600.0);
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	glLineWidth(2.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	FILE *fp1 = fopen("manleft.txt", "r");
	manleft =malloc(sizeof(int*)*2);
	manleft[0]=malloc(sizeof(int)*69);
	manleft[1]=malloc(sizeof(int)*69);
	for(int i=0; i <68;i++)
	{
		fscanf(fp1, "%d", &manleft[0][i]);
		fscanf(fp1, "%d", &manleft[1][i]);
	}
	manleft[0][68] = -1;manleft[1][68]=-1;

	FILE *fp2 = fopen("manmid.txt", "r");
	manmid =malloc(sizeof(int*)*2);
	manmid[0]=malloc(sizeof(int)*65);
	manmid[1]=malloc(sizeof(int)*65);
	for(int i=0; i <64;i++)
	{
		fscanf(fp2, "%d", &manmid[0][i]);
		fscanf(fp2, "%d", &manmid[1][i]);
	}
	manmid[0][64] = -1;manmid[1][64]=-1;
	FILE *fp3 = fopen("manright.txt", "r");
	manright =malloc(sizeof(int*)*2);
	manright[0]=malloc(sizeof(int)*71);
	manright[1]=malloc(sizeof(int)*71);
	for(int i=0; i <70;i++)
	{
		fscanf(fp3, "%d", &manright[0][i]);
		fscanf(fp3, "%d", &manright[1][i]);
	}
	manright[0][70] = -1;manright[1][70]=-1;

}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Catch the Ball");
	myinit();
	glutKeyboardFunc(mykeyboard);
	glutSpecialFunc(specialkey);
	glutSpecialUpFunc(specialkeyup);
	glutDisplayFunc(mydisplay);
	glutTimerFunc(100, mydisplay, 0);
	glutMainLoop();
}