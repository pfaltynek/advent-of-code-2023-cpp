#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

struct dig_str {
	char direction;
	int64_t size;
	int64_t color;

	void decode_color() {
		size = color / 16;

		switch (color % 16) {
			case 0:
				direction = 'R';
				break;
			case 1:
				direction = 'D';
				break;
			case 2:
				direction = 'L';
				break;
			case 3:
				direction = 'U';
				break;
			default:
				assert(false);
				break;
		}
	}
};

static bool compare2sort(std::pair<int64_t, int64_t> l, std::pair<int64_t, int64_t> r) {
	if (l.first == r.first) {
		return l.second < r.second;
	} else {
		return l.first < r.first;
	}
}

class AoC2023_day18 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<dig_str> digs_;
	uint64_t get_lava_cubic_meters_room(const bool part2);
};

bool AoC2023_day18::init(const std::vector<std::string> lines) {
	dig_str dig;
	std::stringstream ss;
	const std::string C_DIRECTIONS = "UDLR";

	digs_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss >> std::dec;
		ss.str(lines[i]);
		ss >> dig.direction;
		ss >> dig.size;
		ss.ignore(3);
		ss >> std::hex >> dig.color;

		if (C_DIRECTIONS.find(dig.direction) == std::string::npos) {
			std::cout << "Invalid direction at line " << i + 1 << std::endl;
			return false;
		}

		digs_.push_back(dig);
	}

	return true;
}

uint64_t AoC2023_day18::get_lava_cubic_meters_room(const bool part2 = false) {
	uint64_t result = 0, border = 0;
	bool in;
	std::map<int64_t, std::vector<std::pair<int64_t, int64_t>>> h, v;
	std::set<int64_t> u, d;
	int64_t minx = 0, miny = 0, maxx = 0, maxy = 0, x = 0, y = 0, last;
	dig_str dig;

	h.clear();
	v.clear();

	for (size_t i = 0; i < digs_.size(); i++) {
		dig = digs_[i];

		if (part2) {
			dig.decode_color();
		}

		border += dig.size;

		switch (dig.direction) {
			case 'D':
				v[x].push_back({y, y + dig.size});
				y += dig.size;
				break;
			case 'U':
				v[x].push_back({y - dig.size, y});
				y -= dig.size;
				break;
			case 'L':
				h[y].push_back({x - dig.size, x});
				x -= dig.size;
				break;
			case 'R':
				h[y].push_back({x, x + dig.size});
				x += dig.size;
				break;
			default:
				std::cout << "Invalid direction at line " << i + 1 << std::endl;
				return false;
		}

		if (x > maxx) {
			maxx = x;
		}
		if (y > maxy) {
			maxy = y;
		}
		if (x < minx) {
			minx = x;
		}
		if (y < miny) {
			miny = y;
		}
	}

	std::vector<std::pair<int64_t, int64_t>> line;
	for (int64_t yy = miny; yy <= maxy; yy++) {
		line.clear();

		if (h.count(yy)) {
			line = h[yy];
		}

		u.clear();
		d.clear();

		for (auto& pair : v) {
			for (size_t p = 0; p < pair.second.size(); p++) {
				if (pair.second[p].first == yy) {
					u.emplace(pair.first);
				} else if (pair.second[p].second == yy) {
					d.emplace(pair.first);
				} else if ((pair.second[p].first < yy) && (pair.second[p].second > yy)) {
					line.push_back({pair.first, pair.first});
				}
			}
		}

		std::sort(line.begin(), line.end(), compare2sort);

		in = false;
		last = minx;

		for (size_t i = 0; i < line.size(); i++) {
			if (line[i].first == line[i].second) {
				if (in) {
					result += line[i].second - last - 1;
				}

				in = !in;
				last = line[i].second;
			} else {
				if ((u.count(line[i].first) && d.count(line[i].second)) || (d.count(line[i].first) && u.count(line[i].second))) {
					if (in) {
						result += line[i].first - last - 1;
					}
					in = !in;
				} else {
					if (in) {
						result += line[i].first - last - 1;
					}
				}

				last = line[i].second;
			}
		}
	}

	return result + border;
}

int32_t AoC2023_day18::get_aoc_day() {
	return 18;
}

int32_t AoC2023_day18::get_aoc_year() {
	return 2023;
}

void AoC2023_day18::tests() {
	uint64_t result;

	if (init({"R 6 (#70c710)", "D 5 (#0dc571)", "L 2 (#5713f0)", "D 2 (#d2c081)", "R 2 (#59c680)", "D 2 (#411b91)", "L 5 (#8ceee2)", "U 2 (#caa173)",
			  "L 1 (#1b58a2)", "U 2 (#caa171)", "R 2 (#7807d2)", "U 3 (#a77fa3)", "L 2 (#015232)", "U 2 (#7a21e3)"})) {
		result = get_lava_cubic_meters_room();	   // 62
		result = get_lava_cubic_meters_room(true); // 952408144115
	}
}

bool AoC2023_day18::part1() {
	int64_t result = 0;

	result = get_lava_cubic_meters_room();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day18::part2() {
	int64_t result = 0;

	result = get_lava_cubic_meters_room(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day18 day18;

	return day18.main_execution();
}
