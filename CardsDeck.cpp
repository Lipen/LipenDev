#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>

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

	Card popCard() {
		Card back = deck.back();
		deck.pop_back();
		return back;
	}
};


class Player {
	std::string name;
	std::vector<Card> hand;

public:
	Player(std::string name) : name(name) {}

	std::string getName() {
		return name;
	}

	void addCard(Card card) {
		hand.pb(card);
	}

	Card popCard() {
		Card back = hand.back();
		hand.pop_back();
		return back;
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
};


int main() {
	std::ifstream fi("input.txt");
	std::ofstream fo("output.txt");

	if (fi && fo) {
		Deck deck;
		deck.shuffle();

		std::cout << "Initial shuffled deck:\n" << deck.getCards() << std::endl;

		Player player1("Lipen");
		Player player2("AI");

		for (int i=0; i<6; ++i) {
			player1.addCard(deck.popCard());
			player2.addCard(deck.popCard());
		}

		std::cout << player1.getName() << "`s cards:\n\t" << player1.getCards() << '\n';
		std::cout << player2.getName() << "`s cards:\n\t" << player2.getCards() << '\n';

		fi.close();
		fo.close();
	} else {
		std::cout << "Unable to open input or output file :c\n";
	}
	return 0;
}
