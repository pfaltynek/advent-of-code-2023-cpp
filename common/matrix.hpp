#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_
#include <cstdint>
#include <stdexcept>
#include <vector>

// https://cs.wikipedia.org/wiki/Gaussova_elimina%C4%8Dn%C3%AD_metoda
std::vector<long double> gauss_elim(const std::vector<std::vector<long double>> matrix) {
	std::vector<std::vector<long double>> m = matrix;
    std::vector<long double> result = {};

	uint32_t rows = m.size();
	uint32_t cols = m[0].size();

	if (rows + 1 != cols) {

		throw std::invalid_argument("Invalid matrix dimensions for Gauss elimination");
		return {};
	}

	for (size_t i = 1; i < m.size(); i++) {
		if (m[i].size() != cols) {
			throw std::invalid_argument("Invalid matrix dimensions for Gauss elimination");
			return {};
		}
	}

	// normalize rows - get value 1 into pivot fields
	for (size_t r = 0; r < m.size(); r++) {
		long double pivot = m[r][r];
		for (size_t c = 0; c < m[r].size(); c++) {
			m[r][c] /= pivot;
		}

		// eliminate columns
		for (size_t rr = 0; rr < m.size(); rr++) {
			if (rr != r) {
				long double factor = m[rr][r];
				for (size_t c = 0; c < m[r].size(); c++) {
					m[rr][c] -= factor * m[r][c];
				}
			}
		}
	}

    for (size_t r = 0; r < m.size(); r++) {
        result.push_back(m[r].back());
    }
    
	return result;
}

#endif
