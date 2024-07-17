#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <algorithm>
#include <set>

/* part2 solution explanation from related reddit thread:

Part two can in principle be solved similarly, though now we are looking for positions reached in an odd number of steps (as 26501365 is odd).
The periodicity of the grid/map can be emulated simply by grid[i % size][j % size], where size = 131 is the width and height of the grid.
In practice though, the number of steps is too high for this. But, unlike the example input, the full input is rigged. If you visualize the
entire map (open in text editor and make the font size very small), you will notice a big diamond shape of free path.
It turns out that the entire perimeter of this diamond is exactly reached after size//2 = 65 steps. Because the corner of the diamond are at
the boundary of the map (when thought of as non-periodic), and the middle row and column (where the starting position S is located)
are completely free (no rocks #), we are then guaranteed that another 8 surrounding diamonds will be exactly reached after size = 131 steps
(in addition to the first size//2 = 65 steps). After another size = 131 steps, the next layer of diamonds are exactly reached, and so on.
If we were in the continuous limit and with no rocks #, the number of positions covered as a function of steps would be A(t) = πt² (area of disk),
where t is the number of steps. Having discrete steps and dismissing certain positions (adding in rocks) cannot introduce higher-order terms,
so the most general form will be A(t) = at² + bt + c. We can determine a, b and c if we know A(t) for three values of t.
Here we can use t = size//2 = 65, t = size//2 + size = 196 and t = size//2 + 2*size = 327, as argued above. One way of doing this in practice
(obtaining A(t) without actually ever finding a, b and c) is through the Lagrange interpolation polynomial. The final answer is then A(26501365).
Note that our formula A(t) is only valid for t of the form t = size//2 + n*size = 65 + n*131, which is exactly the form of the requested step number 26501365 =
65 + 202300*131).

All assumptions (e.g. presence of diamond) are explicitly tested at runtime. If some assumption is not satisfied, the fully general method
(as in part 1) is used. Thus correct results are obtained for both the full input and the many examples given for part 2.
*/

const int64_t C_STEPS_TEST = 6;
const int64_t C_STEPS_PART1 = 64;
const int64_t C_STEPS_PART2 = 26501365;

class AoC2023_day21 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> rocks_;
	coord_str start_, min_, max_;
	bool in_bounds(coord_str pos);
	int64_t get_garden_plots_total(const bool part2, const int64_t steps);
};

bool AoC2023_day21::init(const std::vector<std::string> lines) {
	std::size_t size;
	bool start_found = false;

	rocks_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i) {
			if (lines[i].size() != size) {
				std::cout << "Invalid map line size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			size = lines[i].size();
		}

		if (lines[i].find_first_not_of(".#S") != std::string::npos) {
			std::cout << "Invalid map symbol at line " << i + 1 << std::endl;
			return false;
		}

		for (size_t j = 0; j < lines[i].size(); j++) {
			switch (lines[i][j]) {
				case 'S':
					start_ = {static_cast<int32_t>(j), static_cast<int32_t>(i)};
					start_found = true;
					break;
				case '#':
					rocks_.emplace(coord_str(static_cast<int32_t>(j), static_cast<int32_t>(i)));
					break;
			}
		}
	}

	if (!start_found) {
		std::cout << "Start position not found" << std::endl;
		return false;
	}

	min_ = {0, 0};
	max_ = {static_cast<int32_t>(size - 1), static_cast<int32_t>(lines.size() - 1)};

	return true;
}

bool AoC2023_day21::in_bounds(coord_str pos) {
	if ((pos.x >= min_.x) && (pos.x <= max_.x) && (pos.y >= min_.y) && (pos.y <= max_.y)) {
		return true;
	} else {
		return false;
	}
}

int64_t AoC2023_day21::get_garden_plots_total(const bool part2, const int64_t steps) {
	std::set<coord_str> set1, set2;
	std::set<coord_str> directions = {coord_step_east, coord_step_west, coord_step_north, coord_step_south};
	coord_str coord, tmp, div;
	std::set<int32_t> part2_steps;
	int64_t p2steps;
	std::vector<coord_str> part2_counts;

	div = max_;
	div.x++;
	div.y++;

	if (div.x != div.y) {
		assert(false);
	}

	if (part2) {
		part2_steps.clear();
		p2steps = div.x / 2;
		part2_steps.emplace(p2steps);
		p2steps += div.x;
		part2_steps.emplace(p2steps);
		p2steps += div.x;
		part2_steps.emplace(p2steps);
		part2_counts.clear();
	}

	set1.emplace(start_);

	for (int64_t i = 0; i < steps; i++) {
		set2 = {};

		for (auto& c : set1) {
			for (auto& d : directions) {
				coord = c;
				coord = coord + d;

				if (part2) {
					tmp = {((coord.x % div.x) + div.x) % div.x, ((coord.y % div.y) + div.y) % div.y};
					if (rocks_.count(tmp) == 0) {
						set2.emplace(coord);
					}
				} else {
					if (in_bounds(coord)) {
						if (rocks_.count(coord) == 0) {
							set2.emplace(coord);
						}
					}
				}
			}
		}

		set1.swap(set2);

		if (part2 && part2_steps.count(i + 1)) {
			coord_str data;
			data.x = i + 1;
			data.y = set1.size();
			part2_counts.push_back(data);
			part2_steps.erase(i + 1);
		}
	}

	if (part2) {
		int64_t result, rp;

		if (part2_counts.size() != 3) {
			assert(false);
			return 0;
		}

		result = 0;
		for (size_t i = 0; i < 3; i++) {
			rp = part2_counts[i].y;

			for (size_t j = 0; j < 3; j++) {
				if (i != j) {
					rp *= (C_STEPS_PART2 - part2_counts[j].x);
					rp /= (part2_counts[i].x - part2_counts[j].x);
				}
			}

			result += rp;
		}

		return result;

	} else {
		return set1.size();
	}
}

int32_t AoC2023_day21::get_aoc_day() {
	return 21;
}

int32_t AoC2023_day21::get_aoc_year() {
	return 2023;
}

void AoC2023_day21::tests() {
	int64_t result;

	if (init({"...........", ".....###.#.", ".###.##..#.", "..#.#...#..", "....#.#....", ".##..S####.", ".##..#...#.", ".......##..", ".##.#.####.",
			  ".##..##.##.", "..........."})) {
		result = get_garden_plots_total(false, C_STEPS_TEST); // 16
	}
}

bool AoC2023_day21::part1() {
	int64_t result = 0;

	result = get_garden_plots_total(false, C_STEPS_PART1);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day21::part2() {
	int64_t result = 0;

	result = get_garden_plots_total(true, ((max_.x + 1) * 2) + ((max_.x + 1) / 2));

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day21 day21;

	return day21.main_execution();
}
