#ifndef DECK_H
#define DECK_H
#include "card.h"

enum DECK_TYPE {
	DECK_NONETYPE = 0x0, DECK_FACEUP = 0x1, DECK_SPREAD = 0x2
};

class Deck {
public:
	Card** cards;
	int type;

	Deck(int dtype);

	~Deck();

	void clear(Deck* dest);

	void add_card(Card* card);

	Card* peek_card(void);

	COLOR peek_color(void);

	Card* pop_card(void);

	int get_size(void);

	bool is_empty(void);

	/* data */
};
#endif
