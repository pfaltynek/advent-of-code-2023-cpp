#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

enum direction_t { E_DIRECTION_NORTH, E_DIRECTION_SOUTH, E_DIRECTION_WEST, E_DIRECTION_EAST };

struct crucible_str {
	coord_str coord;
	direction_t direction;
	uint8_t steps;
	uint64_t heat_loss;

	void turn_left() {
		switch (direction) {
			case E_DIRECTION_NORTH:
				coord = coord + coord_step_west;
				direction = E_DIRECTION_WEST;
				break;
			case E_DIRECTION_SOUTH:
				coord = coord + coord_step_east;
				direction = E_DIRECTION_EAST;
				break;
			case E_DIRECTION_EAST:
				coord = coord + coord_step_north;
				direction = E_DIRECTION_NORTH;
				break;
			case E_DIRECTION_WEST:
				coord = coord + coord_step_south;
				direction = E_DIRECTION_SOUTH;
				break;
		}

		steps = 1;
	}

	void turn_right() {
		switch (direction) {
			case E_DIRECTION_SOUTH:
				coord = coord + coord_step_west;
				direction = E_DIRECTION_WEST;
				break;
			case E_DIRECTION_NORTH:
				coord = coord + coord_step_east;
				direction = E_DIRECTION_EAST;
				break;
			case E_DIRECTION_WEST:
				coord = coord + coord_step_north;
				direction = E_DIRECTION_NORTH;
				break;
			case E_DIRECTION_EAST:
				coord = coord + coord_step_south;
				direction = E_DIRECTION_SOUTH;
				break;
		}

		steps = 1;
	}

	bool go_ahead() {
		if (steps == 3) {
			return false;
		} else {
			switch (direction) {
				case E_DIRECTION_WEST:
					coord = coord + coord_step_west;
					break;
				case E_DIRECTION_NORTH:
					coord = coord + coord_step_north;
					break;
				case E_DIRECTION_SOUTH:
					coord = coord + coord_step_south;
					break;
				case E_DIRECTION_EAST:
					coord = coord + coord_step_east;
					break;
			}

			steps++;
		}

		return true;
	}

	bool go_ahead2() {
		if (steps == 10) {
			return false;
		} else {
			switch (direction) {
				case E_DIRECTION_WEST:
					coord = coord + coord_step_west;
					break;
				case E_DIRECTION_NORTH:
					coord = coord + coord_step_north;
					break;
				case E_DIRECTION_SOUTH:
					coord = coord + coord_step_south;
					break;
				case E_DIRECTION_EAST:
					coord = coord + coord_step_east;
					break;
			}

			steps++;
		}

		return true;
	}
};

class AoC2023_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	coord_str finish_;
	std::map<coord_str, uint8_t> map_;
	uint64_t get_least_heat_lost(const bool part2);
};

bool AoC2023_day17::init(const std::vector<std::string> lines) {
	size_t size;
	map_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("123456789") != std::string::npos) {
			std::cout << "Invalid map content at line " << i + 1 << std::endl;
			return false;
		}

		if (i == 0) {
			size = lines[i].size();
		} else {
			if (size != lines[i].size()) {
				std::cout << "Invalid map size at line " << i + 1 << std::endl;
				return false;
			}
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			map_[{j, i}] = lines[i][j] - '0';
		}
	}

	finish_ = {static_cast<uint32_t>(size - 1), static_cast<uint32_t>(lines.size() - 1)};

	return true;
}

uint64_t AoC2023_day17::get_least_heat_lost(const bool part2 = false) {
	std::queue<crucible_str> q;
	std::map<coord_str, std::map<direction_t, std::map<uint8_t, uint64_t>>> h;
	std::vector<crucible_str> tmp;
	crucible_str cr, next;
	uint64_t result = UINT64_MAX;

	h.clear();
	q = {};

	cr.coord = {0, 0};
	cr.direction = E_DIRECTION_EAST;
	cr.heat_loss = 0;
	cr.steps = 0;

	q.push(cr);

	while (!q.empty()) {
		cr = q.front();
		q.pop();

		tmp.clear();

		if (part2) {
			next = cr;
			if (next.go_ahead2()) {
				tmp.push_back(next);
			}

			if (cr.steps >= 4) {
				next = cr;
				next.turn_left();
				tmp.push_back(next);
				next = cr;
				next.turn_right();
				tmp.push_back(next);
			}
		} else {
			next = cr;
			if (next.go_ahead()) {
				tmp.push_back(next);
			}

			next = cr;
			next.turn_left();
			tmp.push_back(next);
			next = cr;
			next.turn_right();
			tmp.push_back(next);
		}

		for (auto& crucible : tmp) {
			if (map_.count(crucible.coord)) {
				crucible.heat_loss += map_[crucible.coord];

				if (crucible.coord == finish_) {
					if (crucible.heat_loss < result) {
						if ((part2) && (crucible.steps < 4)) {
							continue;
						}
						result = crucible.heat_loss;
					}
				} else {
					if (h.count(crucible.coord)) {
						if (h[crucible.coord].count(crucible.direction)) {
							if (h[crucible.coord][crucible.direction].count(crucible.steps)) {
								if (h[crucible.coord][crucible.direction][crucible.steps] > crucible.heat_loss) {
									h[crucible.coord][crucible.direction][crucible.steps] = crucible.heat_loss;
									q.push(crucible);
								}
							} else {
								h[crucible.coord][crucible.direction][crucible.steps] = crucible.heat_loss;
								q.push(crucible);
							}
						} else {
							h[crucible.coord][crucible.direction][crucible.steps] = crucible.heat_loss;
							q.push(crucible);
						}
					} else {
						h[crucible.coord][crucible.direction][crucible.steps] = crucible.heat_loss;
						q.push(crucible);
					}
				}
			}
		}
	}

	return result;
}

int32_t AoC2023_day17::get_aoc_day() {
	return 17;
}

int32_t AoC2023_day17::get_aoc_year() {
	return 2023;
}

void AoC2023_day17::tests() {
	uint64_t result;

	if (init({"2413432311323", "3215453535623", "3255245654254", "3446585845452", "4546657867536", "1438598798454", "4457876987766", "3637877979653",
			  "4654967986887", "4564679986453", "1224686865563", "2546548887735", "4322674655533"})) {
		result = get_least_heat_lost();		// 102
		result = get_least_heat_lost(true); // 94
	}

	if (init({"111111111111", "999999999991", "999999999991", "999999999991", "999999999991"})) {
		result = get_least_heat_lost(true); // 71
	}
}

bool AoC2023_day17::part1() {
	int64_t result = 0;

	result = get_least_heat_lost();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day17::part2() {
	int64_t result = 0;

	result = get_least_heat_lost(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day17 day17;

	return day17.main_execution();
}
