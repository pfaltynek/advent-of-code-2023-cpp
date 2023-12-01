#include "./../common/aoc.hpp"
#include <map>

const std::string C_NUMBERS = "0123456789";
const std::map<char, std::string> C_DIGITS = {{'1', "one"}, {'2', "two"},	{'3', "three"}, {'4', "four"}, {'5', "five"},
											  {'6', "six"}, {'7', "seven"}, {'8', "eight"}, {'9', "nine"}};

class AoC2023_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_calibration_values_sum(const bool part2);
	std::vector<std::string> calibration_;
};

bool AoC2023_day01::init(const std::vector<std::string> lines) {
	calibration_ = lines;

	return true;
}

uint64_t AoC2023_day01::get_calibration_values_sum(const bool part2) {
	uint64_t result = 0, one;
	std::string number;
	size_t pos1, pos2, tmp;
	char c1, c2;

	for (auto& cal : calibration_) {
		number.clear();
		pos1 = cal.find_first_of(C_NUMBERS);
		pos2 = cal.find_last_of(C_NUMBERS);

		if (part2) {
			if (pos1 != std::string::npos) {
				c1 = cal[pos1];
			} else {
				pos1 = cal.size() - 1;
				c1 = '0';
			}
			if (pos2 != std::string::npos) {
				c2 = cal[pos2];
			} else {
				c2 = '0';
				pos2 = 0;
			}

			for (auto& pair : C_DIGITS) {
				tmp = cal.find(pair.second);
				if ((tmp != std::string::npos) && (tmp < pos1)) {
					pos1 = tmp;
					c1 = pair.first;
				}

				tmp = cal.rfind(pair.second);
				if ((tmp != std::string::npos) && (tmp > pos2)) {
					pos2 = tmp;
					c2 = pair.first;
				}
			}

			number += c1;
			number += c2;
			one = std::stoi(number);
			result += one;
		} else {
			if ((pos1 != std::string::npos) && (pos2 != std::string::npos)) {
				number += cal[pos1];
				number += cal[pos2];
				one = std::stoi(number);
				result += one;
			}
		}
	}

	return result;
}

int32_t AoC2023_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2023_day01::get_aoc_year() {
	return 2023;
}

void AoC2023_day01::tests() {
	uint64_t result;

	if (init({"1abc2", "pqr3stu8vwx", "a1b2c3d4e5f", "treb7uchet"})) {
		result = get_calibration_values_sum(false); // 142
	}

	if (init({"two1nine", "eightwothree", "abcone2threexyz", "xtwone3four", "4nineeightseven2", "zoneight234", "7pqrstsixteen"})) {
		result = get_calibration_values_sum(true); // 281
	}
}

bool AoC2023_day01::part1() {
	int64_t result = 0;

	result = get_calibration_values_sum(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day01::part2() {
	int64_t result = 0;

	result = get_calibration_values_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day01 day01;

	return day01.main_execution();
}
