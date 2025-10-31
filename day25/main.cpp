#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <algorithm>
#include <map>
#include <queue>
#include <set>

struct edge_str {
	std::string from;
	std::string to;
	size_t count;

	bool operator<(const edge_str& other) const {
		if (count == other.count) {
			if (from == other.from) {
				return to < other.to;
			} else {
				return from < other.from;
			}
		} else {
			return count < other.count;
		}
	}

	bool operator>(const edge_str& other) const {
		if (count == other.count) {
			if (from == other.from) {
				return to > other.to;
			} else {
				return from > other.from;
			}
		} else {
			return count > other.count;
		}
	}
};

class AoC2023_day25 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, std::set<std::string>> connections_;
	std::set<std::string> nodes_;
	int64_t find_parts_sizes_multiplication(const bool testing);
	int64_t calculate_connected_components(std::map<std::string, std::set<std::string>>& disabled_edges);
	std::vector<std::string> get_path(const std::string& from, const std::string& to);
};

bool AoC2023_day25::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens;
	std::string from;

	connections_.clear();
	nodes_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {

		tokens = split(lines[i], ": ");

		if (tokens.size() != 2) {
			std::cout << "Invalid line format in line " << i + 1 << std::endl;
			return false;
		}

		from = tokens[0];
		nodes_.insert(from);

		tokens = split(tokens[1], " ");

		for (const auto& to : tokens) {
			connections_[from].insert(to);
			connections_[to].insert(from);
			nodes_.insert(to);
		}
	}

	return true;
}

int64_t AoC2023_day25::calculate_connected_components(std::map<std::string, std::set<std::string>>& disabled_edges) {
	std::set<std::string> visited;
	std::queue<std::string> q;

	visited.clear();

	q.push(nodes_.begin()->data());
	visited.insert(nodes_.begin()->data());

	while (!q.empty()) {
		auto current = q.front();
		q.pop();

		for (const auto& neighbor : connections_[current]) {
			if (disabled_edges.find(current) != disabled_edges.end() && disabled_edges[current].find(neighbor) != disabled_edges[current].end()) {
				continue;
			}

			if (visited.find(neighbor) == visited.end()) {
				visited.insert(neighbor);
				q.push(neighbor);
			}
		}
	}

	return visited.size();
}

int64_t AoC2023_day25::find_parts_sizes_multiplication(const bool testing) {
	/*
	The graph has two highly-connected components with the three edges connecting them. If I pick two nodes at random, the shortest path between them may stay
	within one component or may cross from one to the other. If those two components are about equal in size (they're both about 700 nodes in my input), then
	about half those paths will moves between components. If it goes between components, the path will include one of the cut-set edges. That means the cut-set
	edges will appear in many more paths than the other edges.

	That's the idea behind my solution. The steps I took to solve it are:
	 - Generate a few hundred random pairs of nodes and find the paths between them.
	 - Convert the sequences of nodes to the edges used (with getEdges). Count how many times each edge appears in all the paths put together. (This uses a
	Multiset to handle the counting.)
	 - The three most common edges are the cut set.
	 - Remove those edges from the graph
	 - Find the sizes of the two now-disconnected components.
	*/
	int64_t result = 0;
	size_t pairs_count = 200;
	std::set<std::pair<std::string, std::string>> nodes;
	std::set<std::string> path;
	std::vector<std::string> tmp;
	std::map<std::string, std::map<std::string, int32_t>> edge_count = {};
	std::set<edge_str, std::greater<edge_str>> edges;
	std::map<std::string, std::set<std::string>> disabled_edges = {};

	if (testing) {
		pairs_count = 50;
	}

	nodes.clear();
	tmp.insert(tmp.end(), nodes_.begin(), nodes_.end());

	while (nodes.size() < pairs_count) {
		nodes.insert(make_pair(tmp[rand() % tmp.size()], tmp[rand() % tmp.size()]));
	}

	for (const auto& node_pair : nodes) {
		auto path = get_path(node_pair.first, node_pair.second);

		for (size_t i = 0; i < path.size() - 1; i++) {
			auto from = path[i];
			auto to = path[i + 1];

			if (from > to) {
				std::swap(from, to);
			}

			edge_count[from][to]++;
		}
	}

	edges.clear();

	for (const auto& pair : edge_count) {
		for (const auto& inner_pair : pair.second) {
			edge_str edge;
			edge.from = pair.first;
			edge.to = inner_pair.first;
			edge.count = inner_pair.second;
			edges.insert(edge);
		}
	}

	std::vector<edge_str> candidates = {};
	int cnt = 0;
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		candidates.push_back(*it);
		if (++cnt == 6) {
			break;
		}
	}

	for (size_t i = 0; i < candidates.size(); i++) {
		for (size_t j = i + 1; j < candidates.size(); j++) {
			for (size_t k = j + 1; k < candidates.size(); k++) {
				disabled_edges.clear();
				disabled_edges[candidates[i].from].insert(candidates[i].to);
				disabled_edges[candidates[j].from].insert(candidates[j].to);
				disabled_edges[candidates[k].from].insert(candidates[k].to);

				result = calculate_connected_components(disabled_edges);

				if (result < static_cast<int64_t>(nodes_.size())) {
					return result * (nodes_.size() - result);
				}
			}
		}
	}

	return 0;
}

std::vector<std::string> AoC2023_day25::get_path(const std::string& from, const std::string& to) {
	std::set<std::string> history = {};
	std::vector<std::string> path = {};
	std::queue<std::vector<std::string>> q;

	q.push({from});

	while (!q.empty()) {
		auto visited = q.front();
		q.pop();

		if (visited.back() == to) {
			return visited;
		}

		for (const auto& neighbor : connections_[visited.back()]) {
			if (history.count(neighbor)) {
				continue;
			}

			auto new_visited = visited;
			new_visited.push_back(neighbor);
			q.push(new_visited);
			history.insert(neighbor);
		}
	}

	return {};
}

int32_t AoC2023_day25::get_aoc_day() {
	return 25;
}

int32_t AoC2023_day25::get_aoc_year() {
	return 2023;
}

void AoC2023_day25::tests() {
	int64_t result;

	if (init({"jqt: rhn xhk nvd", "rsh: frs pzl lsr", "xhk: hfx", "cmg: qnr nvd lhk bvb", "rhn: xhk bvb hfx", "bvb: xhk hfx", "pzl: lsr hfx nvd", "qnr: nvd",
			  "ntq: jqt hfx bvb xhk", "nvd: lhk", "lsr: lhk", "rzs: qnr cmg lsr rsh", "frs: qnr lhk lsr"})) {
		result = find_parts_sizes_multiplication(true); // 54
	}
}

bool AoC2023_day25::part1() {
	int64_t result = 0;

	result = find_parts_sizes_multiplication(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day25::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day25 day25;

	return day25.main_execution();
}
