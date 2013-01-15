#include <iostream>
#include <cstdio>
#include <string>
#include "visuals.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "deck.h"
#include "card.h"
#include "player.h"

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) width / (GLfloat) height, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);

}

unsigned char* Visuals::rgb_tga(const char *filename, int *w, int *h) {
	printf("start\n");
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Could not open the file: %s\n", filename);
		exit(0);
	}

	unsigned char header[20];

	//read all 18 bytes of the header
	fread(header, sizeof(char), 18, file);

	//should be image type 2 (color) or type 10 (rle compressed color)
	if (header[2] != 2 && header[2] != 10) {
		fclose(file);
		exit(0);
	}

	if (header[0]) {
		fseek(file, header[0], SEEK_CUR);
	}

	// get the size and bitdepth from the header
	int m_width = header[13] * 256 + header[12];
	int m_height = header[15] * 256 + header[14];
	int m_bpp = header[16] / 8;
	*w = m_width;
	*h = m_height;

	if (m_bpp != 3 && m_bpp != 4) {
		fclose(file);
		exit(0);
	}

	int imageSize = m_width * m_height * m_bpp;

	//allocate memory for image data
	unsigned char *data = new unsigned char[imageSize];

	//read the uncompressed image data if type 2
	if (header[2] == 2) {
		fread(data, sizeof(char), imageSize, file);
	}

	long ctpixel = 0, ctloop = 0;

	//read the compressed image data if type 10
	if (header[2] == 10) {
		// stores the rle header and the temp color data
		unsigned char rle;
		unsigned char color[4];

		while (ctpixel < imageSize) {
			// reads the the RLE header
			fread(&rle, 1, 1, file);

			// if the rle header is below 128 it means that what folows is just raw data with rle+1 pixels
			if (rle < 128) {
				fread(&data[ctpixel], m_bpp, rle + 1, file);
				ctpixel += m_bpp * (rle + 1);
			}

			// if the rle header is equal or above 128 it means that we have a string of rle-127 pixels
			// that use the folowing pixels color
			else {
				// read what color we should use
				fread(&color[0], 1, m_bpp, file);

				// insert the color stored in tmp into the folowing rle-127 pixels
				ctloop = 0;
				while (ctloop < (rle - 127)) {
					data[ctpixel] = color[0];
					data[ctpixel + 1] = color[1];
					data[ctpixel + 2] = color[2];
					if (m_bpp == 4) {
						data[ctpixel + 3] = color[3];
					}

					ctpixel += m_bpp;
					ctloop++;
				}
			}
		}
	}

	ctpixel = 0;

	//Because TGA file store their colors in BGRA format we need to swap the red and blue color components
	unsigned char temp;
	while (ctpixel < imageSize) {
		temp = data[ctpixel];
		data[ctpixel] = data[ctpixel + 2];
		data[ctpixel + 2] = temp;
		ctpixel += m_bpp;
	}

	//close file
	fclose(file);
	printf("ok\n");
	return data;
}

/*void keyPressed (unsigned char key, int x, int y) {
 switch(key){
 case 'l':
 std::cout << "lol\n";
 case 'n':
 case 27:
 exit(0);
 }
 }*/

void update(int value) {
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

void Visuals::start_update() {
	glutTimerFunc(25, update, 0);
	glutMainLoop();
}
Visuals::Visuals(int* argc, char* argv[], void display(void),
		void keypress(unsigned char, int, int)) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 500);
	glutCreateWindow("Rosolli-Laarnio");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width, height;
	unsigned char* data = rgb_tga(".\\allcards.tga", &width, &height);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	//glutKeyboardUpFunc(keyUp);
	//glutSpecialFunc(keySpecial);
	//glutSpecialUpFunc(keySpecialUp);

	/*glutTimerFunc(25,update,0);
	 glutMainLoop();*/
}

void Visuals::draw_deck(DECK_POS pos, Deck* deck) {
	if (deck->is_empty())
		return;
	SUITE suit;
	int val;
	if (deck->type == DECK_FACEUP) {
		suit = deck->peek_card()->suit;
		val = deck->peek_card()->val;
	} else {
		suit = SUIT_HEARTS;
		val = 14;
	}

	glPushMatrix();

	switch (pos) {
	case P_DRAW:
		glTranslatef(1.0f, 0.1f, -1.5f);
		break;
	case P_DISCARD:
		glTranslatef(-3.0f, 0.1f, -1.5f);
		break;
	case P_PLAYER1:
		glTranslatef(-1.0f, 0.1f, 2.5f);
		break;
	}
	float height = (float) deck->get_size() / 52;
	int xoff, yoff;
	switch (suit) {
	case SUIT_HEARTS:
		xoff = 0;
		yoff = 1024;
		break;
	case SUIT_DIAMONDS:
		xoff = 512;
		yoff = 1024;
		break;
	case SUIT_SPADES:
		xoff = 0;
		yoff = 512;
		break;
	case SUIT_CLUBS:
		xoff = 512;
		yoff = 512;
		break;
	}
	GLfloat pts[4][2];
	//top-left corner
	pts[0][0] = (GLfloat) (xoff + (80 * ((val - 1) % 6))) / 1024;	//x
	pts[0][1] = (GLfloat) (yoff - (120 * ((val - 1) / 6))) / 1024;	//y
	//top-right corner
	pts[1][0] = (GLfloat) (80 + xoff + (80 * ((val - 1) % 6))) / 1024;	//x
	pts[1][1] = (GLfloat) (yoff - (120 * ((val - 1) / 6))) / 1024;	//y
	//lower-right corner
	pts[2][0] = (GLfloat) (80 + xoff + (80 * ((val - 1) % 6))) / 1024;	//x
	pts[2][1] = (GLfloat) (yoff - 120 - (120 * ((val - 1) / 6))) / 1024;	//y
	//lower-left corner
	pts[3][0] = (GLfloat) (xoff + (80 * ((val - 1) % 6))) / 1024;	//x
	pts[3][1] = (GLfloat) (yoff - 120 - (120 * ((val - 1) / 6))) / 1024;	//y

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	//top of deck
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, height, 0.0f);
	glVertex3f(0.0f, height, 0.0f);
	//right side
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 3.0f);
	glVertex3f(2.0f, height, 3.0f);
	glVertex3f(2.0f, height, 0.0f);
	//bottom of deck
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glVertex3f(2.0f, 0.0f, 3.0f);
	glVertex3f(2.0f, height, 3.0f);
	glVertex3f(0.0f, height, 3.0f);
	//left side
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glVertex3f(0.0f, height, 3.0f);
	glVertex3f(0.0f, height, 0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	//card on top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(pts[0][0], pts[0][1]);
	glVertex3f(0.0f, height, 0.0f);
	glTexCoord2f(pts[1][0], pts[1][1]);
	glVertex3f(2.0f, height, 0.0f);
	glTexCoord2f(pts[2][0], pts[2][1]);
	glVertex3f(2.0f, height, 3.0f);
	glTexCoord2f(pts[3][0], pts[3][1]);
	glVertex3f(0.0f, height, 3.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Visuals::draw_enviroment() {
	return;
}

void Visuals::display_start() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -13.0f);
	glRotatef(32, 1.0f, 0.0f, 0.0f);

	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 7.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

}

void Visuals::display_end() {
	glutSwapBuffers();
}
