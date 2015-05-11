#ifndef CARD_HPP
#define CARD_HPP

#include "CardsDeck.hpp"

class Card {
	Rank rank;
	Suit suit;

	Card() {}

public:
	Card(const Card &other);
	Card(Rank rank, Suit suit);

	std::string toString();
	Rank getRank();
	Suit getSuit();
	bool isBeats(const Card &other);

	bool operator==(const Card compCard);
};

#endif
