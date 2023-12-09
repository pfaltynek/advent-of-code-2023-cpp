#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <sstream>

class AoC2023_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<coord_str> races_;
	int64_t get_highest_races_score(const bool part2);
	int64_t get_higher_race_score_count(const int64_t time, const int64_t distance);
};

bool AoC2023_day06::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	coord_str param;
	std::string title;
	size_t idx;

	if (lines.size() < 2) {
		std::cout << "Incomplete input" << std::endl;
		return false;
	}

	races_.clear();

	ss.str(lines[0]);
	ss >> title;
	if (title != "Time:") {
		std::cout << "Times not found" << std::endl;
		return false;
	}

	while (!ss.eof()) {
		param.x = param.y = 0;
		ss >> param.x;
		races_.push_back(param);
	}

	ss.clear();
	ss.str(lines[1]);
	ss >> title;
	if (title != "Distance:") {
		std::cout << "Distancess not found" << std::endl;
		return false;
	}

	idx = 0;

	while (!ss.eof()) {
		ss >> param.y;

		if (idx < races_.size()) {
			races_[idx].y = param.y;
			idx++;
		} else {
			std::cout << "Too much distances" << std::endl;
			return false;
		}
	}

	if (idx < races_.size()) {
		std::cout << "Too few distances" << std::endl;
		return false;
	}

	return true;
}

int64_t AoC2023_day06::get_higher_race_score_count(const int64_t time, const int64_t distance) {
	int64_t result = 0;
	__int128_t dist, t128;

	t128 = time;
	for (__int128_t i = 0; i <= t128; i++) {
		dist = (t128 - i) * i;

		if (dist > distance) {
			result++;
		}
	}

	return result;
}

int64_t AoC2023_day06::get_highest_races_score(const bool part2 = false) {
	uint64_t result = 1;
	if (part2) {
		std::string num1 = "", num2 = "";
		int64_t time, dist;

		for (size_t i = 0; i < races_.size(); i++) {
			num1 += std::to_string(races_[i].x);
			num2 += std::to_string(races_[i].y);
		}
		time = std::stoll(num1);
		dist = std::stoll(num2);

		result = get_higher_race_score_count(time, dist);
	} else {
		for (size_t i = 0; i < races_.size(); i++) {
			result *= get_higher_race_score_count(races_[i].x, races_[i].y);
		}
	}

	return result;
}

int32_t AoC2023_day06::get_aoc_day() {
	return 6;
}

int32_t AoC2023_day06::get_aoc_year() {
	return 2023;
}

void AoC2023_day06::tests() {
	uint64_t result;

	if (init({"Time:      7  15   30", "Distance:  9  40  200"})) {
		result = get_highest_races_score();		// 288
		result = get_highest_races_score(true); // 71503
	}
}

bool AoC2023_day06::part1() {
	int64_t result = 0;

	result = get_highest_races_score();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day06::part2() {
	int64_t result = 0;

	result = get_highest_races_score(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day06 day06;

	return day06.main_execution();
}
