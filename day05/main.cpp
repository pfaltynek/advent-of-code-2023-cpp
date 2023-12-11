#include "./../common/aoc.hpp"
#include <sstream>

const std::string C_SEEDS_HEADER = "seeds:";
const std::string C_SEED_TO_SOIL_HEADER = "seed-to-soil map:";
const std::string C_SOIL_TO_FERTILIZER_HEADER = "soil-to-fertilizer map:";
const std::string C_FERTILIZER_TO_WATER_HEADER = "fertilizer-to-water map:";
const std::string C_WATER_TO_LIGHT_HEADER = "water-to-light map:";
const std::string C_LIGHT_TO_TEMPERATURE_HEADER = "light-to-temperature map:";
const std::string C_TEMPERATURE_TO_HUMIDITY = "temperature-to-humidity map:";
const std::string C_HUMIDITY_TO_LOCATION_HEADER = "humidity-to-location map:";

enum input_part_t {
	E_INPUT_SEEDS,
	E_INPUT_SEED2SOIL,
	E_INPUT_SOIL2FERTILIZER,
	E_INPUT_FERTILIZER2WATER,
	E_INPUT_WATER2LIGHT,
	E_INPUT_LIGHT2TEMPERATURE,
	E_INPUT_TEMPERATURE2HUMIDITY,
	E_INPUT_HUMIDITY2LOCATION
};

struct conversion_map_str {
	int64_t src_min, src_max;
	int64_t dest_min, dest_max;
	int64_t size;
	int64_t diff;

	void clear() {
		src_min = src_max = dest_min = dest_max = size = diff = 0;
	}
};

class AoC2023_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_lowest_location_number(const bool part2);
	int64_t convert_by_map(const int64_t value, std::vector<conversion_map_str>& map);
	int64_t convert_totally(const int64_t value);
	void print_input();
	std::vector<uint64_t> seeds_;
	std::vector<conversion_map_str> seed2soil_, soil2fertilizer_, fertilizer2water_, water2light_, light2temperature_, temperature2humidity_,
		humidity2location_;
};

void AoC2023_day05::print_input() {
	std::cout << std::endl;

	std::cout << C_SEEDS_HEADER;

	for (size_t i = 0; i < seeds_.size(); i++) {
		std::cout << " " << seeds_[i];
	}
	std::cout << std::endl << std::endl;

	std::cout << C_SEED_TO_SOIL_HEADER << std::endl;
	for (size_t i = 0; i < seed2soil_.size(); i++) {
		std::cout << seed2soil_[i].dest_min << " " << seed2soil_[i].src_min << " " << seed2soil_[i].size << std::endl;
	}
	std::cout << std::endl;

	std::cout << C_SOIL_TO_FERTILIZER_HEADER << std::endl;
	for (size_t i = 0; i < soil2fertilizer_.size(); i++) {
		std::cout << soil2fertilizer_[i].dest_min << " " << soil2fertilizer_[i].src_min << " " << soil2fertilizer_[i].size << std::endl;
	}
	std::cout << std::endl;

	std::cout << C_FERTILIZER_TO_WATER_HEADER << std::endl;
	for (size_t i = 0; i < fertilizer2water_.size(); i++) {
		std::cout << fertilizer2water_[i].dest_min << " " << fertilizer2water_[i].src_min << " " << fertilizer2water_[i].size << std::endl;
	}
	std::cout << std::endl;

	std::cout << C_WATER_TO_LIGHT_HEADER << std::endl;
	for (size_t i = 0; i < water2light_.size(); i++) {
		std::cout << water2light_[i].dest_min << " " << water2light_[i].src_min << " " << water2light_[i].size << std::endl;
	}
	std::cout << std::endl;

	std::cout << C_LIGHT_TO_TEMPERATURE_HEADER << std::endl;
	for (size_t i = 0; i < light2temperature_.size(); i++) {
		std::cout << light2temperature_[i].dest_min << " " << light2temperature_[i].src_min << " " << light2temperature_[i].size << std::endl;
	}
	std::cout << std::endl;

	std::cout << C_TEMPERATURE_TO_HUMIDITY << std::endl;
	for (size_t i = 0; i < temperature2humidity_.size(); i++) {
		std::cout << temperature2humidity_[i].dest_min << " " << temperature2humidity_[i].src_min << " " << temperature2humidity_[i].size << std::endl;
	}
	std::cout << std::endl;

	std::cout << C_HUMIDITY_TO_LOCATION_HEADER << std::endl;
	for (size_t i = 0; i < humidity2location_.size(); i++) {
		std::cout << humidity2location_[i].dest_min << " " << humidity2location_[i].src_min << " " << humidity2location_[i].size << std::endl;
	}
	std::cout << std::endl << std::endl;
}

bool AoC2023_day05::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	std::vector<std::string> section;
	int64_t tmp;
	input_part_t part = E_INPUT_SEEDS;
	bool all_parts = false;
	std::string txt;
	bool title = true;
	conversion_map_str conv_map;

	seed2soil_.clear();
	soil2fertilizer_.clear();
	fertilizer2water_.clear();
	water2light_.clear();
	light2temperature_.clear();
	temperature2humidity_.clear();
	humidity2location_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		switch (part) {
			case E_INPUT_SEEDS:
				if (title) {
					ss.clear();
					ss.str(lines[i]);

					seeds_.clear();
					ss >> txt;

					if (txt != C_SEEDS_HEADER) {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}

					while (!ss.eof()) {
						ss >> tmp;
						seeds_.push_back(tmp);
					}
					title = false;
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_SEED2SOIL;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				}
				break;
			case E_INPUT_SEED2SOIL:
				if (title) {
					if (lines[i] == C_SEED_TO_SOIL_HEADER) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_SOIL2FERTILIZER;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						seed2soil_.push_back(conv_map);
					}
				}
				break;
			case E_INPUT_SOIL2FERTILIZER:
				if (title) {
					if (lines[i] == C_SOIL_TO_FERTILIZER_HEADER) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_FERTILIZER2WATER;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						soil2fertilizer_.push_back(conv_map);
					}
				}
				break;
			case E_INPUT_FERTILIZER2WATER:
				if (title) {
					if (lines[i] == C_FERTILIZER_TO_WATER_HEADER) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_WATER2LIGHT;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						fertilizer2water_.push_back(conv_map);
					}
				}
				break;
			case E_INPUT_WATER2LIGHT:
				if (title) {
					if (lines[i] == C_WATER_TO_LIGHT_HEADER) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_LIGHT2TEMPERATURE;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						water2light_.push_back(conv_map);
					}
				}
				break;
			case E_INPUT_LIGHT2TEMPERATURE:
				if (title) {
					if (lines[i] == C_LIGHT_TO_TEMPERATURE_HEADER) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_TEMPERATURE2HUMIDITY;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						light2temperature_.push_back(conv_map);
					}
				}
				break;
			case E_INPUT_TEMPERATURE2HUMIDITY:
				if (title) {
					if (lines[i] == C_TEMPERATURE_TO_HUMIDITY) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_HUMIDITY2LOCATION;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						temperature2humidity_.push_back(conv_map);
					}
				}
				break;
			case E_INPUT_HUMIDITY2LOCATION:
				if (title) {
					if (lines[i] == C_HUMIDITY_TO_LOCATION_HEADER) {
						title = false;
					} else {
						std::cout << "Invalid input format at line " << i + 1 << std::endl;
						return false;
					}
				} else {
					if (lines[i].empty()) {
						title = true;
						part = E_INPUT_SEEDS;
					} else {
						ss.clear();
						ss.str(lines[i]);
						conv_map.clear();
						ss >> conv_map.dest_min;
						ss >> conv_map.src_min;
						ss >> conv_map.size;
						conv_map.src_max = conv_map.src_min + conv_map.size - 1;
						conv_map.dest_max = conv_map.dest_min + conv_map.size - 1;
						conv_map.diff = conv_map.dest_min - conv_map.src_min;
						humidity2location_.push_back(conv_map);
						all_parts = true;
					}
				}
				break;
		}
	}

	// print_input();

	return all_parts;
}

int64_t AoC2023_day05::convert_by_map(const int64_t value, std::vector<conversion_map_str>& map) {
	for (size_t i = 0; i < map.size(); i++) {
		if ((map[i].src_min <= value) && ((map[i].src_max) >= value)) {
			return value + map[i].diff;
		}
	}

	return value;
}

int64_t AoC2023_day05::convert_totally(const int64_t value) {
	uint64_t val;

	val = convert_by_map(value, seed2soil_);
	val = convert_by_map(val, soil2fertilizer_);
	val = convert_by_map(val, fertilizer2water_);
	val = convert_by_map(val, water2light_);
	val = convert_by_map(val, light2temperature_);
	val = convert_by_map(val, temperature2humidity_);
	val = convert_by_map(val, humidity2location_);

	return val;
}

int64_t AoC2023_day05::get_lowest_location_number(const bool part2 = false) {
	uint64_t result = UINT64_MAX;
	uint64_t converted;
	std::vector<uint64_t> seed_ranges;

	if (part2) {
		seed_ranges = seeds_;
	} else {
		seed_ranges.clear();

		for (size_t i = 0; i < seeds_.size(); i++) {
			seed_ranges.push_back(seeds_[i]);
			seed_ranges.push_back(1);
		}
	}

	for (size_t r = 0; r < seed_ranges.size(); r += 2) {
		std::cout << "Range " << seed_ranges[r] << " , " << seed_ranges[r + 1] << std::endl;

		for (size_t i = seed_ranges[r]; i < seed_ranges[r] + seed_ranges[r + 1]; i++) {

			converted = convert_totally(i);

			if (converted < result) {
				result = converted;
			}
		}
	}

	return result;
}

int32_t AoC2023_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2023_day05::get_aoc_year() {
	return 2023;
}

void AoC2023_day05::tests() {
	uint64_t result;

	if (init({"seeds: 79 14 55 13",
			  "",
			  "seed-to-soil map:",
			  "50 98 2",
			  "52 50 48",
			  "",
			  "soil-to-fertilizer map:",
			  "0 15 37",
			  "37 52 2",
			  "39 0 15",
			  "",
			  "fertilizer-to-water map:",
			  "49 53 8",
			  "0 11 42",
			  "42 0 7",
			  "57 7 4",
			  "",
			  "water-to-light map:",
			  "88 18 7",
			  "18 25 70",
			  "",
			  "light-to-temperature map:",
			  "45 77 23",
			  "81 45 19",
			  "68 64 13",
			  "",
			  "temperature-to-humidity map:",
			  "0 69 1",
			  "1 0 69",
			  "",
			  "humidity-to-location map:",
			  "60 56 37",
			  "56 93 4"})) {
		result = get_lowest_location_number();	   // 35
		result = get_lowest_location_number(true); // 46
	}
}

bool AoC2023_day05::part1() {
	int64_t result = 0;

	result = get_lowest_location_number();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day05::part2() {
	int64_t result = 0;

	result = get_lowest_location_number(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day05 day05;

	return day05.main_execution();
}
