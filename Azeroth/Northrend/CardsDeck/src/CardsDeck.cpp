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

/*
class Game {
	std::vector<Player> players;
	int turn = 0;
	Game() {}

public:
	Game(std::vector<string> &playersNames) {
		for (string playerName : playersNames) {
			players.pb(Player(playerName));
		}
	}

	Player& getTurnPlayer() {
		return players[turn];
	}
};
*/


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

		while (true) {
			++turn;

			Player& playerTurn = (turn%2) ? human : computer;
			Player& playerNext = (turn%2) ? computer : human;

			cout << "Turn #" << turn << " of the " << playerTurn.getName() << ":\n";

			if (playerTurn.isSkipsTurn()) {
				cout << playerTurn.getName() << " skipping its turn\n";
				playerTurn.skipTurn();
				continue;
			}

			if (!playerTurn.hasCards()) {
				cout << playerTurn.getName() << " has no more cards left\n";
				break;
			}

			// Player`s turn
			// Card &tableCard = playerTurn.popRandomCard();
			Card& tableCard = playerTurn.makeTurn();
			cout << playerTurn.getName() << " puts " << tableCard.toString() << " on table\n";

			// Next player`s counter cards
			std::vector<Card> counterCards = playerNext.getCounterCards(tableCard);
			// Output counter cards
			cout << playerNext.getName() << " has " << counterCards.size() << " couter card for " << tableCard.toString();
			if (counterCards.size() > 0){
				cout << ": ";
				std::vector<Card>::iterator iter = counterCards.begin();
				while (true) {
					cout << (*iter).toString();
					++iter;
					if (iter != counterCards.end()) {
						cout << ' ';
					} else {
						break;
					}
				}
			}
			cout << ".\n";

			// Try to beat
			if (counterCards.size() == 0) {
				cout << playerNext.getName() << " takes " << tableCard.toString() << " and skipping its turn\n";
				playerNext.addCard(tableCard);
				playerNext.skipTurn();
			} else {
				playerNext.makeCounterTurn(tableCard, counterCards);
				// int i = std::rand() % counterCards.size();
				// Card& counterCard = counterCards[i];
				// playerNext.eraseCard(counterCard);

				// cout << playerNext.getName() << " beats " << tableCard.toString() << " with " << counterCard.toString() << "!\n";
			}

			// Get cards after turn
			// For playerTurn:
			if (deck.hasCards()) {
				while (playerTurn.cardsLeft() < 6) {
					if (!playerTurn.getCard(deck)) {
						cout << "No more cards in the deck.\n";
						break;
					} else {
						cout << playerTurn.getName() << " got a card from a deck.\n";
					}
				}
			} else if (playerTurn.cardsLeft() == 0) {
				cout << playerTurn.getName() << " won!\n";
				cout << playerNext.getName() << " left cards: " << playerNext.getCardsString() << '\n';
				break;
			}
			// and same for playerNext:
			if (deck.hasCards()) {
				while (playerNext.cardsLeft() < 6) {
					if (!playerNext.getCard(deck)) {
						cout << "No more cards in the deck.\n";
						break;
					} else {
						cout << playerNext.getName() << " got a card from a deck.\n";
					}
				}
			} else if (playerNext.cardsLeft() == 0) {
				cout << playerNext.getName() << " won!\n";
				cout << playerTurn.getName() << " left cards: " << playerNext.getCardsString() << '\n';
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
