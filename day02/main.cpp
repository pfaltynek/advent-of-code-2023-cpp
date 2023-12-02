#include "./../common/aoc.hpp"
#include <map>
#include <set>
#include <sstream>

struct round_str {
	uint16_t red, green, blue;

	void clear() {
		red = green = blue = 0;
	}
};

const round_str C_BAG_CONTENT = {12, 13, 14};

class AoC2023_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_possible_games_ids_sum();
	uint64_t get_game_powers_sum();
	std::map<uint16_t, std::vector<round_str>> games_;
};

bool AoC2023_day02::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	uint16_t id, count;
	std::string color;
	char delimiter;
	std::vector<round_str> game;
	round_str round;
	games_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.str(lines[i]);
		game.clear();
		round.clear();

		ss.ignore(5); //"Game "
		ss >> id;

		do {
			ss.ignore(1); // " "
			ss >> count;
			ss.ignore(1); // " "
			ss >> color;
			delimiter = color.back();

			if ((delimiter == ',') || (delimiter == ';')) {
				color.pop_back();
			} else {
				delimiter = 0;
			}

			if (color == "red") {
				round.red = count;
			} else if (color == "green") {
				round.green = count;
			} else if (color == "blue") {
				round.blue = count;
			} else {
				std::cout << "Game parsing failed on line " << i + 1 << std::endl;
				return false;
			}

			if ((delimiter == ';') || (delimiter == 0)) {
				game.push_back(round);
				round.clear();
			}
		} while (!ss.eof());

		games_[id] = game;
		ss.clear();
	}

	return true;
}

uint64_t AoC2023_day02::get_possible_games_ids_sum() {
	uint64_t result = 0;
	bool possible;

	for (auto& game : games_) {
		possible = true;

		for (auto& round : game.second) {
			if ((round.red > C_BAG_CONTENT.red) || (round.green > C_BAG_CONTENT.green) || (round.blue > C_BAG_CONTENT.blue)) {
				possible = false;
				break;
			}
		}

		if (possible) {
			result += game.first;
		}
	}

	return result;
}

uint64_t AoC2023_day02::get_game_powers_sum() {
	uint64_t result = 0;
	round_str power;

	for (auto& game : games_) {
		power.clear();
		for (auto& round : game.second) {
			if (power.red < round.red) {
				power.red = round.red;
			}
			if (power.green < round.green) {
				power.green = round.green;
			}
			if (power.blue < round.blue) {
				power.blue = round.blue;
			}
		}

		result += (power.blue * power.green * power.red);
	}

	return result;
}

int32_t AoC2023_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2023_day02::get_aoc_year() {
	return 2023;
}

void AoC2023_day02::tests() {
	uint64_t result;

	if (init({"Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green", "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue",
			  "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red",
			  "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red", "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"})) {
		result = get_possible_games_ids_sum(); // 142
		result = get_game_powers_sum();		   // 2286
	}
}

bool AoC2023_day02::part1() {
	int64_t result = 0;

	result = get_possible_games_ids_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day02::part2() {
	int64_t result = 0;

	result = get_game_powers_sum();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day02 day02;

	return day02.main_execution();
}
