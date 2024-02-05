#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>

// My original solution used bitmasks, it was quick but not precise :(
// following solution got inspired from https://pastebin.com/djb8RJ85

struct row_info_str {
	std::string mask;
	std::vector<uint32_t> sizes;

	void clear() {
		mask.clear();
		sizes.clear();
	}
};

class AoC2023_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<row_info_str> rows_;
	std::map<std::string, uint64_t> cache_;
	uint64_t get_arangements_sum(const bool part2);
	uint64_t get_count(std::string springs, std::vector<uint32_t> groups);
	uint64_t get_count_recurse(std::string springs, std::vector<uint32_t> groups);
};

bool AoC2023_day12::init(const std::vector<std::string> lines) {
	std::vector<std::string> parts, numbers;
	row_info_str row;

	rows_.clear();
	cache_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		parts = split(lines[i], " ");

		if (parts.size() != 2) {
			std::cout << "Invalid row info format at line " << i + 1 << std::endl;
			return false;
		}

		if (parts[0].find_first_not_of("#.?") != std::string::npos) {
			std::cout << "Invalid row info char at line " << i + 1;
			return false;
		}

		row.clear();
		row.mask = parts[0];

		numbers = split(parts[1], ",");

		for (size_t j = 0; j < numbers.size(); j++) {
			row.sizes.push_back(std::stoi(numbers[j]));
		}

		rows_.push_back(row);
	}

	return true;
}

uint64_t AoC2023_day12::get_count(std::string springs, std::vector<uint32_t> groups) {
	uint64_t result;
	std::vector<std::string> sizes = {};

	for (size_t i = 0; i < groups.size(); i++) {
		sizes.push_back(std::to_string(groups[i]));
	}

	std::string key = springs + ' ' + join(sizes, ",");

	if (cache_.count(key) != 0) {
		return cache_[key];
	}

	result = get_count_recurse(springs, groups);

	cache_[key] = result;

	return result;
}

uint64_t AoC2023_day12::get_count_recurse(std::string springs, std::vector<uint32_t> groups) {

	while (true) {

		if (groups.size() == 0) {
			if (springs.find('#') == std::string::npos) {
				return 1;
			} else {
				return 0;
			}
		}

		if (springs.empty()) {
			return 0;
		}

		while (springs.front() == '.') {
			springs = springs.substr(1);
		}

		if (springs.front() == '?') {
			std::string s1, s2;

			s1 = s2 = springs;
			s1.front() = '.';
			s2.front() = '#';
			return get_count(s1, groups) + get_count(s2, groups);
		}

		if (springs.front() == '#') {
			if (groups.size() == 0) {
				return 0;
			}

			if (springs.size() < groups[0]) {
				return 0; // not enough characters to match the group size
			}

			if (springs.find('.') < groups[0]) {
				return 0; // group cannot contain a dot
			}

			if (groups.size() > 1) {
				if (springs.size() < groups[0] + 1 || springs[groups[0]] == '#') {
					return 0; // group cannot be followed by spring and springs must have sufficient length to have group size
				}

				springs = springs.substr(groups[0] + 1); // skip the character after group - it is dot or question mark
				groups.erase(groups.begin());
				continue;
			}
			springs = springs.substr(groups[0]); // last group, no need to check the character after group
			groups.erase(groups.begin());
			continue;
		}
	}

	return 0;
}

uint64_t AoC2023_day12::get_arangements_sum(const bool part2 = false) {
	int64_t result = 0, subresult;

	for (size_t i = 0; i < rows_.size(); i++) {
		if (part2) {
			std::string mask = "";
			std::vector<uint32_t> sizes = {};

			mask = rows_[i].mask + "?" + rows_[i].mask + "?" + rows_[i].mask + "?" + rows_[i].mask + "?" + rows_[i].mask;
			sizes.insert(sizes.end(), rows_[i].sizes.begin(), rows_[i].sizes.end());
			sizes.insert(sizes.end(), rows_[i].sizes.begin(), rows_[i].sizes.end());
			sizes.insert(sizes.end(), rows_[i].sizes.begin(), rows_[i].sizes.end());
			sizes.insert(sizes.end(), rows_[i].sizes.begin(), rows_[i].sizes.end());
			sizes.insert(sizes.end(), rows_[i].sizes.begin(), rows_[i].sizes.end());

			subresult = get_count(mask, sizes);
		} else {
			subresult = get_count(rows_[i].mask, rows_[i].sizes);
		}
		result += subresult;
	}

	return result;
}

int32_t AoC2023_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2023_day12::get_aoc_year() {
	return 2023;
}

void AoC2023_day12::tests() {
	uint64_t result;

	if (init({"???.### 1,1,3", ".??..??...?##. 1,1,3", "?#?#?#?#?#?#?#? 1,3,1,6", "????.#...#... 4,1,1", "????.######..#####. 1,6,5", "?###???????? 3,2,1"})) {
		result = get_arangements_sum(); // 21
		result = get_arangements_sum(true); // 525152
	}
}

bool AoC2023_day12::part1() {
	int64_t result = 0;

	result = get_arangements_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day12::part2() {
	int64_t result = 0;

	result = get_arangements_sum(true); // 441440315737, 10861030975833

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day12 day12;

	return day12.main_execution();
}
