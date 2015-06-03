#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

#define pb push_back

Player::Player(std::string name, bool humanity)
	: name(name), humanity(humanity) {}

std::string Player::getName() {
	return name;
}

void Player::addCard(Card &card) {
	hand.pb(card);
}

bool Player::hasCards() {
	return !hand.empty();
}

int Player::cardsLeft() {
	return hand.size();
}

Card& Player::popCard() {
	Card &back = hand.back();
	hand.pop_back();
	return back;
}

Card& Player::popCard(int i) {
	Card *card = new Card(hand[i]);
	hand.erase(hand.begin() + i);
	return *card;
}

Card& Player::popRandomCard() {
	int i = std::rand() % hand.size();
	return popCard(i);
}

Card& Player::makeTurn() {
	if (humanity) {
		std::cout << "Your cards:\n\t" << getCardsString() << "\nChoose card:";
		std::string choiceStr;
		std::cin >> choiceStr;
		int choice = std::stoi(choiceStr);

		return popCard(choice-1);
	} else {
		return popRandomCard();
	}
}

// FIXME: Arch
void Player::popCounterCard(int i, const Card &tableCard, const std::vector<Card> &counterCards) {
	Card& counterCard = (const_cast<std::vector<Card>&>(counterCards))[i];
	eraseCard(counterCard);

	std::cout << getName() << " beats " << (const_cast<Card&>(tableCard)).toString() << " with " << counterCard.toString() << "!\n";
}

void Player::makeCounterTurn(const Card &tableCard, const std::vector<Card> &counterCards) {
	if (humanity) {
		std::cout << "Your cards:\n\t" << getCardsString() << "\nChoose card:";
		std::string choiceStr;
		std::cin >> choiceStr;
		int choice = std::stoi(choiceStr);

		if (choice == 0) {
			std::cout << getName() << " takes " << (const_cast<Card&>(tableCard)).toString() << " and skipping its turn\n";
			addCard(const_cast<Card&>(tableCard));
			skipTurn();
		} else {
			popCounterCard(choice-1, tableCard, counterCards);
		}
	} else {
		int i = std::rand() % counterCards.size();
		popCounterCard(i, tableCard, counterCards);
	}

}

std::string Player::getCardsString() {
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

std::vector<Card> Player::getCounterCards(const Card &card) {
	std::vector<Card> counterCards;

	for (Card & handCard : hand) {
		if (handCard.isBeats(card)) {
			counterCards.pb(handCard);
		}
	}

	return counterCards;
}

void Player::eraseCard(const Card &card) {
	for (std::vector<Card>::iterator iter = hand.begin(); iter != hand.end(); ++iter) {
		if (*iter == card) {
			hand.erase(iter);
			return;
		}
	}
}

bool Player::isHuman() {
	return humanity;
}

bool Player::isComputer() {
	return !humanity;
}

void Player::skipTurn() {
	skipsTurn = !skipsTurn;
}

bool Player::isSkipsTurn() {
	return skipsTurn;
}
