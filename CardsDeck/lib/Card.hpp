#ifndef CARD_HPP
#define CARD_HPP

#include "CardsDeck.hpp"

class Card {
	Rank rank;
	Suit suit;

	Card() {}

public:
	Card(Rank rank, Suit suit);

	std::string toString();
	Rank getRank();
	Suit getSuit();
	bool isBeats(Card &card);

	bool operator==(Card compCard);
};

#endif
