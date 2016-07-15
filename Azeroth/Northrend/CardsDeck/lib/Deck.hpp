#ifndef DECK_HPP
#define DECK_HPP

#include "Card.hpp"


class Deck {
	std::vector<Card> deck;

public:
	Deck();

	void shuffle();
	std::string getCardsString();
	bool hasCards();
	Card& popCard();
};

#endif
