#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <numeric>
#include <sstream>

const std::string C_PATH_DIRECTIONS_VALID = "LR";
const std::string C_START_NODE = "AAA";
const std::string C_FINISH_NODE = "ZZZ";

class AoC2023_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::string path_;
	std::map<std::string, std::pair<std::string, std::string>> tree_;
	int64_t get_steps_count_to_ZZZ(const bool part2 = false);
	std::vector<int64_t> get_prime_numbers(const int64_t max);
};

bool AoC2023_day08::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	std::string root;
	std::pair<std::string, std::string> leaves;

	path_.clear();
	tree_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		switch (i) {
			case 0:
				if (lines[i].find_first_not_of(C_PATH_DIRECTIONS_VALID) != std::string::npos) {
					std::cout << "Invalid path identifier at line " << i + 1 << std::endl;
					return false;
				} else {
					path_ = lines[i];
				}
				break;
			case 1:
				if (!lines[i].empty()) {
					std::cout << "Invalid data format at line " << i + 1 << std::endl;
					return false;
				}
				break;
			default:
				ss >> root;
				ss.ignore(4); // " = ("
				ss >> leaves.first;
				leaves.first.pop_back();
				ss >> leaves.second;
				leaves.second.pop_back();
				tree_[root] = leaves;
				break;
		}
	}

	return true;
}

std::vector<int64_t> AoC2023_day08::get_prime_numbers(const int64_t max) {
	std::vector<int64_t> result;

	result.clear();
	result = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

	for (int64_t i = 100; i <= max; i++) {
		bool prime = true;

		for (size_t j = 0; j < result.size(); j++) {
			if (i % result[j] == 0) {
				prime = false;
				break;
			}
		}

		if (prime) {
			result.push_back(i);
		}
	}

	return result;
}

int64_t AoC2023_day08::get_steps_count_to_ZZZ(const bool part2) {
	int64_t result = 0, max = 0;
	std::string current = C_START_NODE;
	std::vector<std::string> currents;
	std::vector<int64_t> periods, counts, primes;
	std::vector<std::map<int64_t, int64_t>> lcms;
	std::map<int64_t, int64_t> lcm;

	size_t idx = 0, finished = 0;
	bool left;

	if (part2) {
		currents.clear();
		periods.clear();
		counts.clear();
		lcms.clear();

		for (auto& pair : tree_) {
			if (pair.first.back() == 'A') {
				currents.push_back(pair.first);
				periods.push_back(0);
				counts.push_back(0);
			}
		}

		while (finished != currents.size()) {
			left = path_[idx] == 'L';
			idx++;
			idx %= path_.size();
			result++;

			for (size_t i = 0; i < currents.size(); i++) {
				if (tree_.count(currents[i])) {

					if (left) {
						currents[i] = tree_[currents[i]].first;
					} else {
						currents[i] = tree_[currents[i]].second;
					}

					if (currents[i].back() == 'Z') {
						if (periods[i] == 0) {
							periods[i] = result;
						} else {
							if (result % periods[i] == 0) {
								counts[i]++;
							} else {
								periods[i] = result;
								counts[i] = 0;
							}
						}
					}
				} else {
					return -1;
				}
			}
			finished = 0;

			for (auto& cnt : counts) {
				if (cnt >= 5) {
					finished++;
				}
			}
		}

		for (auto& p : periods) {
			if (p > max) {
				max = p;
			}
		}

		primes = get_prime_numbers(max / 2);

		for (auto& p : periods) {
			int64_t x;
			lcm.clear();

			x = p;
			for (auto& prime : primes) {
				while (((x % prime) == 0) && (x > 1)) {
					x /= prime;
					lcm[prime]++;
				}

				if (x < prime) {
					break;
				}
			}

			lcms.push_back(lcm);
		}

		lcm.clear();

		for (auto& item : lcms) {
			for (auto& pair : item) {
				if (lcm.count(pair.first)) {
					if (lcm[pair.first] < pair.second) {
						lcm[pair.first] = pair.second;
					}
				} else {
					lcm.insert(pair);
				}
			}
		}

		result = 1;

		for (auto& pair : lcm) {
			for (int64_t i = 0; i < pair.second; i++)
			{
				result *= pair.first;
			}
		}
	} else {
		while (current != C_FINISH_NODE) {
			if (tree_.count(current)) {
				left = path_[idx] == 'L';
				idx++;
				idx %= path_.size();

				if (left) {
					current = tree_[current].first;
				} else {
					current = tree_[current].second;
				}

				result++;
			} else {
				return -1;
			}
		}
	}

	return result;
}

int32_t AoC2023_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2023_day08::get_aoc_year() {
	return 2023;
}

void AoC2023_day08::tests() {
	uint64_t result;

	if (init({"RL", "", "AAA = (BBB, CCC)", "BBB = (DDD, EEE)", "CCC = (ZZZ, GGG)", "DDD = (DDD, DDD)", "EEE = (EEE, EEE)", "GGG = (GGG, GGG)",
			  "ZZZ = (ZZZ, ZZZ)"})) {
		result = get_steps_count_to_ZZZ(); // 2
	}

	if (init({"LLR", "", "AAA = (BBB, BBB)", "BBB = (AAA, ZZZ)", "ZZZ = (ZZZ, ZZZ)"})) {
		result = get_steps_count_to_ZZZ(); // 6
	}

	if (init({"LR", "", "11A = (11B, XXX)", "11B = (XXX, 11Z)", "11Z = (11B, XXX)", "22A = (22B, XXX)", "22B = (22C, 22C)", "22C = (22Z, 22Z)",
			  "22Z = (22B, 22B)", "XXX = (XXX, XXX)"})) {
		result = get_steps_count_to_ZZZ(true); // 6
	}
}

bool AoC2023_day08::part1() {
	int64_t result = 0;

	result = get_steps_count_to_ZZZ();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day08::part2() {
	int64_t result = 0;

	result = get_steps_count_to_ZZZ(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day08 day08;

	return day08.main_execution();
}
