#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <algorithm>
#include <map>
#include <queue>
#include <sstream>


const std::string C_BROADCASTER_NAME = "broadcaster";
const std::string C_BUTTON_MODULE_NAME = "button";
const std::string C_TARGET_MODULE_NAME = "rx";
const int64_t C_PART1_BUTTON_PUSHES_COUNT = 1000;

enum module_type_t { E_MODULE_FLIP_FLOP, E_MODULE_CONJUNCTION, E_MODULE_BROADCASTER };

struct pulse_out_str {
	std::string name;
	bool high_pulse;
};

struct pulse_str {
	std::string from, to;
	bool high_pulse;
};

struct module_str {
	module_type_t module_type;
	std::string name;
	std::map<std::string, bool> states;
	std::vector<std::string> targets;
	bool flip_flop_state;

	void init_flip_flop(const std::string mname, const std::vector<std::string> ouputs) {
		init(E_MODULE_FLIP_FLOP, mname, ouputs);
	}

	void init_conjunction(const std::string mname, const std::vector<std::string> ouputs) {
		init(E_MODULE_CONJUNCTION, mname, ouputs);
	}

	void init_broadcaster(const std::string mname, const std::vector<std::string> ouputs) {
		init(E_MODULE_BROADCASTER, mname, ouputs);
	}

	std::vector<pulse_out_str> receive_pulse(const std::string source, const bool high_pulse) {
		std::vector<pulse_out_str> result;
		bool pulse = false;

		result.clear();

		switch (module_type) {
			case E_MODULE_BROADCASTER:
				for (auto& dest : targets) {
					result.push_back({dest, high_pulse});
				}
				break;

			case E_MODULE_FLIP_FLOP:
				if (!high_pulse) {
					flip_flop_state = !flip_flop_state;

					for (auto& dest : targets) {
						result.push_back({dest, flip_flop_state});
					}
				}
				break;

			case E_MODULE_CONJUNCTION:
				states[source] = high_pulse;

				for (auto& pair : states) {
					if (!pair.second) {
						pulse = true;
						break;
					}
				}

				for (auto& dest : targets) {
					result.push_back({dest, pulse});
				}
				break;

			default:
				assert(false);
				break;
		}

		return result;
	}

	void add_module_input(const std::string name) {
		if (module_type == E_MODULE_CONJUNCTION) {
			states[name] = false;
		}
	}

	void reset() {
		flip_flop_state = false;

		for (auto& pair : states) {
			pair.second = false;
		}
	}

  private:
	void init(const module_type_t mod_type, const std::string mname, const std::vector<std::string> outputs) {
		module_type = mod_type;
		flip_flop_state = false;
		name = mname;

		states.clear();

		targets = outputs;
	}
};

class AoC2023_day20 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, module_str> modules_;
	int64_t get_pulses_total(const bool part2, const int64_t rounds);
};

bool AoC2023_day20::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	char c;
	std::string name, tmp;
	std::vector<std::string> names;
	module_str module;

	modules_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		names.clear();

		c = ss.get();
		ss >> name;
		ss.ignore(4); // " -> "

		while (!ss.eof()) {
			ss >> tmp;

			if (tmp.back() == ',') {
				tmp.pop_back();
			}

			names.push_back(tmp);
		}

		switch (c) {
			case '&':
				module.init_conjunction(name, names);
				break;
			case '%':
				module.init_flip_flop(name, names);
				break;
			case 'b':
				name = c + name;
				module.init_broadcaster(name, names);
				break;
			default:
				std::cout << "Invalid module definition at line " << i + 1 << std::endl;
				return false;
		}

		modules_[module.name] = module;
	}

	if (!modules_.count(C_BROADCASTER_NAME)) {
		std::cout << "Module BROADCASTER definition missing" << std::endl;
		return false;
	}

	for (auto& pair : modules_) {
		for (auto& target : pair.second.targets) {
			modules_[target].add_module_input(pair.first);
		}
	}

	return true;
}

int64_t AoC2023_day20::get_pulses_total(const bool part2, const int64_t rounds) {
	int64_t result = 0, pulses_high_count = 0, pulses_low_count = 0, counter = 0;
	std::queue<pulse_str> q = {};
	pulse_str pulse;
	std::vector<pulse_out_str> out_pulses;
	std::string from;
	bool finished = false;
	std::vector<std::string> part2parts = {};

	if (part2) {
		for (auto& module : modules_) {
			module.second.reset(); // for part2 all modules have to be reset into default state

			// acording to general sugestion on reddit is much simpler and quicker to go back in tree a bit and find cycle numbers
			// of all sources of conjuction module which outputs to 'rx'
			if ((module.second.module_type == E_MODULE_CONJUNCTION) && (module.second.targets[0] == C_TARGET_MODULE_NAME)) {
				for (auto& state : module.second.states) {
					part2parts.push_back(state.first);
				}
			}
		}

		result = 1;
	}

	while (true) {
		pulse.from = C_BUTTON_MODULE_NAME;
		pulse.to = C_BROADCASTER_NAME;
		pulse.high_pulse = false;

		q.push(pulse);
		while (q.size()) {
			pulse = q.front();
			q.pop();
			if (pulse.high_pulse) {
				pulses_high_count++;
			} else {
				pulses_low_count++;
			}

			if (!modules_.count(pulse.to)) {
				assert(false);
			}

			if (part2) {
				auto it = std::find(part2parts.begin(), part2parts.end(), pulse.from);
				if (it != part2parts.end() && pulse.high_pulse) {
					result *= (counter + 1);

					part2parts.erase(it);

					if (part2parts.empty()) {
						finished = true;
					}
				}
			}

			out_pulses = modules_[pulse.to].receive_pulse(pulse.from, pulse.high_pulse);
			from = pulse.to;

			for (size_t j = 0; j < out_pulses.size(); j++) {
				pulse.from = from;
				pulse.to = out_pulses[j].name;
				pulse.high_pulse = out_pulses[j].high_pulse;

				q.push(pulse);
			}
		}

		counter++;

		if (part2) {
			if (finished) {
				break;
			}
		} else {
			if (counter == rounds) {
				result = pulses_high_count * pulses_low_count;
				break;
			}
		}
	}

	return result;
}

int32_t AoC2023_day20::get_aoc_day() {
	return 20;
}

int32_t AoC2023_day20::get_aoc_year() {
	return 2023;
}

void AoC2023_day20::tests() {
	int64_t result;

	if (init({"broadcaster -> a, b, c", "%a -> b", "%b -> c", "%c -> inv", "&inv -> a"})) {
		result = get_pulses_total(false, C_PART1_BUTTON_PUSHES_COUNT); // 32000000
	}

	if (init({"broadcaster -> a", "%a -> inv, con", "&inv -> b", "%b -> con", "&con -> output"})) {
		result = get_pulses_total(false, C_PART1_BUTTON_PUSHES_COUNT); // 11687500
	}
}

bool AoC2023_day20::part1() {
	int64_t result = 0;

	result = get_pulses_total(false, C_PART1_BUTTON_PUSHES_COUNT);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day20::part2() {
	int64_t result = 0;

	result = get_pulses_total(true, C_PART1_BUTTON_PUSHES_COUNT);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day20 day20;

	return day20.main_execution();
}
