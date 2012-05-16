#include <stdio.h>

void addCRC(char byte, char* p_crcH, char* p_crcL)
{
}

void USARTinit() {}
void USARTsend(char ch)
{
	FILE* gUSART = fopen("out_usart.txt", "a+");
	fputc(ch, gUSART);
	fclose(gUSART);
}


static char gEEPROM[255];
char EEPROM_RD(char adress)
{
	return gEEPROM[adress];
}


void EEPROM_WR(char adress, char data)
{
	gEEPROM[adress] = data;
}

void spi_init() {}
char spi_send(char data)
{
	FILE* gSPI = fopen("out_spi.txt", "a+");
	fprintf(gSPI, "%c", data);
	fclose(gSPI);
}

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>


static float pos = 0.0;

void gl_print_text(char* text, GLfloat x, GLfloat y)
{
	glRasterPos3f(0, 0, 0.0);
	glTranslatef(x, y, 0.0);
	while(*text != 0)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *text++);
	}
	glTranslatef(-x, -y, 0.0);
}

void gl_print_sphere(GLfloat x, GLfloat y)
{
	glColor4f(0.0, 1.0, 0.0, 1.0);
	glTranslatef(x, y, -50.0);
	glutSolidSphere(1.0, 64, 64);
	glTranslatef(-x, -y, +50.0);
}	
	 

void gl_display(void)
{
	static unsigned long frames = 0;
	static struct timespec lastTime;
	static struct timespec nextTime;
	clock_gettime(CLOCK_MONOTONIC, &nextTime);
	
	frames++;


	for(;;) {
		glClear(GL_COLOR_BUFFER_BIT);
	//	glPolygonMode(GL_FRONT, GL_LINE);
	//	glPolygonMode(GL_BACK, GL_FILL);
		glLoadIdentity();
#if 0
		gluLookAt(
			0.0, 10.0, 10.0,
			0.0,  0.0,  0.0,
			0.0,  1.0,  0.0);
#endif
	
		glColor4f(1.0, 0.0, 0.0, 1.0);
		gl_print_text("HUHUHU", 5.0, 4.0);

		unsigned int i;
		for(i = 0; i < 4; i++)
		{
			gl_print_sphere(pos-2.0*i, 2.0*i);
		}
			
		glFlush();
		pos += 0.01;
	}
}

void gl_start(void)
{
	int argc = 1;
	glutInit(&argc, NULL);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Wifly_Light");
	glutDisplayFunc(gl_display);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, 300, 300);
	glutMainLoop();
}

