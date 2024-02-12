#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <queue>
#include <set>

enum direction_t { E_DIRECTION_NORTH, E_DIRECTION_SOUTH, E_DIRECTION_WEST, E_DIRECTION_EAST };

struct beam_str {
	coord_str coord;
	direction_t direction;

	void turn(direction_t new_direction) {
		switch (new_direction) {
			case E_DIRECTION_NORTH:
				coord = coord + coord_step_north;
				break;
			case E_DIRECTION_SOUTH:
				coord = coord + coord_step_south;
				break;
			case E_DIRECTION_EAST:
				coord = coord + coord_step_east;
				break;
			case E_DIRECTION_WEST:
				coord = coord + coord_step_west;
				break;
		}

		direction = new_direction;
	}

	bool const operator<(const beam_str& other) const {
		if ((coord.x == other.coord.x) && (coord.y == other.coord.y)) {
			return direction < other.direction;
		} else {
			return coord < other.coord;
		}
	}
};

class AoC2023_day16 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> map_;
	uint64_t get_energized_tiles_sum(const beam_str beam_init);
	uint64_t get_energized_tiles_sum(const bool part2);
	bool is_in_map(const coord_str coord);
};

bool AoC2023_day16::init(const std::vector<std::string> lines) {
	size_t size;

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("|-./\\") != std::string::npos) {
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
	}

	map_ = lines;

	return true;
}

bool AoC2023_day16::is_in_map(const coord_str coord) {
	if ((coord.x < 0) || (coord.x >= static_cast<int32_t>(map_[0].size()))) {
		return false;
	}
	if ((coord.y < 0) || (coord.y >= static_cast<int32_t>(map_.size()))) {
		return false;
	}

	return true;
}

uint64_t AoC2023_day16::get_energized_tiles_sum(const bool part2 = false) {
	beam_str beam;
	uint64_t result = 0, subresult;

	if (part2) {
		std::vector<beam_str> beams = {};

		for (size_t i = 0; i < map_.size(); i++) {
			beam.coord.y = i;
			beam.coord.x = 0;
			beam.direction = E_DIRECTION_EAST;
			beams.push_back(beam);

			beam.coord.x = map_[0].size() - 1;
			beam.direction = E_DIRECTION_WEST;
			beams.push_back(beam);
		}

		for (size_t i = 0; i < map_[0].size(); i++) {
			beam.coord.x = i;
			beam.coord.y = 0;
			beam.direction = E_DIRECTION_SOUTH;
			beams.push_back(beam);

			beam.coord.y = map_.size() - 1;
			beam.direction = E_DIRECTION_NORTH;
			beams.push_back(beam);
		}

		for (auto& input : beams) {
			subresult = get_energized_tiles_sum(input);

			result = std::max(subresult, result);
		}

		return result;
	} else {
		beam.coord = {0, 0};
		beam.direction = E_DIRECTION_EAST;

		return get_energized_tiles_sum(beam);
	}
}

uint64_t AoC2023_day16::get_energized_tiles_sum(const beam_str beam_init) {
	std::set<coord_str> energized = {};
	std::set<beam_str> history = {};
	beam_str beam;
	std::queue<beam_str> q;
	char content;
	coord_str coord;

	q.emplace(beam_init);

	while (q.size()) {
		beam = q.front();
		q.pop();

		energized.emplace(beam.coord);

		content = map_[beam.coord.y][beam.coord.x];

		switch (content) {
			case '.':
				beam.turn(beam.direction);
				if (is_in_map(beam.coord)) {
					energized.emplace(beam.coord);
					if (history.count(beam) == 0) {
						q.emplace(beam);
						history.emplace(beam);
					}
				}
				break;
			case '\\':
				switch (beam.direction) {
					case E_DIRECTION_EAST:
						beam.turn(E_DIRECTION_SOUTH);
						break;
					case E_DIRECTION_WEST:
						beam.turn(E_DIRECTION_NORTH);
						break;
					case E_DIRECTION_NORTH:
						beam.turn(E_DIRECTION_WEST);
						break;
					case E_DIRECTION_SOUTH:
						beam.turn(E_DIRECTION_EAST);
						break;
					default:
						assert(false);
						break;
				}

				if (is_in_map(beam.coord)) {
					energized.emplace(beam.coord);
					if (history.count(beam) == 0) {
						q.emplace(beam);
						history.emplace(beam);
					}
				}
				break;
			case '/':
				switch (beam.direction) {
					case E_DIRECTION_EAST:
						beam.turn(E_DIRECTION_NORTH);
						break;
					case E_DIRECTION_WEST:
						beam.turn(E_DIRECTION_SOUTH);
						break;
					case E_DIRECTION_NORTH:
						beam.turn(E_DIRECTION_EAST);
						break;
					case E_DIRECTION_SOUTH:
						beam.turn(E_DIRECTION_WEST);
						break;
					default:
						assert(false);
						break;
				}

				if (is_in_map(beam.coord)) {
					energized.emplace(beam.coord);
					if (history.count(beam) == 0) {
						q.emplace(beam);
						history.emplace(beam);
					}
				}
				break;
			case '-':
				switch (beam.direction) {
					case E_DIRECTION_EAST:
					case E_DIRECTION_WEST:
						beam.turn(beam.direction);
						if (is_in_map(beam.coord)) {
							energized.emplace(beam.coord);
							if (history.count(beam) == 0) {
								q.emplace(beam);
								history.emplace(beam);
							}
						}
						break;
					case E_DIRECTION_SOUTH:
					case E_DIRECTION_NORTH:
						coord = beam.coord;
						beam.turn(E_DIRECTION_WEST);
						if (is_in_map(beam.coord)) {
							energized.emplace(beam.coord);
							if (history.count(beam) == 0) {
								q.emplace(beam);
								history.emplace(beam);
							}
						}

						beam.coord = coord;
						beam.turn(E_DIRECTION_EAST);
						if (is_in_map(beam.coord)) {
							energized.emplace(beam.coord);
							if (history.count(beam) == 0) {
								q.emplace(beam);
								history.emplace(beam);
							}
						}
						break;
					default:
						assert(false);
						break;
				}
				break;
			case '|':
				switch (beam.direction) {
					case E_DIRECTION_NORTH:
					case E_DIRECTION_SOUTH:
						beam.turn(beam.direction);
						if (is_in_map(beam.coord)) {
							energized.emplace(beam.coord);
							if (history.count(beam) == 0) {
								q.emplace(beam);
								history.emplace(beam);
							}
						}
						break;
					case E_DIRECTION_EAST:
					case E_DIRECTION_WEST:
						coord = beam.coord;
						beam.turn(E_DIRECTION_SOUTH);
						if (is_in_map(beam.coord)) {
							energized.emplace(beam.coord);
							if (history.count(beam) == 0) {
								q.emplace(beam);
								history.emplace(beam);
							}
						}

						beam.coord = coord;
						beam.turn(E_DIRECTION_NORTH);
						if (is_in_map(beam.coord)) {
							energized.emplace(beam.coord);
							if (history.count(beam) == 0) {
								q.emplace(beam);
								history.emplace(beam);
							}
						}
						break;
					default:
						assert(false);
						break;
				}
				break;
			default:
				assert(false);
				break;
		}
	}

	return energized.size();
}

int32_t AoC2023_day16::get_aoc_day() {
	return 16;
}

int32_t AoC2023_day16::get_aoc_year() {
	return 2023;
}

void AoC2023_day16::tests() {
	uint64_t result;

	if (init({".|...\\....", "|.-.\\.....", ".....|-...", "........|.", "..........", ".........\\", "..../.\\\\..", ".-.-/..|..", ".|....-|.\\",
			  "..//.|...."})) {
		result = get_energized_tiles_sum();		// 46
		result = get_energized_tiles_sum(true); // 51
	}
}

bool AoC2023_day16::part1() {
	int64_t result = 0;

	result = get_energized_tiles_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day16::part2() {
	int64_t result = 0;

	result = get_energized_tiles_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day16 day16;

	return day16.main_execution();
}
