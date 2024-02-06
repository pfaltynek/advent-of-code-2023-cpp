#include "./../common/aoc.hpp"
#include <map>

class AoC2023_day13 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::vector<std::string>> patterns_, flipped_;
	std::map<uint64_t, uint64_t> cache_;
	uint64_t get_pattern_mirroring_notes_sum(const bool part2);
	uint64_t get_mirror_position_vert(const std::vector<std::string>& pattern, const uint64_t skip);
	std::vector<std::string> get_pattern_flipped(const std::vector<std::string>& pattern);
	bool has_1_smudge(const std::string& left, const std::string& right, size_t& idx);
	uint64_t identify_smudge(const std::vector<std::string>& pattern, const uint64_t cached);
};

bool AoC2023_day13::init(const std::vector<std::string> lines) {
	std::vector<std::string> pattern;
	size_t size = 0;

	patterns_.clear();
	flipped_.clear();
	cache_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (size == 0) {
			size = lines[i].size();
			pattern.clear();
			pattern.push_back(lines[i]);
		} else {
			if (lines[i].empty()) {
				patterns_.push_back(pattern);
				flipped_.push_back(get_pattern_flipped(pattern));
				size = 0;
			} else {
				if (lines[i].size() != size) {
					std::cout << "Invalid pattern size at line " << i + 1 << std::endl;
					return false;
				} else {
					pattern.push_back(lines[i]);
				}
			}
		}
	}

	if (pattern.size() > 0) {
		patterns_.push_back(pattern);
		flipped_.push_back(get_pattern_flipped(pattern));
	}

	return true;
}

uint64_t AoC2023_day13::get_pattern_mirroring_notes_sum(const bool part2 = false) {
	uint64_t result = 0, subresult;
	size_t max = std::min(patterns_.size(), flipped_.size());

	for (size_t i = 0; i < max; i++) {
		if (part2) {
			subresult = identify_smudge(patterns_[i], cache_[i]);

			if (subresult == 0) {
				subresult = identify_smudge(flipped_[i], cache_[i * 1000]);

				if (subresult == 0) {
					assert(false);
				} else {
					result += subresult;
				}
			} else {
				result += 100 * subresult;
			}
		} else {
			subresult = get_mirror_position_vert(flipped_[i], 0);
			cache_[i * 1000] = subresult;

			if (subresult == 0) {
				subresult = get_mirror_position_vert(patterns_[i], 0);
				cache_[i] = subresult;

				if (subresult == 0) {
					assert(false);
				} else {
					result += 100 * subresult;
				}
			} else {
				result += subresult;
				cache_[i] = 0;
			}
		}
	}

	return result;
}

std::vector<std::string> AoC2023_day13::get_pattern_flipped(const std::vector<std::string>& pattern) {
	std::vector<std::string> flipped;
	std::string tmp;

	flipped = {};

	for (size_t x = 0; x < pattern[0].size(); x++) {
		tmp.clear();

		for (size_t y = 0; y < pattern.size(); y++) {
			tmp += pattern[y][x];
		}
		flipped.push_back(tmp);
	}

	return flipped;
}

uint64_t AoC2023_day13::get_mirror_position_vert(const std::vector<std::string>& pattern, const uint64_t skip) {
	for (size_t i = 1; i < pattern.size(); i++) {
		if ((pattern[i] == pattern[i - 1]) && (i != skip)) {
			bool mirror = true;
			int16_t l, r, s;

			s = pattern.size();
			l = i - 2;
			r = i + 1;
			while ((l >= 0) && (r < s)) {
				if (pattern[l] != pattern[r]) {
					mirror = false;
					break;
				}
				l--;
				r++;
			}

			if (mirror) {
				return i;
			}
		}
	}

	return 0;
}

uint64_t AoC2023_day13::identify_smudge(const std::vector<std::string>& pattern, const uint64_t cached) {
	std::vector<std::string> tmp;
	uint64_t subresult;
	size_t idx;
	char was;

	tmp = pattern;

	for (size_t j = 0; j < tmp.size(); j++) {
		for (size_t k = j + 1; k < tmp.size(); k++) {
			if (has_1_smudge(tmp[j], tmp[k], idx)) {
				was = tmp[j][idx];
				tmp[j][idx] = (was == '#') ? '.' : '#';
				subresult = get_mirror_position_vert(tmp, cached);

				if (subresult != 0) {
					return subresult;
				}
				tmp[j][idx] = was;

				was = tmp[k][idx];
				tmp[k][idx] = (was == '#') ? '.' : '#';
				subresult = get_mirror_position_vert(tmp, cached);

				if (subresult != 0) {
					return subresult;
				}
				tmp[k][idx] = was;
			}
		}
	}

	return 0;
}

bool AoC2023_day13::has_1_smudge(const std::string& left, const std::string& right, size_t& idx) {
	size_t max = std::min(left.size(), right.size());
	bool smudge_found = false;

	for (size_t i = 0; i < max; i++) {
		if (left[i] != right[i]) {
			if (smudge_found) {
				return false;
			} else {
				smudge_found = true;
				idx = i;
			}
		}
	}

	return smudge_found;
}

int32_t AoC2023_day13::get_aoc_day() {
	return 13;
}

int32_t AoC2023_day13::get_aoc_year() {
	return 2023;
}

void AoC2023_day13::tests() {
	uint64_t result;

	if (init({"#.##..##.", "..#.##.#.", "##......#", "##......#", "..#.##.#.", "..##..##.", "#.#.##.#.", "", "#...##..#", "#....#..#", "..##..###", "#####.##.",
			  "#####.##.", "..##..###", "#....#..#"})) {
		result = get_pattern_mirroring_notes_sum();		// 405
		result = get_pattern_mirroring_notes_sum(true); // 400
	}
}

bool AoC2023_day13::part1() {
	int64_t result = 0;

	result = get_pattern_mirroring_notes_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day13::part2() {
	int64_t result = 0;

	result = get_pattern_mirroring_notes_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day13 day13;

	return day13.main_execution();
}
