#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>
#include <set>

const std::string C_ACCEPTED = "A";
const std::string C_REJECTED = "R";
const std::string C_BEGIN_WORKFLOW = "in";
const std::regex C_REGEX_WORKFLOW("^([a-z]+)\\{(.+),([a-zAR]+)\\}$");
const std::regex C_REGEX_RULE("^([axsm])([<>])(\\d+):([a-zAR]+)$");
const std::regex C_REGEX_PART("^\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}$");
const uint16_t C_RATING_MIN = 1;
const uint16_t C_RATING_MAX = 4000;
const int64_t C_COMBINATIONS_MAX = 4000LL * 4000LL * 4000LL * 4000LL;

static bool get_ranges_intersection(std::pair<uint16_t, uint16_t>& main, const std::pair<uint16_t, uint16_t> r) {
	if (main.first < r.first) {
		if (main.second < r.first) {
			return false;
		}

		main.first = r.first;
	} else {
		if (r.second < main.first) {
			return false;
		}
	}

	main.second = std::min(main.second, r.second);

	return true;
}

struct conditions_str {
	std::pair<uint16_t, uint16_t> x, m, a, s;

	conditions_str() {
		x = m = a = s = {C_RATING_MIN, C_RATING_MAX};
	}

	bool update(const char parameter, std::pair<uint16_t, uint16_t> range) {
		switch (parameter) {
			case 'x':
				return get_ranges_intersection(x, range);
			case 'm':
				return get_ranges_intersection(m, range);
			case 'a':
				return get_ranges_intersection(a, range);
			case 's':
				return get_ranges_intersection(s, range);
			default:
				assert(false);
				return false;
		}
	}
};

struct rule_str {
	char rating;
	uint16_t value;
	bool is_greater;
	std::string send_to;
};

struct workflow_str {
	std::string name;
	std::vector<rule_str> rules;
	std::string send_to;

	bool exists_rule_sending_to(std::string target) {
		for (auto& rule : rules) {
			if (rule.send_to == target) {
				return true;
			}
		}

		return false;
	}
};

struct part_str {
	uint16_t x, m, a, s;
};

class AoC2023_day19 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<part_str> parts_;
	std::map<std::string, workflow_str> workflows_;
	std::map<std::string, std::set<std::string>> backtrack_;
	int64_t get_all_accepted_parts_ratings_sum();
	int64_t get_all_distinct_ratings_combinations_accepted();
	void rekurz(const std::string current, const std::string path, conditions_str cond, std::vector<conditions_str>& conditions);
};

bool AoC2023_day19::init(const std::vector<std::string> lines) {
	bool parts = false;
	std::smatch sm;
	workflow_str wf;
	rule_str rule;
	part_str part;
	std::string item;

	workflows_.clear();
	parts_.clear();
	backtrack_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			parts = true;
			continue;
		}

		if (parts) {
			if (std::regex_match(lines[i], sm, C_REGEX_PART)) {
				part.x = std::stoi(sm.str(1));
				part.m = std::stoi(sm.str(2));
				part.a = std::stoi(sm.str(3));
				part.s = std::stoi(sm.str(4));

				parts_.push_back(part);
			} else {
				std::cout << "Invalid part definition at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			if (std::regex_match(lines[i], sm, C_REGEX_WORKFLOW)) {
				wf.name = sm.str(1);
				item = sm.str(2);
				wf.send_to = sm.str(3);
				backtrack_[wf.send_to].emplace(wf.name);

				std::vector<std::string> rules = split(item, ",");

				wf.rules.clear();

				for (size_t r = 0; r < rules.size(); r++) {
					if (std::regex_match(rules[r], sm, C_REGEX_RULE)) {
						rule.rating = sm.str(1)[0];
						rule.is_greater = sm.str(2) == ">";
						rule.value = std::stoi(sm.str(3));
						rule.send_to = sm.str(4);
						wf.rules.push_back(rule);
						backtrack_[rule.send_to].emplace(wf.name);
					} else {
						std::cout << "Invalid workflow definition at line " << i + 1 << " rule " << r + 1 << std::endl;
						return false;
					}
				}

				workflows_[wf.name] = wf;
			} else {
				std::cout << "Invalid workflow definition at line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	return true;
}

int64_t AoC2023_day19::get_all_accepted_parts_ratings_sum() {
	int64_t result = 0;
	std::vector<part_str> r, a;
	workflow_str wf;
	std::string next;
	uint16_t tmp;
	bool comparison;

	a.clear();
	r.clear();

	for (size_t i = 0; i < parts_.size(); i++) {
		wf = workflows_[C_BEGIN_WORKFLOW];
		next = wf.send_to;

		while (true) {
			for (size_t j = 0; j < wf.rules.size(); j++) {
				switch (wf.rules[j].rating) {
					case 'x':
						tmp = parts_[i].x;
						break;
					case 'm':
						tmp = parts_[i].m;
						break;
					case 'a':
						tmp = parts_[i].a;
						break;
					case 's':
						tmp = parts_[i].s;
						break;
					default:
						assert(false);
						break;
				}

				if (wf.rules[j].is_greater) {
					comparison = tmp > wf.rules[j].value;
				} else {
					comparison = tmp < wf.rules[j].value;
				}

				if (comparison) {
					next = wf.rules[j].send_to;
					break;
				}
			}

			if (next == C_ACCEPTED) {
				a.push_back(parts_[i]);
				break;
			} else if (next == C_REJECTED) {
				r.push_back(parts_[i]);
				break;
			} else {
				wf = workflows_[next];
				next = wf.send_to;
			}
		}
	}

	for (auto& acc : a) {
		result += acc.a + acc.m + acc.s + acc.x;
	}

	return result;
}

int64_t AoC2023_day19::get_all_distinct_ratings_combinations_accepted() {
	int64_t result = 0, subresult;
	std::vector<conditions_str> conditions;
	conditions_str cond;

	rekurz(C_REJECTED, C_REJECTED, cond, conditions);

	for (auto& cond : conditions) {
		subresult = static_cast<int64_t>(cond.x.second - cond.x.first + 1);
		subresult *= static_cast<int64_t>(cond.m.second - cond.m.first + 1);
		subresult *= static_cast<int64_t>(cond.a.second - cond.a.first + 1);
		subresult *= static_cast<int64_t>(cond.s.second - cond.s.first + 1);
		result += subresult;
	}

	result = C_COMBINATIONS_MAX - result;

	return result;
}

void AoC2023_day19::rekurz(const std::string current, const std::string path, conditions_str cond, std::vector<conditions_str>& conditions) {
	conditions_str c1, c2;
	std::pair<uint16_t, uint16_t> range;

	for (auto& prev : backtrack_[current]) {
		c1 = cond;

		for (auto& rule : workflows_[prev].rules) {

			if (rule.send_to == current) {
				if (rule.is_greater) {
					range.first = rule.value + 1;
					range.second = C_RATING_MAX;
				} else {
					range.first = C_RATING_MIN;
					range.second = rule.value - 1;
				}
				c2 = c1;
				if (!c2.update(rule.rating, range)){
					return;
				}

				if (prev == C_BEGIN_WORKFLOW) {
					conditions.push_back(c2);
				} else {
					rekurz(prev, prev + "->" + path, c2, conditions);
				}
			}
			
			if (rule.is_greater) {
				range.first = C_RATING_MIN;
				range.second = rule.value;
			} else {
				range.first = rule.value;
				range.second = C_RATING_MAX;
			}

			if (!c1.update(rule.rating, range)) {
				return;
			}
		}

		if (workflows_[prev].send_to == current) {
			if (prev == C_BEGIN_WORKFLOW) {
				conditions.push_back(c1);
			} else {
				rekurz(prev, prev + "->" + path, c1, conditions);
			}
		}
	}
}

int32_t AoC2023_day19::get_aoc_day() {
	return 19;
}

int32_t AoC2023_day19::get_aoc_year() {
	return 2023;
}

void AoC2023_day19::tests() {
	int64_t result;

	if (init({"px{a<2006:qkq,m>2090:A,rfg}", "pv{a>1716:R,A}", "lnx{m>1548:A,A}", "rfg{s<537:gd,x>2440:R,A}", "qs{s>3448:A,lnx}", "qkq{x<1416:A,crn}",
			  "crn{x>2662:A,R}", "in{s<1351:px,qqz}", "qqz{s>2770:qs,m<1801:hdj,R}", "gd{a>3333:R,R}", "hdj{m>838:A,pv}", "", "{x=787,m=2655,a=1222,s=2876}",
			  "{x=1679,m=44,a=2067,s=496}", "{x=2036,m=264,a=79,s=2244}", "{x=2461,m=1339,a=466,s=291}", "{x=2127,m=1623,a=2188,s=1013}"})) {
		result = get_all_accepted_parts_ratings_sum();			   // 19114
		result = get_all_distinct_ratings_combinations_accepted(); // 167409079868000
	}
}

bool AoC2023_day19::part1() {
	int64_t result = 0;

	result = get_all_accepted_parts_ratings_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day19::part2() {
	int64_t result = 0;

	result = get_all_distinct_ratings_combinations_accepted();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day19 day19;

	return day19.main_execution();
}
