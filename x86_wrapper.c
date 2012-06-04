#include <stdio.h>
#include "ledstrip.h"

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
	x86_led_recv(data);
	FILE* gSPI = fopen("out_spi.txt", "a+");
	fprintf(gSPI, "%c", data);
	fclose(gSPI);
}

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>



static char g_led_status[NUM_OF_LED*3];

void x86_led_init(void)
{
#if 1
	char r = 0;
	char g = 0;
	char b = 0;
	char i;
	for(i = 0; i < NUM_OF_LED;i++)
	{
		g_led_status[3*i] = r;
		g_led_status[3*i+1] = g;
		g_led_status[3*i+2] = b;
		r += 255/NUM_OF_LED/4;
		g += 255/NUM_OF_LED/2;
		b += 255/NUM_OF_LED;		
	}

struct cmd_set_color color;
	color.addr[0] = 0xff;
	color.addr[1] = 0x0;
	color.addr[2] = 0x0;
	color.addr[3] = 0x0;
	color.red = 255;
	color.green = 255;
	color.blue = 255;
	color.reserved0 = 0;
	color.reserved1 = 0;
	ledstrip_set_color(&color);
#endif
}

void x86_led_recv(char data)
{
	printf("x86_led_recv: 0x%02x\n", data);
	char i = NUM_OF_LED - 1;
	while(i > 0)
	{
		g_led_status[i] = g_led_status[i-1];
		i--;
	}
	g_led_status[0] = data;
}

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

void gl_print_sphere(GLfloat x, GLfloat y, float r, float g, float b)
{
	glColor4f(r, g, b, 1.0);
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

	for(;;) {
		frames++;
		glClear(GL_COLOR_BUFFER_BIT);
	//	glPolygonMode(GL_FRONT, GL_LINE);
	//	glPolygonMode(GL_BACK, GL_FILL);
		glLoadIdentity();
#if 0
		gluLookAt(
			0.0, 10.0, 10.0,
			0.0,  0.0,  0.0,
			0.0,  1.0,  0.0);

		glColor4f(1.0, 0.0, 0.0, 1.0);
		gl_print_text("HUHUHU", 5.0, 4.0);
#endif
		if( 0 == frames % 50)
			printf("frame: %lu\n", frames);

		unsigned int i;
		for(i = 0; i < NUM_OF_LED; i++)
		{
			float x = -16.0 + 2.0 * (i % 8);
			float y = 2.0 * (i / 8);			
			float r = (float)g_led_status[3*i] / 255.0;
			float g = (float)g_led_status[3*i+1] / 255.0;
			float b = (float)g_led_status[3*i+2] / 255.0;
			gl_print_sphere(x, y, r, g, b);
		}
			
		glFlush();
	}
}

void gl_start(void)
{
	int argc = 1;
	x86_led_init();
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

