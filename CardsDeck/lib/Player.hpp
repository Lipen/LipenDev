#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Card.hpp"
#include "Deck.hpp"

class Player {
	std::string name;
	std::vector<Card> hand;

	Player() {}

public:
	Player(std::string name);

	std::string getName();
	void addCard(Card &card);
	bool hasCards();
	Card & popCard();
	Card & popRandomCard();
	std::string getCards();
	bool getCard(Deck &deck);
	std::vector<Card> getCounterCards(Card &card);
	bool eraseCard(Card &card);
};

#endif
