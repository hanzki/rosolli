#ifndef VISUALS_H
#define VISUALS_H
#include "deck.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

enum DECK_POS {
	P_DRAW, P_DISCARD, P_PLAYER1
};
class Visuals {
private:
	unsigned char* rgb_tga(const char *filename, int *w, int *h);
	GLfloat** get_cardtex(SUITE suit, int val, GLfloat* points);
	GLuint textureId;

public:
	Visuals(int* argc, char* argv[], void display(void),
			void keypress(unsigned char, int, int));
	void draw_deck(DECK_POS pos, Deck* deck);
	void draw_enviroment();
	void display_start();
	void display_end();
	void start_update();
	~Visuals();

	/* data */
};
#endif
