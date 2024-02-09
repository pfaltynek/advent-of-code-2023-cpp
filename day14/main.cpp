#include "./../common/aoc.hpp"
#include <algorithm>

const uint64_t C_CYCLES_PART2 = 1000000000U;

class AoC2023_day14 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> map_;
	void tilt_north(std::vector<std::string>& map);
	uint64_t get_total_load(const bool part2);
	uint64_t calculate_total_load(const std::vector<std::string>& map);
	std::vector<std::string> rotate_map_clockwise(const std::vector<std::string>& map);
};

bool AoC2023_day14::init(const std::vector<std::string> lines) {
	size_t size = 0;

	map_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (i == 0) {
			size = lines[i].size();
		} else {
			if (size != lines[i].size()) {
				std::cout << "Invalid line size at " << i + 1 << std::endl;
				return false;
			}
		}

		if (lines[i].find_first_not_of("#O.") != std::string::npos) {
			std::cout << "Invalid symbol at line " << i + 1 << std::endl;
			return false;
		}

		map_.push_back(lines[i]);
	}

	return true;
}

void AoC2023_day14::tilt_north(std::vector<std::string>& map) {
	size_t y = 0, base;
	bool search_base = true;

	for (size_t x = 0; x < map[0].size(); x++) {
		y = 0;
		search_base = true;

		while (y < map.size()) {
			if (search_base) {
				if (map[y][x] == '.') {
					search_base = false;
					base = y;
				}

				y++;
			} else {
				if (map[y][x] == 'O') {
					std::swap(map[y][x], map[base][x]);
					base++;
				} else if (map[y][x] == '#') {
					search_base = true;
					y++;
				} else {
					y++;
				}
			}
		}
	}
}

std::vector<std::string> AoC2023_day14::rotate_map_clockwise(const std::vector<std::string>& map) {
	std::vector<std::string> flipped;
	std::string tmp;

	flipped = {};

	for (size_t x = 0; x < map[0].size(); x++) {
		tmp.clear();

		for (size_t y = 0; y < map.size(); y++) {
			tmp = map[y][x] + tmp;
		}
		flipped.push_back(tmp);
	}

	return flipped;
}

uint64_t AoC2023_day14::calculate_total_load(const std::vector<std::string>& map) {
	uint64_t result = 0, count;

	for (size_t i = 0; i < map.size(); i++) {
		count = std::count(map[i].begin(), map[i].end(), 'O');
		result += count * (map.size() - i);
	}

	return result;
}

uint64_t AoC2023_day14::get_total_load(const bool part2 = false) {
	uint64_t result;
	size_t cycles = 0;
	std::vector<std::string> map = map_;

	if (part2) {
		std::vector<uint64_t> history = {};
		std::vector<uint64_t>::iterator it1, it2;
		size_t size;
		bool found = false;

		while (cycles < 500) {
			for (size_t i = 0; i < 4; i++) {
				tilt_north(map);
				map = rotate_map_clockwise(map);
			}
			result = calculate_total_load(map);
			history.push_back(result);
			cycles++;
		}

		it1 = history.begin() + 1;
		do {
			it2 = std::find(it1 + 2, history.end(), *it1);

			if (it2 == history.end()) {
				it1++;
			} else {
				size = it2 - it1;
				if (std::equal(it1, it1 + size, it2, it2 + size)) {
					if (std::equal(it1, it1 + size, it2 + size, it2 + (2 * size))) {
						found = true;
					} else {
						it1++;
					}
				} else {
					it1++;
				}
			}
		} while (!found && (it1 != history.end()));

		if (found) {
			uint64_t base = it1 - history.begin();
			uint64_t idx = (C_CYCLES_PART2 - base) % size;
			result = history[base + idx - 1];
		}
	} else {
		tilt_north(map);
		result = calculate_total_load(map);
	}

	return result;
}

int32_t AoC2023_day14::get_aoc_day() {
	return 14;
}

int32_t AoC2023_day14::get_aoc_year() {
	return 2023;
}

void AoC2023_day14::tests() {
	uint64_t result;

	if (init({"O....#....", "O.OO#....#", ".....##...", "OO.#O....O", ".O.....O#.", "O.#..O.#.#", "..O..#O..O", ".......O..", "#....###..", "#OO..#...."})) {
		result = get_total_load();	   // 136
		result = get_total_load(true); // 64
	}
}

bool AoC2023_day14::part1() {
	int64_t result = 0;

	result = get_total_load();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day14::part2() {
	int64_t result = 0;

	result = get_total_load(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day14 day14;

	return day14.main_execution();
}
