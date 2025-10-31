#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "./../common/matrix.hpp"
#include <math.h>
#include <sstream>

const int64_t C_TEST_AREA_MIN = 7;
const int64_t C_TEST_AREA_MAX = 27;
const int64_t C_REAL_AREA_MIN = 200000000000000;
const int64_t C_REAL_AREA_MAX = 400000000000000;

struct hailstone_str {
	int64_t px, py, pz;
	int64_t vx, vy, vz;
};

class AoC2023_day24 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<hailstone_str> hailstones_;
	int64_t find_XY_colisions(const int64_t min, const int64_t max);
	int64_t find_rock_init_position_sum();
};

bool AoC2023_day24::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	hailstone_str hailstone;

	hailstones_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789-,@ ") != std::string::npos) {
			std::cout << "Invalid character in line " << i + 1 << std::endl;
			return false;
		}

		ss.clear();
		ss.str(lines[i]);
		ss >> hailstone.px;
		ss.ignore(2);
		ss >> hailstone.py;
		ss.ignore(2);
		ss >> hailstone.pz;
		ss.ignore(3);
		ss >> hailstone.vx;
		ss.ignore(2);
		ss >> hailstone.vy;
		ss.ignore(2);
		ss >> hailstone.vz;

		hailstones_.push_back(hailstone);
	}

	return true;
}

int64_t AoC2023_day24::find_XY_colisions(const int64_t min, const int64_t max) {
	int64_t result = 0;
	// coordinates are in form x = at + b and y = ct + d
	// the tA = (x - b) / a and tB = (y - d) / c
	// we need to find its form of y = kx + q
	// so k = c/a and q = d - (b*c)/a
	// if we put then yA = yB then x = (qB - qA)/(kA - kB), y = kx + q as above
	// we need then to check if x and y are in range [min, max] and t >= 0

	for (size_t i = 0; i < hailstones_.size(); i++) {
		for (size_t j = i + 1; j < hailstones_.size(); j++) {
			double k1 = static_cast<double>(hailstones_[i].vy) / static_cast<double>(hailstones_[i].vx);
			double q1 = static_cast<double>(hailstones_[i].py) -
						(static_cast<double>(hailstones_[i].px) * static_cast<double>(hailstones_[i].vy)) / static_cast<double>(hailstones_[i].vx);
			double k2 = static_cast<double>(hailstones_[j].vy) / static_cast<double>(hailstones_[j].vx);
			double q2 = static_cast<double>(hailstones_[j].py) -
						(static_cast<double>(hailstones_[j].px) * static_cast<double>(hailstones_[j].vy)) / static_cast<double>(hailstones_[j].vx);

			double x = (q2 - q1) / (k1 - k2);
			double y = k1 * x + q1;

			double tA = static_cast<double>(x - static_cast<double>(hailstones_[i].px)) / static_cast<double>(hailstones_[i].vx);
			double tB = static_cast<double>(y - static_cast<double>(hailstones_[j].py)) / static_cast<double>(hailstones_[j].vy);

			if ((x >= static_cast<double>(min)) && (x <= static_cast<double>(max)) && (y >= static_cast<double>(min)) && (y <= static_cast<double>(max)) &&
				(tA >= 0) && (tB >= 0)) {
				result++;
			}
		}
	}

	return result;
}

int64_t AoC2023_day24::find_rock_init_position_sum() {
	// https://www.ericburden.work/blog/2024/01/02/advent-of-code-day-24/
	/* Say the rock we want to throw to smash all the hailstones starts out at
	 * xR, yR, zR, dxR, dyR, dzR, but we don't actually know what any of those
	 * values is. We can start "simply" by identifying where on each axis
	 * and when as time (t) the rock should collide with one hailstone
	 * (with properties of, say: x, y, z, dx, dy, dz) as:
	 *
	 *      xR + (t * dxR) = x + (t * dx)
	 *      yR + (t * dyR) = y + (t * dy)
	 *      zR + (t * dzR) = z + (t * dz)
	 *
	 * Rearranging to solve for `t`, we get:
	 *
	 *      t = (xR - x)/(dx - dxR) = (yR - y)/(dy - dyR) = (zR - z)/(dz - dzR).
	 *
	 * For _just_ two axes (start with X/Y again), we can isolate the
	 * values related to just the rock (which won't change from one hailstone
	 * to another in order to solve the puzzle) by rearranging the relationship
	 * between the X and Y axes like so:
	 *
	 *      (xR - x)/(dx - dxR) = (yR - y)/(dy - dyR)
	 *      (xR - x)(dy - dyR)  = (yR - y)(dx - dxR)
	 *      xR*dy - x*dy - xR*dyR + x*dyR = yR*dx - yR*dxR - y*dx + y*dxR
	 *      yR*dxR - xR*dyR = yR*dx - y*dx + y*dxR - xR*dy + x*dy - x*dyR
	 *
	 * Because the terms (yR*dxR - xR*dyR) should be the same no matter which
	 * hailstone we consider (in order to hit all the hailstones), we can
	 * alternatively consider another hailstone with properties of, say:
	 * x', y', z', dx', dy', dz', like so:
	 *
	 *      yR*dxR - xR*dyR = yR*dx' - y'*dx' + y'*dxR - xR*dy' + x'*dy' - x'*dyR
	 *
	 * Because (yR*dxR - xR*dyR) is unchanging, it must be true that:
	 *
	 *      yR*dx - y*dx + y*dxR - xR*dy + x*dy - x*dyR = yR*dx' - y'*dx' + y'*dxR - xR*dy' + x'*dy' - x'*dyR
	 *      (dy'-dy)xR + (dx - dx')yR + (y - y')dxR + (x' - x)dyR = y*dx - x*dy -y'*dx' + x'dy'
	 *
	 * Since we need to solve for the properties of the rock, we can substitute
	 * the actual values from any pair of hailstones into this equation. We'll
	 * need at least four pairs of hailstones to solve for the four unknowns.
	 *
	 * We can perform the same rearrangement for the X and Z axes like so:
	 *
	 *      (xR - x)/(dx - dxR) = (zR - z)/(dz - dzR)
	 *      (xR - x)(dz - dzR)  = (zR - z)(dx - dxR)
	 *      xR*dz - x*dz - xR*dzR + x*dzR = zR*dx - zR*dxR - z*dx + z*dxR
	 *      zR*dxR - xR*dzR = zR*dx  -  z*dx  + z*dxR  - xR*dz  + x*dz   - x*dzR
	 *                      = zR*dx' - z'*dx' + z'*dxR - xR*dz' + x'*dz' - x'*dzR
	 *      zR*dx - z*dx + z*dxR - xR*dz + x*dz  - x*dzR = zR*dx' - z'*dx' + z'*dxR - xR*dz' + x'*dz' - x'*dzR
	 *      (dz'-dz)xR + (dx - dx')zR + (z - z')dxR + (x' - x)dzR = z*dx - x*dz -z'*dx' + x'dz'
	 *
	 * The neat thing is, if we already know xR and dxR from solving the first set
	 * of equations, then we only have two unknowns remaining (zR and dzR), for
	 * which we only need two pairs of hailstones by rearranging the system of
	 * equations like:
	 *
	 *       (dx - dx')zR + (x' - x)dzR = z*dx - x*dz -z'*dx' + x'dz' - (dz'-dz)xR - (z - z')dxR
	 */

	int64_t result = 0;
	std::vector<std::vector<long double>> equations;
	std::vector<long double> GEresult, row;

	equations.clear();

	for (size_t i = 0; i < 4; i++) {
		row.clear();
		row.push_back(hailstones_[i + 1].vy - hailstones_[i].vy);
		row.push_back(hailstones_[i].vx - hailstones_[i + 1].vx);
		row.push_back(hailstones_[i].py - hailstones_[i + 1].py);
		row.push_back(hailstones_[i + 1].px - hailstones_[i].px);
		row.push_back((hailstones_[i].py * hailstones_[i].vx) + (-hailstones_[i].px * hailstones_[i].vy) + (-hailstones_[i + 1].py * hailstones_[i + 1].vx) +
					  (hailstones_[i + 1].px * hailstones_[i + 1].vy));
		equations.push_back(row);
	}

	GEresult = gauss_elim(equations);
	result = std::round(GEresult[0]) + std::round(GEresult[1]);

	equations.clear();

	for (size_t i = 0; i < 2; i++) {
		row.clear();
		row.push_back(hailstones_[i].vx - hailstones_[i + 1].vx);
		row.push_back(hailstones_[i + 1].px - hailstones_[i].px);
		row.push_back((hailstones_[i].pz * hailstones_[i].vx) +
					 (-hailstones_[i].px * hailstones_[i].vz) +
					 (-hailstones_[i + 1].pz * hailstones_[i + 1].vx) +
					 (hailstones_[i + 1].px * hailstones_[i + 1].vz) +
					 (-((hailstones_[i + 1].vz - hailstones_[i].vz) * GEresult[0])) +
					 (-((hailstones_[i].pz - hailstones_[i + 1].pz) * GEresult[2])));
		equations.push_back(row);
	}

	GEresult = gauss_elim(equations);

	result += std::round(GEresult[0]);

	return result;
}

int32_t AoC2023_day24::get_aoc_day() {
	return 24;
}

int32_t AoC2023_day24::get_aoc_year() {
	return 2023;
}

void AoC2023_day24::tests() {
	int64_t result;

	if (init({"19, 13, 30 @ -2,  1, -2", "18, 19, 22 @ -1, -1, -2", "20, 25, 34 @ -2, -2, -4", "12, 31, 28 @ -1, -2, -1", "20, 19, 15 @  1, -5, -3"})) {
		result = find_XY_colisions(C_TEST_AREA_MIN, C_TEST_AREA_MAX); // 2
		result = find_rock_init_position_sum();						  // 47
	}
}

bool AoC2023_day24::part1() {
	int64_t result = 0;

	result = find_XY_colisions(C_REAL_AREA_MIN, C_REAL_AREA_MAX);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2023_day24::part2() {
	int64_t result = 0;

	result = find_rock_init_position_sum();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2023_day24 day24;

	return day24.main_execution();
}
