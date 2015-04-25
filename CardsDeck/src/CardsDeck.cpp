#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "CardsDeck.hpp"
#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

#define FOR(i, n) for(int i = 0; i < (n); i++)


int main() {
	std::srand(time(0));
	std::ifstream fi("input.txt");
	std::ofstream fo("output.txt");

	if (fi && fo) {
		Deck deck;
		deck.shuffle();

		std::cout << "Initial shuffled deck:\n" << deck.getCards() << std::endl;

		Player players[] = { Player("Lipen"), Player("AI"), Player("Qwerty") };
		int playersAmount = (int)(sizeof(players)/sizeof(*players));

		// pre-exhaust deck:
		// FOR(i, 42) deck.popCard();

		FOR(i, 6) {
			bool breaked = false;
			FOR(j, playersAmount) {
				if (!players[j].getCard(deck)) {
					std::cout << "No more cards.\n";
					breaked = true;
					break;
				}
			}
			if (breaked) break;
		}

		FOR(i, playersAmount)
			std::cout << players[i].getName() << "`s cards:\n\t" << players[i].getCards() << '\n';

		int turn = 0;

		while (true) {
			Player &playerTurn = players[turn];
			Player &playerNext = players[(turn+1)%playersAmount];

			std::cout << "Turn of the " << playerTurn.getName() << ":\n";
			FOR(i, playersAmount) std::cout << "\t" << players[i].getName() << "`s cards:\t" << players[i].getCards() << '\n';

			if (!playerTurn.hasCards()) {
				std::cout << playerTurn.getName() << " has no more cards left\n";
				break;
			}

			// Player`s turn
			Card &tableCard = playerTurn.popRandomCard();
			std::cout << playerTurn.getName() << " puts " << tableCard.toString() << " on table\n";

			// Next player`s counter cards
			std::vector<Card> counterCards = playerNext.getCounterCards(tableCard);

			std::cout << playerNext.getName() << " has " << counterCards.size() << " couter card for " << tableCard.toString() << '\n';

			if (counterCards.size() == 0) {
				std::cout << playerNext.getName() << " takes " << tableCard.toString() << " and skipping its turn\n";
				playerNext.addCard(tableCard);
				++turn;
			} else {
				int i = std::rand() % counterCards.size();
				Card &counterCard = counterCards[i];
				bool succ = playerNext.eraseCard(counterCard);

				if (succ) {
					std::cout << "Succ\n";
				} else {
					std::cout << "Nope :c\n";
				}

				std::cout << playerNext.getName() << " beats " << tableCard.toString() << " with " << counterCard.toString() << "!\n";
			}

			// Get cards after turn
			if (!playerTurn.getCard(deck)) {
				bool breaked = false;
				FOR(i, playersAmount) {
					if (!players[i].hasCards()) {
						std::cout << players[i].getName() << " won!\n";
						breaked = true;
						break;
					}
				}
				if (breaked) break;
				std::cout << "No more cards in the deck.\n";
			}

			turn = (turn + 1) % playersAmount;
		}

		fi.close();
		fo.close();
	} else {
		std::cout << "Unable to open input or output file :c\n";
	}
	return 0;
}
