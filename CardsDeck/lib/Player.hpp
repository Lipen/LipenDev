#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Card.hpp"
#include "Deck.hpp"

class Player {
	std::string name;
	bool humanity;
	std::vector<Card> hand;
	bool skipsTurn = false;

	Player() {}

public:
	Player(std::string, bool);

	std::string getName();
	void addCard(Card&);
	bool hasCards();
	int cardsLeft();
	Card& popCard();
	Card& popRandomCard();
	std::string getCardsString();
	bool getCard(Deck&);  //non-const
	std::vector<Card> getCounterCards(const Card&);
	bool eraseCard(const Card&);
	bool isHuman();
	bool isComputer();
	void skipTurn();
	bool isSkipsTurn();
};

#endif
