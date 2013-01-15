#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "deck.h"

class Player {
public:
	Deck* deck;
	std::string* name;
	int to_drink;
	int drunk;

	Player(std::string nam);

	~Player();

	/* data */
};

#endif
