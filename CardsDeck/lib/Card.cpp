#include <string>

#include "CardsDeck.hpp"
#include "Card.hpp"


Card::Card(const Card &other) : rank(other.rank), suit(other.suit) {}

Card::Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

std::string Card::toString() {
	return RankStr[(int)rank] + SuitStr[(int)suit];
}

Rank Card::getRank() {
	return rank;
}

Suit Card::getSuit() {
	return suit;
}

bool Card::isBeats(const Card &card) {
	return (rank > card.rank && suit == card.suit);
}

bool Card::operator==(const Card compCard) {
	return (compCard.rank == rank && compCard.suit == suit);
}
