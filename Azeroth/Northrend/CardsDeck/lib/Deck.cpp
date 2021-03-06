#include <string>
#include <vector>
#include <algorithm>

#include "CardsDeck.hpp"
#include "Card.hpp"
#include "Deck.hpp"

#define pb push_back


Deck::Deck() {
	for (int i=0; i<(int)size(RankStr); ++i) {
		for (int j=0; j<(int)size(SuitStr); ++j) {
			deck.pb(Card(static_cast<Rank>(i), static_cast<Suit>(j)));
		}
	}
}

void Deck::shuffle() {
	std::shuffle(deck.begin(), deck.end(), RandomEngine);
}

std::string Deck::getCardsString() {
	std::string s;
	std::vector<Card>::iterator iter = deck.begin();

	while (true) {
		s += (*iter).toString();
		++iter;
		if (iter != deck.end()) {
			s += ' ';
		} else {
			break;
		}
	}

	return s;
}

bool Deck::hasCards() {
	return !deck.empty();
}

Card& Deck::popCard() {
	Card &back = deck.back();
	deck.pop_back();
	return back;
}
