//g++ -o rosolli.exe main.cpp player.* deck.* card.* visuals.* glew32.lib -lfreeglut -lglu32 -lopengl32
//============================================================================
// Name        : Rosolli.cpp
// Author      : Hanzki
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <stdio.h>
#include "visuals.h"
#include "player.h"
#include "deck.h"
#include "card.h"
//using namespace std;

Visuals* v;
Deck* draw_deck = new Deck(DECK_NONETYPE);
Deck* discard_deck = new Deck(DECK_FACEUP);
Player* player1 = new Player("Player one");
//int deal_delay = 3000;
bool running = true;

void display(void) {
	v->display_start();
	v->draw_deck(P_DRAW, draw_deck);
	v->draw_deck(P_DISCARD, discard_deck);
	v->draw_deck(P_PLAYER1, player1->deck);
	v->display_end();
}

void make_a_deck(void) {
	std::cout << "suffling deck\n";
	static int t = 0;
	if (t == 1) {
		running = false;
	}
	Card* temp[52];
	int i = 0;
	while ((temp[i] = discard_deck->pop_card()))
		i++;
	std::cout << i << "\n";
	for (; i > 0; i--) {
		int r = (rand() % i);
		draw_deck->add_card(temp[r]);
		temp[r] = temp[i - 1];
	}
	t++;
}

void init_deck(void) {
	Card* temp[52];
	for (int i = 0; i < 13; i++) {
		temp[(i * 4) + 0] = new Card(SUIT_SPADES, i + 1);
		temp[(i * 4) + 1] = new Card(SUIT_CLUBS, i + 1);
		temp[(i * 4) + 2] = new Card(SUIT_HEARTS, i + 1);
		temp[(i * 4) + 3] = new Card(SUIT_DIAMONDS, i + 1);
	}
	for (int n = 52; n > 0; n--) {
		int r = (rand() % n);
		draw_deck->add_card(temp[r]);
		temp[r] = temp[n - 1];
	}
}

int deal_card(Player* player) {
	COLOR top_color = player->deck->peek_color();
	Card* temp = draw_deck->pop_card();
	if (temp->color == RED)
		if (temp->suit == SUIT_HEARTS)
			std::cout << "HEART   " << temp->val << "\n";
		else
			std::cout << "DIAMOND " << temp->val << "\n";
	else if (temp->suit == SUIT_SPADES)
		std::cout << "SPADE   " << temp->val << "\n";
	else
		std::cout << "CLUB    " << temp->val << "\n";

	int ret = 0;
	if (temp->val < 11) {
		if (!((top_color == RED) && (temp->color == BLACK))) {
			player->deck->add_card(temp);
		} else {
			while (player->deck->peek_color() == RED) {
				ret++;
				discard_deck->add_card(player->deck->pop_card());
			}
			player->deck->add_card(temp);
		}
	} else {

		switch (temp->val) {
		case 11:
			ret = 1 * player->deck->get_size();
			break;

		case 12:
			ret = 2 * player->deck->get_size();
			break;

		case 13:
			ret = 3 * player->deck->get_size();
			break;
		}
		if (ret == 0)
			ret = (temp->val - 10);
		player->deck->clear(discard_deck);
		discard_deck->add_card(temp);
	}

	if (draw_deck->is_empty())
		make_a_deck();
	return ret;
}

void keyPressed(unsigned char key, int x, int y) {
	int tmp;
	switch (key) {
	case 'l':
		std::cout << "lol\n";
		break;
	case 'n':
		if ((tmp = deal_card(player1))) {
			std::cout << "drink " << tmp << "\n";
			fflush(stdout);}
		break;
		case 27:
		exit(0);
		break;
	}
}

void gameloop() {
	std::cout << "GAMELOOOOOOP\n";
	static int b = 0;
	int a = deal_card(player1);
	if (a > b)
		b = a;
	std::cout << "player drinks: " << a << "\n";
	//v->start_update(gameloop);
	/*if(draw_deck->is_empty()){
	 running = false;
	 std::cout << "largest single drink was "<< b << "\n";
	 }*/
}

int main(int argc, char *argv[]) {
	srand(time(0));
	init_deck();
	v = new Visuals(&argc, argv, display, keyPressed);
	v->start_update();
	/*while (running){
	 gameloop();
	 }*/
	return 0;
}
