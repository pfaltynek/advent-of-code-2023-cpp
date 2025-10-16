
template <class T> bool ranges_intersection(T r1start, T r1end, T r2start, T r2end) {

	if (r1start > r1end) {
		std::swap(r1start, r1end);
	}
	if (r2start > r2end) {
		std::swap(r2start, r2end);
	}

	if (std::min(r1end, r2end) < std::max(r1start, r2start)) {
		return false;
	} else {
		return true;
	}
}
