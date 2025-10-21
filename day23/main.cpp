#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>
#include <sstream>

struct hike_state_str {
	coord_str position;
	int32_t step_count;
	std::set<coord_str> visited;
	coord_str last_node;
};

std::vector<coord_str> directions = {coord_step_north, coord_step_south, coord_step_east, coord_step_west};

class AoC2023_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> forest_, path_;
	std::map<coord_str, char> slopes_;
	coord_str start_, end_;
	std::map<coord_str, std::map<coord_str, int32_t>> graph_;
	int64_t find_longest_hike_part1();
	int64_t find_longest_hike_part2();
	void step_loop(coord_str position, int32_t step_count, int64_t& longest_hike, std::set<coord_str>& visited);
	void step_loop2(coord_str position, int32_t step_count, int64_t& longest_hike, std::set<coord_str>& visited);
};

bool AoC2023_day23::init(const std::vector<std::string> lines) {
	size_t size;
	bool start_found, end_found;

	forest_.clear();
	path_.clear();
	slopes_.clear();
	start_found = false;
	end_found = false;

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("><v^#.") != std::string::npos) {
			std::cout << "Invalid character in line " << i + 1 << std::endl;
			return false;
		}

		if (i == 0) {
			size = lines[i].size();
		} else {
			if (lines[i].size() != size) {
				std::cout << "Inconsistent line size at line " << i + 1 << std::endl;
				return false;
			}
		}

		for (size_t j = 0; j < size; j++) {
			switch (lines[i][j]) {
				case '#':
					forest_.emplace(j, i);
					break;
				case '.':
					path_.emplace(j, i);

					if (i == 0) {
						if (start_found) {
							std::cout << "Multiple start points found!" << std::endl;
							return false;
						}
						start_ = {static_cast<int32_t>(j), static_cast<int32_t>(i)};
						start_found = true;
					}

					if (i == lines.size() - 1) {
						if (end_found) {
							std::cout << "Multiple end points found!" << std::endl;
							return false;
						}
						end_ = {static_cast<int32_t>(j), static_cast<int32_t>(i)};
						end_found = true;
					}

					break;
				case '>':
				case '<':
				case '^':
				case 'v':
					slopes_[{static_cast<int32_t>(j), static_cast<int32_t>(i)}] = lines[i][j];
					break;
				default:
					break;
			}
		}
	}

	return true;
}

void AoC2023_day23::step_loop(coord_str position, int32_t step_count, int64_t& longest_hike, std::set<coord_str>& visited) {
	coord_str new_pos;

	if (position == end_) {
		if (step_count > longest_hike) {
			longest_hike = step_count;
		}
		return;
	}

	if (forest_.count(position)) {
		assert(false);
		return;
	}

	if (slopes_.count(position)) {
		switch (slopes_[position]) {
			case '>':
				new_pos = position + coord_step_east;
				break;
			case '<':
				new_pos = position + coord_step_west;
				break;
			case '^':
				new_pos = position + coord_step_north;
				break;
			case 'v':
				new_pos = position + coord_step_south;
				break;
			default:
				break;
		}

		if (!forest_.count(new_pos) && !visited.count(new_pos)) {
			visited.emplace(new_pos);
			step_loop(new_pos, step_count + 1, longest_hike, visited);
			visited.erase(new_pos);
		}
	} else if (path_.count(position)) {
		for (const auto& dir : directions) {
			coord_str new_pos = position + dir;

			if (!forest_.count(new_pos) && !visited.count(new_pos)) {
				visited.emplace(new_pos);
				step_loop(new_pos, step_count + 1, longest_hike, visited);
				visited.erase(new_pos);
			}
		}
	}
}

int64_t AoC2023_day23::find_longest_hike_part2() {
	int64_t longest_hike = 0;
	std::queue<hike_state_str> q = {};
	hike_state_str hike_state;
	coord_str next_step;
	std::set<coord_str> full_path = {}, visited = {};
	std::vector<coord_str> next_steps = {};
	std::map<coord_str, int32_t> nodes = {};

	full_path.insert(path_.begin(), path_.end());
	for (const auto& slope : slopes_) {
		full_path.emplace(slope.first);
	}

	for (const auto& pos : full_path) {
		next_steps.clear();

		for (const auto& dir : directions) {
			next_step = static_cast<coord_str>(pos) + static_cast<coord_str>(dir);
			if (full_path.count(next_step)) {
				next_steps.push_back(next_step);
			}
		}
		if (next_steps.size() > 2) {
			nodes[pos] = static_cast<int32_t>(next_steps.size());
		}
	}

	hike_state.position = start_;
	hike_state.step_count = 0;
	hike_state.last_node = start_;
	q.push(hike_state);
	graph_.clear();

	while (!q.empty()) {
		hike_state = q.front();
		q.pop();

		while (true) {
			if (visited.count(hike_state.position)) {
				break;
			}

			if (hike_state.position == end_) {
				graph_[hike_state.last_node][hike_state.position] = hike_state.step_count;
				graph_[hike_state.position][hike_state.last_node] = hike_state.step_count;
				break;
			}

			next_steps.clear();
			for (const auto& dir : directions) {
				next_step = hike_state.position + dir;
				if (full_path.count(next_step) && !visited.count(next_step) && !(next_step == hike_state.last_node)) {
					next_steps.push_back(next_step);
				}
			}

			if (nodes.count(hike_state.position)) {
				graph_[hike_state.last_node][hike_state.position] = hike_state.step_count;
				graph_[hike_state.position][hike_state.last_node] = hike_state.step_count;

				hike_state.step_count = 1;
				hike_state.last_node = hike_state.position;
				for (const auto& pos : next_steps) {
					hike_state.position = pos;
					q.push(hike_state);
				}
				break;
			} else {
				visited.emplace(hike_state.position);
				if (next_steps.size() == 1) {
					hike_state.position = next_steps[0];
					hike_state.step_count++;
				} else {
					break;
				}
			}
		}
	}

	visited = {start_};
	step_loop2(start_, 0, longest_hike, visited);

	return longest_hike;
}

void AoC2023_day23::step_loop2(coord_str position, int32_t step_count, int64_t& longest_hike, std::set<coord_str>& visited) {
	if (position == end_) {
		if (step_count > longest_hike) {
			longest_hike = step_count;
		}
		return;
	}

	for (const auto& next : graph_[position]) {
		if (visited.count(next.first) == 0) {
			visited.emplace(next.first);
			step_loop2(next.first, step_count + next.second, longest_hike, visited);
			visited.erase(next.first);
		}
	}
}

int64_t AoC2023_day23::find_longest_hike_part1() {
	int64_t longest_hike = 0;
	std::set<coord_str> visited{start_};

	step_loop(start_, 0, longest_hike, visited);

	return longest_hike;
}

int32_t AoC2023_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2023_day23::get_aoc_year() {
	return 2023;
}

void AoC2023_day23::tests() {
	int64_t result;

	if (init({"#.#####################", "#.......#########...###", "#######.#########.#.###", "###.....#.>.>.###.#.###", "###v#####.#v#.###.#.###",
			  "###.>...#.#.#.....#...#", "###v###.#.#.#########.#", "###...#.#.#.......#...#", "#####.#.#.#######.#.###", "#.....#.#.#.......#...#",
			  "#.#####.#.#.#########v#", "#.#...#...#...###...>.#", "#.#.#v#######v###.###v#", "#...#.>.#...>.>.#.###.#", "#####v#.#.###v#.#.###.#",
			  "#.....#...#...#.#.#...#", "#.#########.###.#.#.###", "#...###...#...#...#.###", "###.###.#.###v#####v###", "#...#...#.#.>.>.#.>.###",
			  "#.###.###.#.###.#.#v###", "#.....###...###...#...#", "#####################.#"})) {
		result = find_longest_hike_part1(); // 94
		result = find_longest_hike_part2(); // 154
	}
}

bool AoC2023_day23::part1() {
	int64_t result = 0;

	result = find_longest_hike_part1();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day23::part2() {
	int64_t result = 0;

	result = find_longest_hike_part2();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day23 day23;

	return day23.main_execution();
}
