#include <cassert>
#include "deck.h"
#include "card.h"

Deck::Deck(int dtype) {
	type = dtype;
	cards = new Card*[105];
	cards[0] = 0;
}

Deck::~Deck() {
	clear(0);
	delete[] cards;
}

void Deck::clear(Deck* dest) {
	Card* temp = pop_card();
	while (temp) {
		if (dest) {
			dest->add_card(temp);
		} else
			delete temp;
		temp = pop_card();
	}
}

void Deck::add_card(Card* card) {
	assert(card);
	int size = get_size();
	cards[size] = card;
	cards[size + 1] = 0;
}

Card* Deck::peek_card(void) {
	int size = get_size();
	if (size) {
		return cards[size - 1];
	} else
		return 0;
}

COLOR Deck::peek_color(void) {
	if (cards[0]) {
		return cards[get_size() - 1]->color;
	} else
		return BLACK;
}

Card* Deck::pop_card(void) {
	int size = get_size();
	if (size) {
		Card* temp = cards[size - 1];
		cards[size - 1] = 0;
		return temp;
	} else
		return 0;
}

int Deck::get_size(void) {
	int size = 0;
	while (cards[size])
		size++;
	return size;
}

bool Deck::is_empty(void) {
	if (cards[0])
		return false;
	else
		return true;
}