#include <string>
#include "player.h"
#include "deck.h"

Player::Player(std::string nam) {
	deck = new Deck(DECK_FACEUP | DECK_SPREAD);
	name = new std::string(nam);
	to_drink = 0;
	drunk = 0;
}

Player::~Player() {
	delete deck;
	delete name;
}