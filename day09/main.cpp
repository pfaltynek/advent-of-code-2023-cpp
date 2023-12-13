#include "./../common/aoc.hpp"
#include <algorithm>
#include <sstream>

class AoC2023_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::vector<int32_t>> history_;
	int64_t get_extrapolated_values_sum(const bool part2 = false);
	int32_t extrapolate(const std::vector<int32_t> data, const bool part2);
};

bool AoC2023_day09::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	std::vector<int32_t> hist;
	int32_t tmp;

	history_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);
		hist.clear();

		while (!ss.eof()) {
			ss >> tmp;
			hist.push_back(tmp);
		}

		history_.push_back(hist);
	}

	return true;
}

int32_t AoC2023_day09::extrapolate(const std::vector<int32_t> data, const bool part2) {
	std::vector<std::vector<int32_t>> diffs;
	bool all_zeroes = false;
	std::vector<int32_t> new_line;
	size_t idx;
	int32_t tmp;
	std::vector<int32_t> lasts;

	lasts.clear();
	diffs.clear();
	diffs.push_back(data);

	while (!all_zeroes) {
		new_line.clear();
		all_zeroes = true;

		for (size_t i = 1; i < diffs.back().size(); i++) {
			tmp = diffs.back()[i] - diffs.back()[i - 1];
			new_line.push_back(tmp);
			if (tmp != 0) {
				all_zeroes = false;
			}
		}
		diffs.push_back(new_line);
	}

	std::reverse(diffs.begin(), diffs.end());
	tmp = 0;

	if (part2) {
		for (size_t i = 1; i < diffs.size(); i++) {
			tmp = diffs[i].front() - tmp;
		}
	} else {
		for (size_t i = 1; i < diffs.size(); i++) {
			tmp = tmp + diffs[i].back();
		}
	}
	return tmp;
}

int64_t AoC2023_day09::get_extrapolated_values_sum(const bool part2) {
	int64_t result = 0;

	for (auto& item : history_) {
		result += extrapolate(item, part2);
	}

	return result;
}

int32_t AoC2023_day09::get_aoc_day() {
	return 9;
}

int32_t AoC2023_day09::get_aoc_year() {
	return 2023;
}

void AoC2023_day09::tests() {
	uint64_t result;

	if (init({"0 3 6 9 12 15", "1 3 6 10 15 21", "10 13 16 21 30 45"})) {
		result = get_extrapolated_values_sum(); // 114
		result = get_extrapolated_values_sum(true); // 2
	}
}

bool AoC2023_day09::part1() {
	int64_t result = 0;

	result = get_extrapolated_values_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day09::part2() {
	int64_t result = 0;

	result = get_extrapolated_values_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day09 day09;

	return day09.main_execution();
}
