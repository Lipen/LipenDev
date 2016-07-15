#ifndef CARD_HPP
#define CARD_HPP

#include "CardsDeck.hpp"

class Card {
	Rank rank;
	Suit suit;

	Card() {}

public:
	Card(const Card&);
	Card(Rank, Suit);

	std::string toString() const;
	Rank getRank();
	Suit getSuit();
	bool isBeats(const Card&);

	bool operator==(const Card);
};

#endif
