#ifndef CARDS_DECK_HPP
#define CARDS_DECK_HPP

#include <string>
#include <ctime>
#include <random>


static auto RandomEngine = std::default_random_engine(time(0));

enum class Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
static const std::string RankStr[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

enum class Suit { Hearts, Diamonds, Clubs, Spades };
static const std::string SuitStr[] = { "H", "D", "C", "S" };

#endif
