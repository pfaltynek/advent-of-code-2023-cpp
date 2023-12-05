#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

struct card_str {
	uint16_t number;
	std::set<uint8_t> having;
	std::set<uint8_t> winning;
	uint16_t winners;

	uint32_t get_card_worth() {
		uint32_t result = 0;

		winners = 0;

		for (auto& item : winning) {
			if (having.count(item)) {

				winners++;

				if (!result) {
					result = 1;
				} else {
					result *= 2;
				}
			}
		}

		return result;
	}
};

class AoC2023_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<uint16_t, card_str> cards_;
	uint64_t get_total_cards_worth();
	uint64_t get_total_scratchcards_count();
	uint16_t min_, max_;
};

bool AoC2023_day04::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	card_str card;
	uint16_t tmp;

	cards_.clear();

	min_ = max_ = 1;

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		card.having.clear();
		card.winning.clear();

		ss.ignore(5); // "Card "
		ss >> card.number;
		ss.ignore(1); // ":"
		while (true) {
			while (ss.peek() == ' ') {
				ss.ignore(1);
			}
			if (ss.peek() == '|') {
				break;
			}
			ss >> tmp;
			card.winning.emplace(tmp);
		}

		ss.ignore(1);

		while (!ss.eof()) {
			while (ss.peek() == ' ') {
				ss.ignore(1);
			}
			ss >> tmp;
			card.having.emplace(tmp);
		}

		cards_[card.number] = card;

		if (card.number < min_) {
			min_ = card.number;
		}

		if (card.number > max_) {
			max_ = card.number;
		}
	}

	return true;
}

uint64_t AoC2023_day04::get_total_cards_worth() {
	uint64_t result = 0;

	for (auto& pair : cards_) {
		result += pair.second.get_card_worth();
	}

	return result;
}

uint64_t AoC2023_day04::get_total_scratchcards_count() {
	uint16_t count;
	uint64_t diff;
	std::map<uint16_t, uint64_t> counts;
	uint64_t result = 0;
	counts.clear();

	for (size_t i = min_; i <= max_; i++) {
		counts[i] = 1;
	}

	for (size_t id = min_; id <= max_; id++) {
		count = cards_[id].winners;
		diff = counts[id];

		for (size_t i = id + 1; i < id + 1 + count; i++) {
			if (i <= max_) {
				counts[i] += diff;
			}
		}
	}

	for (auto& pair : counts) {
		result += pair.second;
	}

	return result;
}

int32_t AoC2023_day04::get_aoc_day() {
	return 4;
}

int32_t AoC2023_day04::get_aoc_year() {
	return 2023;
}

void AoC2023_day04::tests() {
	uint64_t result;

	if (init({"Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53", "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
			  "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1", "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
			  "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36", "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"})) {
		result = get_total_cards_worth();		 // 13
		result = get_total_scratchcards_count(); // 30
	}
}

bool AoC2023_day04::part1() {
	int64_t result = 0;

	result = get_total_cards_worth();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day04::part2() {
	int64_t result = 0;

	result = get_total_scratchcards_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day04 day04;

	return day04.main_execution();
}
