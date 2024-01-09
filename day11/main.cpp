#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

class AoC2023_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> galaxies_;
	std::map<int32_t, std::vector<int32_t>> galaxies_rows_, galaxies_cols_;
	coord_str min_, max_;
	int64_t get_galaxies_distances_sum(const int32_t grow);
	void update_limits(coord_str coord);
	std::vector<coord_str> get_time_expanded_universe(const int32_t grow);
};

bool AoC2023_day11::init(const std::vector<std::string> lines) {
	size_t size;
	galaxies_.clear();
	galaxies_cols_.clear();
	galaxies_rows_.clear();

	min_ = max_ = {0, 0};

	for (uint32_t y = 0; y < lines.size(); y++) {
		if (y == 0) {
			size = lines[y].size();
		} else {
			if (lines[y].size() != size) {
				std::cout << "Invalid map size at line " << y + 1 << std::endl;
				return false;
			}
		}

		if (lines[y].find_first_not_of("#.") != std::string::npos) {
			std::cout << "Invalid map content at line " << y + 1 << std::endl;
			return false;
		}

		size_t pos = lines[y].find_first_of('#', 0);

		while (pos != std::string::npos) {
			uint32_t x = static_cast<uint32_t>(pos);
			galaxies_.emplace(coord_str(x, y));
			galaxies_rows_[y].push_back(x);
			galaxies_cols_[x].push_back(y);

			update_limits({x, y});

			pos = lines[y].find_first_of('#', pos + 1);
		}
	}

	return true;
}

void AoC2023_day11::update_limits(coord_str coord) {

	if (min_.x > coord.x) {
		min_.x = coord.x;
	}

	if (max_.x < coord.x) {
		max_.x = coord.x;
	}

	if (min_.y > coord.y) {
		min_.y = coord.y;
	}

	if (max_.y < coord.y) {
		max_.y = coord.y;
	}
}

std::vector<coord_str> AoC2023_day11::get_time_expanded_universe(const int32_t grow) {
	std::vector<coord_str> result;
	std::vector<int32_t> cols, rows;
	std::map<int32_t, int32_t> row_remap, col_remap;
	int32_t add;

	result.clear();
	row_remap.clear();
	col_remap.clear();

	add = 0;
	for (int32_t i = min_.x; i <= max_.x; i++) {
		if (galaxies_cols_.count(i) == 0) {
			add++;
		} else {
			col_remap[i] = i + (add * (grow - 1));
		}
	}

	add = 0;
	for (int32_t i = min_.y; i <= max_.y; i++) {
		if (galaxies_rows_.count(i) == 0) {
			add++;
		} else {
			row_remap[i] = i + (add * (grow - 1));
		}
	}

	for (auto& c : galaxies_) {
		result.push_back({col_remap[c.x], row_remap[c.y]});
	}

	return result;
}

int64_t AoC2023_day11::get_galaxies_distances_sum(const int32_t grow = 2) {
	int64_t result = 0, tmp;

	std::vector<coord_str> universe = get_time_expanded_universe(grow);

	for (size_t i = 0; i < universe.size() - 1; i++) {
		for (size_t j = i + 1; j < universe.size(); j++) {
			tmp = abs(universe[j].x - universe[i].x) + abs(universe[j].y - universe[i].y);
			result += tmp;
		}
	}

	return result;
}

int32_t AoC2023_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2023_day11::get_aoc_year() {
	return 2023;
}

void AoC2023_day11::tests() {
	uint64_t result;

	if (init({"...#......", ".......#..", "#.........", "..........", "......#...", ".#........", ".........#", "..........", ".......#..", "#...#....."})) {
		result = get_galaxies_distances_sum(2); // 374
		result = get_galaxies_distances_sum(10); // 1030
		result = get_galaxies_distances_sum(100); // 8410
	}
}

bool AoC2023_day11::part1() {
	int64_t result = 0;

	result = get_galaxies_distances_sum(2);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day11::part2() {
	int64_t result = 0;

	result = get_galaxies_distances_sum(1000000);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day11 day11;

	return day11.main_execution();
}
