#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

#define pb push_back

Player::Player(std::string name) : name(name) {}

std::string Player::getName() {
	return name;
}

void Player::addCard(Card &card) {
	hand.pb(card);
}

bool Player::hasCards() {
	return !hand.empty();
}

Card & Player::popCard() {
	Card &back = hand.back();
	hand.pop_back();
	return back;
}

Card & Player::popRandomCard() {
	int i = std::rand() % hand.size();
	Card &randomCard = hand[i];
	size_t xx = hand.size();
	hand.erase(hand.begin() + i);
	if (hand.size() == xx) {
		std::cout << "WEIRD\n";
	}
	return randomCard;
}

std::string Player::getCards() {
	std::string s;

	std::vector<Card>::iterator iter = hand.begin();

	while (true) {
		s += (*iter).toString();
		++iter;
		if (iter != hand.end()) {
			s += ' ';
		} else {
			break;
		}
	}

	return s;
}

bool Player::getCard(Deck &deck) {
	if (deck.hasCards()) {
		Card &card = deck.popCard();
		this -> addCard(card);
		return true;
	}
	return false;
}

std::vector<Card> Player::getCounterCards(Card &card) {
	std::vector<Card> counterCards;

	for (Card &handCard : hand) {
		if (handCard.isBeats(card)) {
			counterCards.pb(handCard);
		}
	}

	return counterCards;
}

bool Player::eraseCard(Card &card) {
	for (std::vector<Card>::iterator iter = hand.begin(); iter != hand.end(); ++iter) {
		if (*iter == card) {
			size_t xx = hand.size();
			hand.erase(iter);
			return (hand.size() != xx);
		}
	}
	return false;
}
