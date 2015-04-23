#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>

#define pb push_back

using namespace std;

// #include "Card.hpp"

enum class Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
static const string RankStr[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

enum class Suit { Hearts, Diamonds, Clubs, Spades };
static const string SuitStr[] = { "H", "D", "C", "S" };

class Card {
	Rank rank;
	Suit suit;

	Card() {}

public:
	Card(Rank rank, Suit suit) : rank(rank), suit(suit) {}

	string card2str() {
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
	vector<Card> deck;

public:
	Deck() {
		for (int i=0; i<(sizeof(RankStr)/sizeof(*RankStr)); ++i) {
			for (int j=0; j<(sizeof(SuitStr)/sizeof(*SuitStr)); ++j) {
				deck.pb(Card(static_cast<Rank>(i), static_cast<Suit>(j)));
			}
		}
	}

	void shuffle() {
		std::shuffle(begin(deck), end(deck), default_random_engine(time(0)));
	}

	vector<Card> getDeck() {
		return deck;
	}

	Card popCard() {
		Card back = deck.back();
		deck.pop_back();
		return back;
	}
};

int main() {
	ifstream fi("input.txt");
	ofstream fo("output.txt");

	if (fi && fo) {
		cout << "Jack of Spades: " << Card(Rank::Jack, Suit::Spades).card2str() << '\n';
		cout << "Seven Hearts: " << Card(Rank::Seven, Suit::Hearts).card2str() << '\n';

		Deck deck;

		for (Card card : deck.getDeck()) cout << card.card2str() << " ";
		cout << '\n';

		deck.shuffle();

		for (Card card : deck.getDeck()) cout << card.card2str() << " ";
		cout << '\n';

		cout << "Last card: " << deck.popCard().card2str() << '\n';

		fi.close();
		fo.close();
	} else {
		cout << "Unable to open input or output file :c\n";
	}
	return 0;
}
