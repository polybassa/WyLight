#include <stdio.h>
#include "ledstrip.h"

//prototypes
void x86_led_recv(char data);

void addCRC(char byte, char* p_crcH, char* p_crcL) {}
void newCRC(char* p_crcH, char* p_crcL) {}
void USARTinit() {}
void USARTsend(char ch)
{
	FILE* gUSART = fopen("out_usart.txt", "a+");
	fputc(ch, gUSART);
	fclose(gUSART);
}


static uns8 gEEPROM[0x100];
char EEPROM_RD(char adress)
{
	return gEEPROM[adress];
}


void EEPROM_WR(char adress, char data)
{
	gEEPROM[adress] = (uns8)data;
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



static uns8 g_led_status[NUM_OF_LED*3];
bit g_led_off = 1;

void x86_led_recv(char data)
{
	int i;
	for(i = 3*NUM_OF_LED - 1; i > 0; i--)
	{
		g_led_status[i] = g_led_status[i-1];
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
	glutSolidSphere(1.0, 16, 16);
	glTranslatef(-x, -y, +50.0);
}	
	 

void gl_display(void)
{
	static unsigned long frames = 0;
	static struct timespec lastTime;
	static struct timespec nextTime;
	clock_gettime(CLOCK_MONOTONIC, &lastTime);

	for(;;) {
		frames++;
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		// fps calculation
		clock_gettime(CLOCK_MONOTONIC, &nextTime);
		time_t seconds = nextTime.tv_sec - lastTime.tv_sec;
		if(seconds > 0)
		{
			long nanos = nextTime.tv_nsec - lastTime.tv_nsec;
			long millis = seconds*1000 + nanos/1000/1000;
			lastTime = nextTime;
			printf("%f fps\n", 1000.0f*frames/millis);
			frames = 0;
		}

		if(!g_led_off)
		{
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
		}
		glFlush();
	}
}

void* gl_start(void* unused)
{
	int argc = 1;
	glutInit(&argc, NULL);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Wifly_Light LED simulation");
	glutDisplayFunc(gl_display);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 50.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, 300, 300);
	glutMainLoop();
}

