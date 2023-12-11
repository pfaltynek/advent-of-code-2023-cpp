#include "./../common/aoc.hpp"
#include <algorithm>
#include <sstream>

const std::string C_CARDS_ORDER_PART1 = "23456789TJQKA";
const std::string C_CARDS_ORDER_PART2 = "J23456789TQKA";

enum hand_type_t { E_FIVE_OF_A_KIND = 6, E_FOUR_OF_A_KIND = 5, E_FULL_HOUSE = 4, E_THREE_OF_A_KIND = 3, E_TWO_PAIR = 2, E_ONE_PAIR = 1, E_HIGH_CARD = 0 };

struct hand_str {
	std::string hand, joker_hand;
	int32_t bid;
	hand_type_t hand_type, joker_hand_type;

	void clear() {
		hand = "";
		joker_hand = "";
		bid = 0;
		hand_type = hand_type_t::E_HIGH_CARD;
	}
};

bool hand_comparer_part1(const hand_str h1, const hand_str h2) {
	if (h1.hand_type != h2.hand_type) {
		return h1.hand_type < h2.hand_type;
	} else {
		size_t p1, p2;

		for (size_t i = 0; i < std::min(h1.hand.size(), h2.hand.size()); i++) {
			p1 = C_CARDS_ORDER_PART1.find(h1.hand[i]);
			p2 = C_CARDS_ORDER_PART1.find(h2.hand[i]);

			if (p1 == p2) {
				continue;
			} else {
				return p1 < p2;
			}
		}

		return false;
	}
}

bool hand_comparer_part2(const hand_str h1, const hand_str h2) {
	if (h1.joker_hand_type != h2.joker_hand_type) {
		return h1.joker_hand_type < h2.joker_hand_type;
	} else {
		size_t p1, p2;

		for (size_t i = 0; i < std::min(h1.hand.size(), h2.hand.size()); i++) {
			p1 = C_CARDS_ORDER_PART2.find(h1.hand[i]);
			p2 = C_CARDS_ORDER_PART2.find(h2.hand[i]);

			if (p1 == p2) {
				continue;
			} else {
				return p1 < p2;
			}
		}

		return false;
	}
}

class AoC2023_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<hand_str> hands_;
	bool get_best_hand_joker(const std::string hand, hand_type_t& joker_hand_type, std::string& joker_hand);
	int64_t get_total_winnings(const bool part2 = false);
	hand_type_t get_hand_type(const std::string cards);
};

bool AoC2023_day07::get_best_hand_joker(const std::string hand, hand_type_t& joker_hand_type, std::string& joker_hand){
	std::string cards = "", tmp;
	uint8_t jokers = 0;
	hand_type_t tmp_ht;

	for (size_t i = 0; i < hand.size(); i++)
	{
		if (hand[i] == 'J') {
			jokers++;
		} else {
			cards += hand[i];
		}
	}
	
	if (jokers == 0) {
		return false;
	} else if (jokers == 5) {
		joker_hand = "AAAAA";
		joker_hand_type = hand_type_t::E_FIVE_OF_A_KIND;

		return true;
	}

	for (size_t i = 0; i < cards.size(); i++)
	{
		tmp.clear();

		for (size_t j = 0; j < jokers; j++)
		{
			tmp += cards[i];
		}

		tmp += cards;

		tmp_ht = get_hand_type(tmp);

		if (tmp_ht > joker_hand_type) {
			joker_hand = tmp;
			joker_hand_type = tmp_ht;
		}
	}

	return true;
}

bool AoC2023_day07::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	hand_str hand;

	hands_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		hand.clear();

		ss >> hand.hand;
		ss >> hand.bid;

		if (hand.hand.size() != 5) {
			std::cout << "Invalid hand size at line " << i + 1 << std::endl;
			return false;
		}
		if (hand.hand.find_first_not_of(C_CARDS_ORDER_PART1) != std::string::npos) {
			std::cout << "Invalid card at line " << i + 1 << std::endl;
			return false;
		}

		hand.hand_type = get_hand_type(hand.hand);
		hand.joker_hand_type = hand.hand_type;
		hand.joker_hand = hand.hand;

		get_best_hand_joker(hand.hand, hand.joker_hand_type, hand.joker_hand);

		hands_.push_back(hand);
	}

	return true;
}

hand_type_t AoC2023_day07::get_hand_type(const std::string cards) {
	std::string tmp = cards;
	char a, b, c, d, e;

	std::sort(tmp.begin(), tmp.end());

	a = tmp[0];
	b = tmp[1];
	c = tmp[2];
	d = tmp[3];
	e = tmp[4];

	if ((a == b) && (b == c) && (c == d) && (d == e)) {
		return hand_type_t::E_FIVE_OF_A_KIND;
	} else if ((b == c) && (c == d) && ((a == b) || (d == e))) {
		return hand_type_t::E_FOUR_OF_A_KIND;
	} else if (((a == b) && (b == c) && (d == e)) || ((a == b) && (c == d) && (d == e))) {
		return hand_type_t::E_FULL_HOUSE;
	} else if (((a == b) && (b == c)) || ((b == c) && (c == d)) || ((c == d) && (d == e))) {
		return hand_type_t::E_THREE_OF_A_KIND;
	} else if (((a == b) && (c == d)) || ((a == b) && (d == e)) || ((b == c) && (d == e))) {
		return hand_type_t::E_TWO_PAIR;
	} else if ((a == b) || (b == c) || (c == d) || (d == e)) {
		return hand_type_t::E_ONE_PAIR;
	} else {
		return hand_type_t::E_HIGH_CARD;
	}
}

int32_t AoC2023_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2023_day07::get_aoc_year() {
	return 2023;
}

void AoC2023_day07::tests() {
	uint64_t result;

	if (init({"32T3K 765", "T55J5 684", "KK677 28", "KTJJT 220", "QQQJA 483"})) {
		result = get_total_winnings(); // 6440
		result = get_total_winnings(true); // 5905
	}
}

int64_t AoC2023_day07::get_total_winnings(const bool part2) {
	int64_t result = 0;
	std::vector<hand_str> hands;

	hands.clear();
	hands = hands_;

	if (part2) {
		std::sort(hands.begin(), hands.end(), hand_comparer_part2);
	} else {
		std::sort(hands.begin(), hands.end(), hand_comparer_part1);
	}

	for (size_t i = 0; i < hands.size(); i++) {
		result += hands[i].bid * (i + 1);
	}

	return result;
}

bool AoC2023_day07::part1() {
	int64_t result = 0;

	result = get_total_winnings();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day07::part2() {
	int64_t result = 0;

	result = get_total_winnings(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day07 day07;

	return day07.main_execution();
}
