#include "card.h"

Card::Card(SUITE suite, int value) {
	suit = suite;
	val = value;
	if ((suite == SUIT_DIAMONDS) || (suite == SUIT_HEARTS))
		color = RED;
	else
		color = BLACK;
}
