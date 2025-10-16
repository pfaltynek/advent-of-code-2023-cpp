#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "./../common/ranges.hpp"
#include <map>
#include <queue>
#include <set>
#include <sstream>

struct brick_str {
	coord_3d_str start, end;
	int32_t id;

	brick_str() {
		start = end = {0, 0, 0};
		id = 0;
	}

	brick_str(const int32_t brick_id, const coord_3d_str& s, const coord_3d_str& e) {
		if (s.z > e.z) {
			end = s;
			start = e;
		} else {
			start = s;
			end = e;
		}

		id = brick_id;
	}
	
	bool colision(const brick_str& other) const {
		if (ranges_intersection(start.x, end.x, other.start.x, other.end.x) && ranges_intersection(start.y, end.y, other.start.y, other.end.y) &&
			ranges_intersection(start.z, end.z, other.start.z, other.end.z)) {
			return true;
		} else {
			return false;
		}
	}

	bool colisionXY(const brick_str& other) const {
		if (ranges_intersection(start.x, end.x, other.start.x, other.end.x) && ranges_intersection(start.y, end.y, other.start.y, other.end.y)) {
			return true;
		} else {
			return false;
		}
	}

	void moveZ(int32_t diff) {
		start.z += diff;
		end.z += diff;
	}

	std::string to_string() {
		return "id: " + std::to_string(id) + " " + start.to_string() + "~" + end.to_string();
	}
};

static bool brick_compare(const brick_str& l, const brick_str& r) {
	if (l.start.z < r.start.z) {
		return true;
	} else if (l.start.z > r.start.z) {
		return false;
	} else {
		return (l.end.z <= r.end.z);
	}
}

struct brick_comparer {
	bool operator()(brick_str& l, brick_str& r) const {
		return brick_compare(l, r);
	}
};

class AoC2023_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<brick_str> bricks_;
	std::map<int32_t, std::set<int32_t>> supports_, supported_by_;
	std::set<int32_t> disintegratables_;
	void settle_bricks();
	void check_collisions();
	void print_bricks();
	int64_t get_safely_disintegrated_bricks_count();
	int64_t get_brick_fall_sum();
};

bool AoC2023_day22::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	std::vector<int32_t> numbers;
	uint32_t tmp;
	int32_t id;

	bricks_.clear();

	id = 0;

	for (uint32_t i = 0; i < lines.size(); i++) {
		numbers.clear();

		ss.clear();
		ss.str(lines[i]);

		while (!ss.eof()) {
			ss >> tmp;
			ss.ignore(1);
			numbers.push_back(tmp);
		}

		if (numbers.size() != 6) {
			std::cout << "Invalid brick format at line " << i + 1 << std::endl;
			return false;
		} else {
			coord_3d_str c1, c2;
			c1 = {numbers[0], numbers[1], numbers[2]};
			c2 = {numbers[3], numbers[4], numbers[5]};

			brick_str brick(id++, c1, c2);
			bricks_.push_back(brick);
		}
	}

	std::sort(bricks_.begin(), bricks_.end(), brick_compare);

	return true;
}

void AoC2023_day22::print_bricks() {
	for (size_t i = 0; i < bricks_.size(); i++) {
		std::cout << "Brick " << i << ": " << bricks_[i].to_string() << std::endl;
	}
}

void AoC2023_day22::settle_bricks() {
	int32_t diff;

	// make sure the first brick is laying flat on the ground
	diff = bricks_[0].start.z - 1;
	bricks_[0].moveZ(-diff);

	for (size_t i = 1; i < bricks_.size(); i++) {
		int32_t new_start = 1;

		for (size_t j = 0; j < i; j++) {
			if (bricks_[i].colisionXY(bricks_[j])) {
				if (bricks_[j].end.z + 1 > new_start) {
					new_start = bricks_[j].end.z + 1;
				}
			}
		}

		
		diff = bricks_[i].start.z - new_start;
		if (diff) {
			bricks_[i].moveZ(-diff);
		}
	}
	
	// std::sort(bricks_.begin(), bricks_.end(), brick_compare);

	// check_collisions();
}

void AoC2023_day22::check_collisions() {
	std::cout << "-----------------------------------------------" << std::endl;

	for (size_t i = 0; i < bricks_.size(); i++) {
		for (size_t j = 0; j < bricks_.size(); j++) {
			if (i == j) {
				continue;
			}

			if (bricks_[i].colision(bricks_[j])) {
				std::cout << "Error: bricks " << i << " and " << j << " are intersecting!   " << bricks_[i].to_string() << "   " << bricks_[j].to_string()
						  << std::endl
						  << "XY collision: " << bricks_[i].colisionXY(bricks_[j]) << std::endl;
				// return -1;
			}
		}
	}

	std::cout << "-----------------------------------------------" << std::endl;
}

int64_t AoC2023_day22::get_safely_disintegrated_bricks_count() {
	std::map<int32_t, std::set<int32_t>> starts, ends;

	if (bricks_.empty()) {
		return 0;
	}

	settle_bricks();

	starts.clear();
	ends.clear();
	supports_.clear();
	supported_by_.clear();
	disintegratables_.clear();

	for (size_t i = 0; i < bricks_.size(); i++) {
		starts[bricks_[i].start.z].emplace(i);
		ends[bricks_[i].end.z].emplace(i);
	}

	for (const auto& end : ends) {
		for (const auto& end_idx : end.second) {
			for (const auto& start_id : starts[end.first + 1]) {
				if (bricks_[end_idx].colisionXY(bricks_[start_id])) {
					supports_[end_idx].emplace(start_id);
					supported_by_[start_id].emplace(end_idx);
				}
			}
		}
	}

	for (size_t i = 0; i < bricks_.size(); i++) {
		if (supports_.count(i) == 0) {
			disintegratables_.emplace(i);
		} else {
			bool fall = false;

			for (const auto& s : supports_[i]) {
				if (supported_by_[s].size() < 2) {
					fall = true;
					break;
				}
			}
			if (!fall) {
				disintegratables_.emplace(i);
			}
		}
	}

	return disintegratables_.size();
}

int64_t AoC2023_day22::get_brick_fall_sum() {
	int64_t result = 0;
	std::set<int32_t> fallen;
	std::queue<int32_t> q;
	int32_t start_idx;
	bool fall;

	for (size_t i = 0; i < bricks_.size(); i++) {
		if (disintegratables_.count(i)) {
			continue;
		}

		q.push(i);
		fallen.clear();
		start_idx = i;

		while (!q.empty()) {
			int32_t idx = q.front();
			q.pop();
			fall = false;

			if (fallen.count(idx)) {
				continue;
			}

			if (idx == start_idx) {
				fall = true;
			} else {
				fall = true;
				for (const auto& sb : supported_by_[idx]) {
					if (fallen.count(sb) == 0) {
						fall = false;
						break;
					}
				}
			}
			if (fall) {
				fallen.emplace(idx);
				for (const auto& s : supports_[idx]) {
					q.push(s);
				}
			}
		}

		result += fallen.size() - 1;
	}

	return result;
}

int32_t AoC2023_day22::get_aoc_day() {
	return 22;
}

int32_t AoC2023_day22::get_aoc_year() {
	return 2023;
}

void AoC2023_day22::tests() {
	int64_t result;

	if (init({"1,0,1~1,2,1", "0,0,2~2,0,2", "0,2,3~2,2,3", "0,0,4~0,2,4", "2,0,5~2,2,5", "0,1,6~2,1,6", "1,1,8~1,1,9"})) {
		result = get_safely_disintegrated_bricks_count(); // 5
		result = get_brick_fall_sum();					  // 7
	}
}

bool AoC2023_day22::part1() {
	int64_t result = 0;

	result = get_safely_disintegrated_bricks_count();
	
	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day22::part2() {
	int64_t result = 0;

	result = get_brick_fall_sum();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day22 day22;

	return day22.main_execution();
}
