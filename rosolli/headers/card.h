#ifndef CARD_H
#define CARD_H 
enum SUITE {
	SUIT_SPADES, SUIT_CLUBS, SUIT_HEARTS, SUIT_DIAMONDS
};
enum COLOR {
	BLACK, RED
};
class Card {
public:
	SUITE suit;
	int val;
	COLOR color;
	Card(SUITE suite, int value);

};
#endif
