#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

const std::string C_MAP_CHARS = "SLJ7F-|.";

enum direction_t { E_DIRECTION_NORTH, E_DIRECTION_EAST, E_DIRECTION_SOUTH, E_DIRECTION_WEST };

struct position_str {
	coord_str coord;
	direction_t direction;
};

class AoC2023_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, char> map_;
	std::set<coord_str> loop_;
	coord_str start_, min_, max_;
	char start_original_;
	char get_start_original(const coord_str coord1, const coord_str coord2);
	void update_loop_limits(const coord_str coord);
	std::vector<std::vector<int32_t>> history_;
	int64_t get_farthest_position_distance();
	int64_t get_count_of_tiles_enclosed_by_loop();
	position_str get_next_pipe_part(const position_str current);
};

bool AoC2023_day10::init(const std::vector<std::string> lines) {
	coord_str coord;
	bool start_found = false;
	size_t size;

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i == 0) {
			size = lines[i].size();
		} else if (size != lines[i].size()) {
			std::cout << "Invalid map size at line " << i + 1 << std::endl;
			return false;
		}

		if (lines[i].find_first_not_of(C_MAP_CHARS) != std::string::npos) {
			std::cout << "Invalid map character at line " << i + 1 << std::endl;
			return false;
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			coord = {j, i};

			switch (lines[i][j]) {
				case 'S':
					start_ = coord;
					start_found = true;
					map_[coord] = lines[i][j];
					break;
				case '.':
					// skip it
					break;
				default:
					map_[coord] = lines[i][j];
					break;
			}
		}
	}

	return start_found;
}

void AoC2023_day10::update_loop_limits(const coord_str coord) {
	if (min_.x > coord.x) {
		min_.x = coord.x;
	}
	if (min_.y > coord.y) {
		min_.y = coord.y;
	}
	if (max_.x < coord.x) {
		max_.x = coord.x;
	}
	if (max_.y < coord.y) {
		max_.y = coord.y;
	}
}

char AoC2023_day10::get_start_original(coord_str coord1, coord_str coord2) {
	if ((coord1 - start_) == coord_step_north) {
		if ((coord2 - start_) == coord_step_south) {
			return '|';
		} else if ((coord2 - start_) == coord_step_east) {
			return 'L';
		} else if ((coord2 - start_) == coord_step_west) {
			return 'J';
		}
	} else if ((coord1 - start_) == coord_step_south) {
		if ((coord2 - start_) == coord_step_north) {
			return '|';
		} else if ((coord2 - start_) == coord_step_east) {
			return 'F';
		} else if ((coord2 - start_) == coord_step_west) {
			return '7';
		}
	} else if ((coord1 - start_) == coord_step_east) {
		if ((coord2 - start_) == coord_step_south) {
			return 'F';
		} else if ((coord2 - start_) == coord_step_north) {
			return 'L';
		} else if ((coord2 - start_) == coord_step_west) {
			return '-';
		}
	} else if ((coord1 - start_) == coord_step_west) {
		if ((coord2 - start_) == coord_step_south) {
			return '7';
		} else if ((coord2 - start_) == coord_step_east) {
			return '-';
		} else if ((coord2 - start_) == coord_step_north) {
			return 'J';
		}
	}

	return '.';
}

position_str AoC2023_day10::get_next_pipe_part(const position_str current) {
	position_str pos;

	pos.coord = current.coord;
	update_loop_limits(pos.coord);

	switch (map_[current.coord]) {
		case 'J':
			if (current.direction == E_DIRECTION_EAST) {
				pos.direction = E_DIRECTION_NORTH;
				pos.coord = pos.coord + coord_step_north;

				return pos;
			} else if (current.direction == E_DIRECTION_SOUTH) {
				pos.direction = E_DIRECTION_WEST;
				pos.coord = pos.coord + coord_step_west;

				return pos;
			}
			break;
		case 'F':
			if (current.direction == E_DIRECTION_WEST) {
				pos.direction = E_DIRECTION_SOUTH;
				pos.coord = pos.coord + coord_step_south;

				return pos;
			} else if (current.direction == E_DIRECTION_NORTH) {
				pos.direction = E_DIRECTION_EAST;
				pos.coord = pos.coord + coord_step_east;

				return pos;
			}
			break;
		case 'L':
			if (current.direction == E_DIRECTION_WEST) {
				pos.direction = E_DIRECTION_NORTH;
				pos.coord = pos.coord + coord_step_north;

				return pos;
			} else if (current.direction == E_DIRECTION_SOUTH) {
				pos.direction = E_DIRECTION_EAST;
				pos.coord = pos.coord + coord_step_east;

				return pos;
			}
			break;
		case '7':
			if (current.direction == E_DIRECTION_EAST) {
				pos.direction = E_DIRECTION_SOUTH;
				pos.coord = pos.coord + coord_step_south;

				return pos;
			} else if (current.direction == E_DIRECTION_NORTH) {
				pos.direction = E_DIRECTION_WEST;
				pos.coord = pos.coord + coord_step_west;

				return pos;
			}
			break;
		case '|':
			if (current.direction == E_DIRECTION_NORTH) {
				pos.direction = E_DIRECTION_NORTH;
				pos.coord = pos.coord + coord_step_north;

				return pos;
			} else if (current.direction == E_DIRECTION_SOUTH) {
				pos.direction = E_DIRECTION_SOUTH;
				pos.coord = pos.coord + coord_step_south;

				return pos;
			}
			break;
		case '-':
			if (current.direction == E_DIRECTION_EAST) {
				pos.direction = E_DIRECTION_EAST;
				pos.coord = pos.coord + coord_step_east;

				return pos;
			} else if (current.direction == E_DIRECTION_WEST) {
				pos.direction = E_DIRECTION_WEST;
				pos.coord = pos.coord + coord_step_west;

				return pos;
			}
			break;
	}

	update_loop_limits(current.coord);

	return current;
}

int64_t AoC2023_day10::get_farthest_position_distance() {
	int64_t result = 0;
	std::vector<position_str> branches = {};
	position_str pos;
	char c;

	min_ = max_ = {0, 0};
	loop_.clear();
	loop_.emplace(start_);

	pos.coord = start_ + coord_step_north;
	if (map_.count(pos.coord)) {
		c = map_[pos.coord];
		if ((c == '|') || (c == 'F') || (c == '7')) {
			pos.direction = E_DIRECTION_NORTH;
			branches.push_back(pos);
		}
	}

	pos.coord = start_ + coord_step_south;
	if (map_.count(pos.coord)) {
		c = map_[pos.coord];
		if ((c == '|') || (c == 'J') || (c == 'L')) {
			pos.direction = E_DIRECTION_SOUTH;
			branches.push_back(pos);
		}
	}

	pos.coord = start_ + coord_step_east;
	if (map_.count(pos.coord)) {
		c = map_[pos.coord];
		if ((c == '-') || (c == 'J') || (c == '7')) {
			pos.direction = E_DIRECTION_EAST;
			branches.push_back(pos);
		}
	}

	pos.coord = start_ + coord_step_west;
	if (map_.count(pos.coord)) {
		c = map_[pos.coord];
		if ((c == '-') || (c == 'F') || (c == 'L')) {
			pos.direction = E_DIRECTION_WEST;
			branches.push_back(pos);
		}
	}

	if (branches.size() == 2) {
		result = 1;

		start_original_ = get_start_original(branches[0].coord, branches[1].coord);

		while (!(branches[0].coord == branches[1].coord)) {
			loop_.emplace(branches[0].coord);
			loop_.emplace(branches[1].coord);
			branches[0] = get_next_pipe_part(branches[0]);
			branches[1] = get_next_pipe_part(branches[1]);
			loop_.emplace(branches[0].coord);
			loop_.emplace(branches[1].coord);
			result++;
		}
	}

	return result;
}

int64_t AoC2023_day10::get_count_of_tiles_enclosed_by_loop() {
	int64_t result = 0;

	for (int32_t y = min_.y; y <= max_.y; y++) {
		bool inloop = false;
		bool wasL = false, wasF = false;
		char c;

		for (int32_t x = min_.x; x <= max_.x; x++) {
			coord_str coord = {x, y};

			if (loop_.count(coord)) {
				if (coord == start_) {
					c = start_original_;
				} else {
					c = map_[coord];
				}

				switch (c) {
					default:
					case '-':
						break;

					case '|':
						inloop = !inloop;
						break;

					case 'L':
						wasL = true;
						break;

					case 'J':
						if (wasF) {
							inloop = !inloop;
							wasF = false;
						}
						wasL = false;
						break;

					case '7':
						if (wasL) {
							inloop = !inloop;
							wasL = false;
						}
						wasF = false;
						break;

					case 'F':
						wasF = true;
						break;
				}
			} else {
				if (inloop) {
					result++;
				}
			}
		}
	}

	return result;
}

int32_t AoC2023_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2023_day10::get_aoc_year() {
	return 2023;
}

void AoC2023_day10::tests() {
	uint64_t result;

	if (init({".....", ".S-7.", ".|.|.", ".L-J.", "....."})) {
		result = get_farthest_position_distance(); // 4
	}

	if (init({"..F7.", ".FJ|.", "SJ.L7", "|F--J", "LJ..."})) {
		result = get_farthest_position_distance(); // 8
	}

	if (init({"...........", ".S-------7.", ".|F-----7|.", ".||.....||.", ".||.....||.", ".|L-7.F-J|.", ".|..|.|..|.", ".L--J.L--J.", "..........."})) {
		get_farthest_position_distance();
		result = get_count_of_tiles_enclosed_by_loop(); // 4
	}

	if (init({".F----7F7F7F7F-7....", ".|F--7||||||||FJ....", ".||.FJ||||||||L7....", "FJL7L7LJLJ||LJ.L-7..", "L--J.L7...LJS7F-7L7.", "....F-J..F7FJ|L7L7L7",
			  "....L7.F7||L7|.L7L7|", ".....|FJLJ|FJ|F7|.LJ", "....FJL-7.||.||||...", "....L---J.LJ.LJLJ..."})) {
		get_farthest_position_distance();
		result = get_count_of_tiles_enclosed_by_loop(); // 8
	}

	if (init({"FF7FSF7F7F7F7F7F---7", "L|LJ||||||||||||F--J", "FL-7LJLJ||||||LJL-77", "F--JF--7||LJLJ7F7FJ-", "L---JF-JLJ.||-FJLJJ7", "|F|F-JF---7F7-L7L|7|",
			  "|FFJF7L7F-JF7|JL---7", "7-L-JL7||F7|L7F-7F7|", "L.L7LFJ|||||FJL7||LJ", "L7JLJL-JLJLJL--JLJ.L"})) {
		get_farthest_position_distance();
		result = get_count_of_tiles_enclosed_by_loop(); // 10
	}
}

bool AoC2023_day10::part1() {
	int64_t result = 0;

	result = get_farthest_position_distance();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day10::part2() {
	int64_t result = 0;

	result = get_count_of_tiles_enclosed_by_loop();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day10 day10;

	return day10.main_execution();
}
