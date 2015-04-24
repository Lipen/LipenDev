#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>

#define pb push_back

enum class Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
static const std::string RankStr[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

enum class Suit { Hearts, Diamonds, Clubs, Spades };
static const std::string SuitStr[] = { "H", "D", "C", "S" };


class Card {
	Rank rank;
	Suit suit;

	Card() {}

public:
	Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

	std::string toString() {
		return RankStr[(int)rank] + SuitStr[(int)suit];
	}

	Rank getRank() {
		return rank;
	}

	Suit getSuit() {
		return suit;
	}
};


class Deck {
	std::vector<Card> deck;

public:
	Deck() {
		for (int i=0; i<(int)(sizeof(RankStr)/sizeof(*RankStr)); ++i) {
			for (int j=0; j<(int)(sizeof(SuitStr)/sizeof(*SuitStr)); ++j) {
				deck.pb(Card(static_cast<Rank>(i), static_cast<Suit>(j)));
			}
		}
	}

	void shuffle() {
		std::shuffle(begin(deck), end(deck), std::default_random_engine(time(0)));
	}

	std::string getCards() {
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

	bool hasCards() {
		return !deck.empty();
	}

	Card & popCard() {
		Card &back = deck.back();
		deck.pop_back();
		return back;
	}
};


class Player {
	std::string name;
	std::vector<Card> hand;

	Player() {}

public:
	Player(std::string name) : name(name) {}

	std::string getName() {
		return name;
	}

	void addCard(Card &card) {
		hand.pb(card);
	}

	bool hasCards() {
		return !hand.empty();
	}

	Card & popCard() {
		Card &back = hand.back();
		hand.pop_back();
		return back;
	}

	Card & popRandomCard() {
		int i = std::rand() % hand.size();
		Card &randomCard = hand[i];
		hand.erase(hand.begin() + i);
		return randomCard;
	}

	std::string getCards() {
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

	bool getCard(Deck &deck) {
		if (deck.hasCards()) {
			Card &card = deck.popCard();
			this -> addCard(card);
			return true;
		}
		return false;
	}
};


int main() {
	std::srand(42);
	std::ifstream fi("input.txt");
	std::ofstream fo("output.txt");

	if (fi && fo) {
		Deck deck;
		deck.shuffle();

		std::cout << "Initial shuffled deck:\n" << deck.getCards() << std::endl;

		Player players[] = { Player("Lipen"), Player("AI"), Player("Qwerty") };
		int playersAmount = (int)(sizeof(players)/sizeof(*players));

		for (int i=0; i<35; ++i) {
			deck.popCard();
		}

		for (int i=0; i<6; ++i) {
			bool breaked = false;
			for (int j=0; j<playersAmount; ++j) {
				if (!players[j].getCard(deck)) {
					std::cout << "No more cards.\n";
					breaked = true;
					break;
				}
			}
			if (breaked) break;
		}

		std::cout << players[0].getName() << "`s cards:\n\t" << players[0].getCards() << '\n';
		std::cout << players[1].getName() << "`s cards:\n\t" << players[1].getCards() << '\n';
		std::cout << players[2].getName() << "`s cards:\n\t" << players[1].getCards() << '\n';

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
