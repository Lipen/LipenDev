#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

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
		FOR(i, 42) deck.popCard();

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

		int playerTurn = 0;

		while (true) {
			if (!players[playerTurn].hasCards()) {
				std::cout << players[playerTurn].getName() << " has no more cards left\n";
				break;
			}

			Card &tableCard = players[playerTurn].popRandomCard();
			std::cout << players[playerTurn].getName() << " puts " << tableCard.toString() << " on table\n";

			// if (!players[playerTurn].getCard(deck)) {
			// 	std::cout << "No more cards in the deck.\n";
			// }

			playerTurn = (playerTurn + 1) % playersAmount;
		}

		fi.close();
		fo.close();
	} else {
		std::cout << "Unable to open input or output file :c\n";
	}
	return 0;
}
