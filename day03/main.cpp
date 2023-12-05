#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

class AoC2023_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_part_numbers_sum();
	uint64_t get_gear_ratios_sum();
	bool is_symbol_adjacent(const int32_t x1, const int32_t x2, const int32_t y, uint32_t& part_number);
	uint32_t get_part_number(const int32_t x1, const int32_t x2, const int32_t y);
	std::map<coord_str, char> map_;
	std::map<coord_str, std::set<uint32_t>> gears_;
	coord_str min_, max_;
};

bool AoC2023_day03::init(const std::vector<std::string> lines) {
	map_.clear();

	min_ = {0, 0};
	max_.y = lines.size();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i != 0) {
			if (static_cast<size_t>(max_.x) != lines[i].size()) {
				std::cout << "Invalid map width at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			max_.x = lines[i].size();
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			if (lines[i][j] != '.') {
				map_[coord_str(j, i)] = lines[i][j];
			}
		}
	}

	return true;
}

uint64_t AoC2023_day03::get_gear_ratios_sum() {
	uint64_t result = 0, ratio;

	for(auto& pair : gears_) {
		if (pair.second.size() == 2) {
			ratio = 1;
			for(auto& item : pair.second){
				ratio *= item;
			}

			result += ratio;
		}
	}

	return result;
}

uint32_t AoC2023_day03::get_part_number(const int32_t x1, const int32_t x2, const int32_t y) {
	std::string value = "";

	for (int32_t x = x1; x <= x2; x++) {
		value += map_[{x, y}];
	}

	return stoi(value);
}

bool AoC2023_day03::is_symbol_adjacent(const int32_t x1, const int32_t x2, const int32_t y, uint32_t& part_number) {
	coord_str c1, c2;
	bool result = false;

	part_number = get_part_number(x1, x2, y);

	for (int32_t x = x1 - 1; x <= x2 + 1; x++) {
		c1 = {x, y + 1};
		c2 = {x, y - 1};

		if (map_.count(c1)) {
			if (!isdigit(map_[c1])) {
				result = true;

				if (map_[c1] == '*') {
					gears_[c1].emplace(part_number);
				}
			}
		}

		if (map_.count(c2)) {
			if (!isdigit(map_[c2])) {
				result = true;

				if (map_[c2] == '*') {
					gears_[c2].emplace(part_number);
				}
			}
		}
	}

	c1 = {x1 - 1, y};
	c2 = {x2 + 1, y};

	if (map_.count(c1)) {
		if (!isdigit(map_[c1])) {
			result = true;

			if (map_[c1] == '*') {
				gears_[c1].emplace(part_number);
			}
		}
	}

	if (map_.count(c2)) {
		if (!isdigit(map_[c2])) {
			result = true;

			if (map_[c2] == '*') {
				gears_[c2].emplace(part_number);
			}
		}
	}

	return result;
}

uint64_t AoC2023_day03::get_part_numbers_sum() {
	uint64_t result = 0;
	bool in_number = false;
	coord_str start, end, coord;
	uint32_t part_number;

	gears_.clear();

	for (int32_t i = 0; i < max_.y; i++) {
		for (int32_t j = 0; j < max_.x; j++) {
			coord = {j, i};
			if (map_.count(coord)) {
				if (in_number) {
					if (!isdigit(map_[coord])) {
						in_number = false;

						if (is_symbol_adjacent(start.x, end.x, start.y, part_number)) {
							result += part_number;
						}
					} else {
						end = coord;
					}
				} else {
					if (isdigit(map_[coord])) {
						in_number = true;
						start = end = coord;
					}
				}
			} else {
				if (in_number) {
					in_number = false;

					if (is_symbol_adjacent(start.x, end.x, start.y, part_number)) {
						result += part_number;
					}
				}
			}
		}

		if (in_number) {
			in_number = false;

			if (is_symbol_adjacent(start.x, end.x, start.y, part_number)) {
				result += part_number;
			}
		}
	}

	return result;
}

int32_t AoC2023_day03::get_aoc_day() {
	return 3;
}

int32_t AoC2023_day03::get_aoc_year() {
	return 2023;
}

void AoC2023_day03::tests() {
	uint64_t result;

	if (init({"467..114..", "...*......", "..35..633.", "......#...", "617*......", ".....+.58.", "..592.....", "......755.", "...$.*....", ".664.598.."})) {
		result = get_part_numbers_sum(); // 4361
		result = get_gear_ratios_sum(); // 467835
	}
}

bool AoC2023_day03::part1() {
	int64_t result = 0;

	result = get_part_numbers_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day03::part2() {
	int64_t result = 0;

	result = get_gear_ratios_sum();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day03 day03;

	return day03.main_execution();
}
