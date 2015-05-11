#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "CardsDeck.hpp"
#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

#define FOR(i, n) for(int i = 0; i < (n); i++)
#define pb push_back

using std::cout;
using std::string;


int main() {
	std::srand(time(0));
	std::ifstream fi("input.txt");
	std::ofstream fo("output.txt");

	if (fi && fo) {
		Deck deck;
		deck.shuffle();

		cout << "Initial shuffled deck:\n" << deck.getCardsString() << '\n';

		Player human = Player("Lipen", true);
		Player computer = Player("AI", false);

		// pre-exhaust deck:
		// FOR(i, 42) deck.popCard();

		bool ex = true;
		int i = 1;
		while (ex) {
			ex = false;
			Player& player = (i%2) ? human : computer;
			if (player.cardsLeft() < 6) {
				if (player.getCard(deck)) {
					ex = true;
				} else {
					cout << "No more cards in the deck.\n";
				}
			}
			++i;
		}

		cout << human.getName() << "`s cards:\n\t" << human.getCardsString() << '\n';
		cout << computer.getName() << "`s cards:\n\t" << computer.getCardsString() << '\n';

		int turn = 0;
		int turnReal = 0;

		while (true) {
			++turn;
			++turnReal;
			cout << "Turn #" << turnReal << '\n';

			Player& playerTurn = (i%2) ? human : computer;
			Player& playerNext = (i%2) ? computer : human;

			cout << "Turn of the " << playerTurn.getName() << ":\n";

			if (!playerTurn.hasCards()) {
				cout << playerTurn.getName() << " has no more cards left\n";
				break;
			}

			// Player`s turn
			Card &tableCard = playerTurn.popRandomCard();
			cout << playerTurn.getName() << " puts " << tableCard.toString() << " on table\n";

			// Next player`s counter cards
			std::vector<Card> counterCards = playerNext.getCounterCards(tableCard);
			cout << playerNext.getName() << " has " << counterCards.size() << " couter card for " << tableCard.toString() << '\n';

			// Try to beat
			if (counterCards.size() == 0) {
				cout << playerNext.getName() << " takes " << tableCard.toString() << " and skipping its turn\n";
				playerNext.addCard(tableCard);
				++turn;
			} else {
				int i = std::rand() % counterCards.size();
				Card& counterCard = counterCards[i];
				bool succ = playerNext.eraseCard(counterCard);

				if (succ) {
					cout << "Succ\n";
				} else {
					cout << "Nope :c\n";
				}

				cout << playerNext.getName() << " beats " << tableCard.toString() << " with " << counterCard.toString() << "!\n";
			}

			// Get cards after turn
			// For playerTurn:
			if (deck.hasCards()) {
				while (playerTurn.cardsLeft() < 6) {
					if (!playerTurn.getCard(deck)) {
						cout << "No more cards in the deck.\n";
						break;
					}
				}
			} else if (playerTurn.cardsLeft() == 0) {
				cout << playerTurn.getName() << " won!\n";
				break;
			}
			// and same for playerNext:
			if (deck.hasCards()) {
				while (playerNext.cardsLeft() < 6) {
					if (!playerNext.getCard(deck)) {
						cout << "No more cards in the deck.\n";
						break;
					}
				}
			} else if (playerNext.cardsLeft() == 0) {
				cout << playerNext.getName() << " won!\n";
				break;
			}
		}

		fi.close();
		fo.close();
	} else {
		cout << "Unable to open input or output file :c\n";
	}
	return 0;
}
