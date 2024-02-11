#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>

const uint64_t C_HASH_DIVIDER = 256;

struct label_str {
	std::string name;
	char focal_length;
};

class AoC2023_day15 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> strings_;
	uint64_t get_hashes_sum(const bool part2);
	uint64_t calculate_hash(const std::string& string);
	label_str get_label(const std::string hashmap, bool& is_dash_operation);
};

bool AoC2023_day15::init(const std::vector<std::string> lines) {
	std::string input;

	strings_.clear();
	input.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		input += lines[i];
	}

	strings_ = split(input, ",");

	return true;
}

uint64_t AoC2023_day15::calculate_hash(const std::string& string) {
	uint64_t result = 0;

	for (size_t i = 0; i < string.size(); i++) {
		result += string[i];
		result *= 17;
		result %= C_HASH_DIVIDER;
	}

	return result;
}

label_str AoC2023_day15::get_label(const std::string hashmap, bool& is_dash_operation) {
	label_str result;
	char tmp;

	result.name = hashmap;
	tmp = result.name.back();
	result.name.pop_back();
	if (tmp == '-') {
		is_dash_operation = true;
		result.focal_length = 0;
	} else {
		if (isdigit(tmp)) {
			result.focal_length = tmp - '0';
			is_dash_operation = false;
			result.name.pop_back();
		} else {
			assert(false);
		}
	}

	return result;
}

uint64_t AoC2023_day15::get_hashes_sum(const bool part2 = false) {
	uint64_t result = 0;

	if (part2) {
		uint64_t hash, subresult;
		bool is_dash_op;
		std::map<uint64_t, std::vector<label_str>> boxes;

		boxes.clear();

		for (size_t i = 0; i < strings_.size(); i++) {
			label_str label = get_label(strings_[i], is_dash_op);
			hash = calculate_hash(label.name);
			size_t idx;

			if (is_dash_op) {
				if (boxes.count(hash)) {
					idx = 0;

					while (idx < boxes[hash].size()) {
						if (boxes[hash][idx].name == label.name) {
							boxes[hash].erase(boxes[hash].begin() + idx);
						} else {
							idx++;
						}
					}
				}
			} else {
				bool replaced = false;

				for (size_t j = 0; j < boxes[hash].size(); j++) {
					if (boxes[hash][j].name == label.name) {
						boxes[hash][j].focal_length = label.focal_length;
						replaced = true;
						break;
					}
				}

				if (!replaced) {
					boxes[hash].push_back(label);
				}
			}
		}

		for (auto& pair : boxes) {
			for (size_t j = 0; j < pair.second.size(); j++) {
				subresult = (pair.first + 1) * (j + 1) * pair.second[j].focal_length;
				result += subresult;
			}
		}

	} else {
		for (size_t i = 0; i < strings_.size(); i++) {
			result += calculate_hash(strings_[i]);
		}
	}

	return result;
}

int32_t AoC2023_day15::get_aoc_day() {
	return 15;
}

int32_t AoC2023_day15::get_aoc_year() {
	return 2023;
}

void AoC2023_day15::tests() {
	uint64_t result;

	if (init({"rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7"})) {
		result = get_hashes_sum();	   // 1320
		result = get_hashes_sum(true); // 145
	}
}

bool AoC2023_day15::part1() {
	int64_t result = 0;

	result = get_hashes_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day15::part2() {
	int64_t result = 0;

	result = get_hashes_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day15 day15;

	return day15.main_execution();
}
